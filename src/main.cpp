
#include <iostream>
#include "JuceHeader.h"
#include "mongoose.h"
#include "NonDeletingOutputStream.h"
#include "urlutils.h"

#define PLUGIN_POOL_SIZE 0
#define PLUGIN_REL_PATH "plugins/miniTERA.vst"

// #include <csignal>
// #define EMBED_BREAKPOINT raise(SIGINT)

using namespace std;
using namespace juce;

static std::ostream &DBG = cerr;
class DebugLogger : public Logger {
  void logMessage(const String &message) {
    DBG << message << endl;
  }
};
static DebugLogger DEBUG_LOGGER;

struct ThreadSafePlugin {
  CriticalSection crit;
  ScopedPointer<AudioPluginInstance> instance;

  ThreadSafePlugin(AudioPluginInstance *_instance) : instance(_instance) {}
};

static OwnedArray<ThreadSafePlugin, CriticalSection> pluginPool;
static File cwd = File::getCurrentWorkingDirectory();

String resolveRelativePath(String relativePath) {
  // We need to use a cached version of the working directory,
  // since plugin initialization is done in a non-threadsafe way at the moment.
  return cwd.getChildFile(relativePath).getFullPathName();
}

// The caller is responsible for deleting the object that is returned
AudioPluginInstance *createSynthInstance() {
  AudioPluginFormatManager pluginManager;
  pluginManager.addDefaultFormats();

  PluginDescription desc;
  desc.fileOrIdentifier = resolveRelativePath(PLUGIN_REL_PATH);
  // DBG << desc.fileOrIdentifier << endl;
  desc.uid = 0;

  String errorMessage;
  AudioPluginInstance *instance = pluginManager.createPluginInstance(desc, errorMessage);
  
  if (!instance) {
    DBG << "Error creating plugin instance: " << errorMessage << endl;
    exit(1);
  }

  // Force initialization on the main thread. This preparation will redone with proper values later.
  instance->prepareToPlay(44100, 512);

  return instance;
}

struct PluginRequestParameters {
  int presetNumber;
  bool listParameters;
  int sampleRate, blockSize, bitDepth;
  int nChannels;
  int midiChannel, midiPitch, midiVelocity;
  float noteSeconds, renderSeconds;
  String formatName, contentType;
  NamedValueSet parameters, indexedParameters;

  PluginRequestParameters(const var &params = var::null) {
    #define PLUGIN_REQUEST_PARAMETERS_DEFAULT(name, default) \
      if (params[#name]) {name = params[#name];} else {name = default;}
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(presetNumber, -1)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(listParameters, false)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(sampleRate, 44100)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(blockSize, 2056)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(bitDepth, 16)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(nChannels, 2)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(renderSeconds, 1.5f)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(midiChannel, 1)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(midiPitch, 60)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(midiVelocity, 120)
    PLUGIN_REQUEST_PARAMETERS_DEFAULT(noteSeconds, 0.75f)

    #define PLUGIN_REQUEST_PARAMETER_DICT(name) { \
      DynamicObject *paramDynObj = params[#name].getDynamicObject(); \
      if (paramDynObj) name = paramDynObj->getProperties(); }

    PLUGIN_REQUEST_PARAMETER_DICT(parameters)
    PLUGIN_REQUEST_PARAMETER_DICT(indexedParameters)
  }

  const char *getFormatName() const {
    return listParameters ? "json" : "wav";
  }

  const char *getContentType() const {
    return listParameters ? "application/json" : "audio/vnw.wave";
  }
};

void pluginParametersSet(AudioPluginInstance *instance, const NamedValueSet &parameters) {
  int numParams = instance->getNumParameters();
  for (int i = 0; i < numParams; ++i) {
    Identifier name = instance->getParameterName(i);
    var val = parameters.getWithDefault(name, var::null);
    if (!val.isVoid()) {
      DBG << "Setting named parameter " << i << " - " << name.toString() << " to " << (float)val;
      instance->setParameter(i, (float)val);
      DBG << "... reported as " << instance->getParameter(i) << endl;
    }
  }
}

void pluginParametersSetIndexed(AudioPluginInstance *instance, const NamedValueSet &indexedParameters) {
  int numParams = instance->getNumParameters();
  for (int j = 0, m = indexedParameters.size(); j < m; ++j) {
    Identifier name = indexedParameters.getName(j);
    int i = name.toString().getIntValue();
    var val = indexedParameters.getValueAt(j);
    if (!val.isVoid() && i >= 0 && i < numParams) {
      DBG << "Setting indexed parameter " << i << " - " << name.toString() << " to " << (float)val;
      instance->setParameter(i, (float)val);
      DBG << "... reported as " << instance->getParameter(i) << endl;
    }
  }
}

bool handlePluginRequest(const PluginRequestParameters &params, OutputStream &ostream, 
                         ThreadSafePlugin *plugin = nullptr) {
  if (!plugin) {
    // It's very possible that all of this was a premature optimization.
    // For VSTs at least, code loading and caching is handled by ModuleHandle::findOrCreateModule,
    // and each instantiation only requires a couple of disc hits for working directory setting.
    // On the other hand, we want to make sure that each audio request has a "fresh" instance.
    // The easiest way to do this is by bypassing the instance pool and instantiating on demand.

    #if PLUGIN_POOL_SIZE

    // Recurse with a plugin from the pool, locking on it.
    // Keep trying with a delay until a timeout occurs.
    const int TIMEOUT = 5000, WAIT = 200;
    int64 startTime = Time::currentTimeMillis();

    while (Time::currentTimeMillis() < startTime + TIMEOUT) {
      int i = 0;
      while ((plugin = pluginPool[i++])) {
        const ScopedTryLock pluginTryLock(plugin->crit);
        if (pluginTryLock.isLocked()) {
          DBG << "Handling with plugin " << i << endl;
          return handlePluginRequest(params, ostream, plugin);
        }
      }
      DBG << "Trying again in " << WAIT << endl;
      Thread::sleep(WAIT);
    }

    // If we were unable to obtain a lock, return failure.
    DBG << "Timeout" << endl;
    return false;

    #else

    ThreadSafePlugin temporaryPlugin(createSynthInstance());
    return handlePluginRequest(params, ostream, &temporaryPlugin);

    #endif
  }
  else {
    // Re-acquire or acquire the lock.
    const ScopedLock pluginLock(plugin->crit);
    AudioPluginInstance *instance = plugin->instance; // unmanaged, for simplicity

    // Attempt to reset the plugin in all ways possible.
    instance->reset();
    // Setting default parameters here causes miniTERA to become unresponsive to parameter settings.
    // It's possible that it's effectively pressing some interface buttons that change the editor mode entirely.
    // It's not necessary anyways if the plugin instance has been freshly created (see above).
    // pluginParametersOldNewFallback(instance, nullptr, &pluginDefaults); // note that the defaults may be empty
    instance->setCurrentProgram(0);

    // Load preset if specified, before listing or modifying parameters!
    if (params.presetNumber >= 0 && params.presetNumber < instance->getNumPrograms()) {
      DBG << "Setting program/preset: " << params.presetNumber << endl;
      instance->setCurrentProgram(params.presetNumber);
    }
    int currentProgram = instance->getCurrentProgram();
    DBG << "Current program/preset: " << currentProgram << " - " << instance->getProgramName(currentProgram) << endl;

    // Set parameters, starting with named, then indexed
    pluginParametersSet(instance, params.parameters);
    pluginParametersSetIndexed(instance, params.indexedParameters);

    // If parameters requested, output them and return
    if (params.listParameters) {
      DBG << "Rendering parameter list: # parameters " << instance->getNumPrograms() << endl;

      // Output each parameter setting in two places: 
      // an indexed array and a dictionary by name
      // All DynamicObjects created will be freed when their var's leave scope.
      DynamicObject *outer = new DynamicObject();
      DynamicObject *innerParams = new DynamicObject();
      var indexedParamVar;
      {
        for (int i = 0, n = instance->getNumParameters(); i < n; ++i) {
          String name = instance->getParameterName(i);
          float val = instance->getParameter(i);
          innerParams->setProperty(name, val);

          DynamicObject *indexedInnerObj = new DynamicObject();
          indexedInnerObj->setProperty("index", i);
          indexedInnerObj->setProperty("name", name);
          indexedInnerObj->setProperty("value", val);
          indexedParamVar.append(var(indexedInnerObj)); // frees indexedInnerObj when this scope ends
        }
      }
      outer->setProperty(Identifier("parameters"), var(innerParams));
      outer->setProperty(Identifier("indexedParameters"), indexedParamVar);

      // List presets/programs.
      var progVar;
      {
        for (int i = 0, n = instance->getNumPrograms(); i < n; ++i) {
          progVar.append(var(instance->getProgramName(i)));
        }
      }
      outer->setProperty(Identifier("presets"), progVar);

      var outerVar(outer);
      JSON::writeToStream(ostream, outerVar);
      // DBG << JSON::toString(outerVar, true /* allOnOneLine */) << endl;

      return true;
    }

    // Now attempt to render audio.
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    OptionalScopedPointer<AudioFormat> outputFormat(formatManager.findFormatForFileExtension(params.getFormatName()), false);
    if (!outputFormat) return false;

    instance->setNonRealtime(true);
    instance->prepareToPlay(params.sampleRate, params.blockSize);
    instance->setNonRealtime(true);

    // The writer takes ownership of the output stream; the  writer will delete it when the writer leaves scope.
    // Therefore, we pass a special pointer class that does not allow the writer to delete it.
    OutputStream *ostreamNonDeleting = new NonDeletingOutputStream(&ostream);
    ScopedPointer<AudioFormatWriter> writer(outputFormat->createWriterFor(ostreamNonDeleting,
      params.sampleRate, params.nChannels, params.bitDepth,
      StringPairArray(), 0));

    // Create a MIDI buffer
    MidiBuffer midiBuffer;
    midiBuffer.addEvent(MidiMessage::noteOn(params.midiChannel, (uint8)params.midiPitch, (uint8)params.midiVelocity),
                        0 /* time */);
    midiBuffer.addEvent(MidiMessage::allNotesOff(params.midiChannel), 
                        params.noteSeconds * params.sampleRate);

    AudioSampleBuffer buffer(params.nChannels, params.blockSize);
    int numBuffers = (int)(params.renderSeconds * params.sampleRate / params.blockSize);
    for (int i = 0; i < numBuffers; ++i) {
      // DBG << "Processing block " << i << "..." << flush;
      instance->processBlock(buffer, midiBuffer);
      // DBG << " left RMS level " << buffer.getRMSLevel(0, 0, params.blockSize) << endl;
      writer->writeFromAudioSampleBuffer(buffer, 0 /* offset into buffer */, params.blockSize);
    }

    instance->reset();

    return true;
  }
}

static int beginRequestHandler(struct mg_connection *conn) {
  enum BeginRequestHandlerReturnValues { HANDLED = 1, NOT_HANDLED = 0 };

  struct mg_request_info *info = mg_get_request_info(conn);
  String uri(info->uri);
  if (!uri.endsWithIgnoreCase(".json") && !uri.endsWithIgnoreCase(".wav")) {
    // DBG << "Not handling as audio request" << endl;
    return NOT_HANDLED;
  }

  // DBG << "Handling URI: " << uri << endl;

  var parsed;
  // First try to look in the query string
  String queryString = urldecode(info->query_string);
  // DBG << queryString << endl;
  parsed = JSON::parse(queryString);
  // Otherwise look in the POST data
  if (!parsed) {
    MemoryBlock postDataBlock;
    char postBuffer[1024];
    int didRead;
    while ((didRead = mg_read(conn, postBuffer, sizeof(postBuffer)))) {
      postDataBlock.append(postBuffer, didRead);
    }
    MemoryInputStream postStream(postDataBlock, false);
    parsed = JSON::parse(postStream);
  }

  DBG << "Request JSON: " << JSON::toString(parsed, true) << endl;

  PluginRequestParameters params(parsed);
  if (uri.endsWithIgnoreCase(".json")) {
    params.listParameters = true;
  }

  MemoryBlock block;
  MemoryOutputStream ostream(block, false);

  // DBG << "Rendering plugin request" << endl;
  int64 startTime = Time::currentTimeMillis();
  bool result = handlePluginRequest(params, ostream);
  if (!result) {
    DBG << "-> Unable to handle plugin request!" << endl;
    mg_printf(conn, "HTTP/1.0 500 ERROR\r\n\r\n");
    return HANDLED;
  }
  DBG << "-> Rendered plugin request in " << (Time::currentTimeMillis() - startTime) << "ms" << endl;
  
  // Note: MemoryOutputStream::getDataSize() is the actual number of bytes written.
  // Do not use MemoryBlock::getSize() since this reports the memory allocated (but not initialized!)
  mg_printf(conn, "HTTP/1.0 200 OK\r\n"
            "Content-Length: %d\r\n"
            "Content-Type: %s\r\n"
            "\r\n",
            (int)ostream.getDataSize(), params.getContentType());
  mg_write(conn, ostream.getData(), ostream.getDataSize());

  return HANDLED;
}

int main (int argc, char *argv[]) {
  Logger::setCurrentLogger(&DEBUG_LOGGER);

  #if 1 // Always load one instance to the pool to keep the plugin in cache
  // Consider testing PLUGIN_POOL_SIZE
  {
    DBG << "Loading plugin..." << endl;
    int poolSize = PLUGIN_POOL_SIZE;
    if (poolSize < 1) poolSize = 1;
    for (int numPlugs = 0; numPlugs < poolSize; ++numPlugs) {
      pluginPool.add(new ThreadSafePlugin(createSynthInstance()));
    }
  }
  #endif

  // Test: fire a request manually
  /*
  {
    DBG << "Firing test request" << endl;
    MemoryBlock testBlock;
    MemoryOutputStream ostream(testBlock, false);
    handlePluginRequest(PluginRequestParameters(), ostream);
    File testFile(resolveRelativePath("tmp/output2.wav"));
    testFile.replaceWithData(testBlock.getData(), testBlock.getSize());
    DBG << "Done firing test request" << endl;
  }
  */

  struct mg_context *ctx;
  const char *options[] = {
    "document_root", "public",
    "listening_ports", "8080",
    NULL
  };
  struct mg_callbacks callbacks;

  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = beginRequestHandler;
  ctx = mg_start(&callbacks, NULL, options);
  DBG << "Server started! Press ENTER to exit." << endl;
  getchar();  // Wait until user hits "enter"
  DBG << "Shutting down server threads" << endl;
  mg_stop(ctx);
  DBG << "Exiting" << endl;
  return 0;
}
