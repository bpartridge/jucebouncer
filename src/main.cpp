
#include <iostream>
#include "JuceHeader.h"
#include "mongoose.h"
#include "NonDeletingOutputStream.h"
#include "urlutils.h"

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

String resolveRelativePath(String relativePath) {
  return File::getCurrentWorkingDirectory().getChildFile(relativePath).getFullPathName();
}

// The caller is responsible for deleting the object that is returned
AudioPluginInstance *createSynthInstance() {
  AudioPluginFormatManager pluginManager;
  pluginManager.addDefaultFormats();

  PluginDescription desc;
  desc.fileOrIdentifier = resolveRelativePath("plugins/FreeAlpha.vst");
  desc.uid = 0;

  String errorMessage;
  AudioPluginInstance *instance = pluginManager.createPluginInstance(desc, errorMessage);

  // Force initialization on the main thread. This preparation will redone with proper values later.
  instance->prepareToPlay(44100, 512);

  return instance;
}

struct AudioRequestParameters {
  bool listParameters;
  int sampleRate, blockSize, bitDepth;
  int nChannels, nSeconds;
  int midiChannel, midiPitch, midiVelocity;
  float noteSeconds;
  String formatName, contentType;
  NamedValueSet parameters;

  AudioRequestParameters(const var &params = var::null) {
    #define AUDIO_REQUEST_PARAMETERS_DEFAULT(name, default) \
      if (params[#name]) {name = params[#name];} else {name = default;}
    AUDIO_REQUEST_PARAMETERS_DEFAULT(listParameters, false);
    AUDIO_REQUEST_PARAMETERS_DEFAULT(sampleRate, 44100)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(blockSize, 2056)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(bitDepth, 16)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(nChannels, 2)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(nSeconds, 3)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(midiChannel, 1)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(midiPitch, 60)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(midiVelocity, 120)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(noteSeconds, 1.0f)

    setListParameters(params["listParameters"]);

    DynamicObject *paramDynObj = params["parameters"].getDynamicObject();
    if (paramDynObj) parameters = paramDynObj->getProperties();
  }

  void setListParameters(bool shouldList) {
    if (shouldList) {
      listParameters = true;
      formatName = "json";
      contentType = "application/json";
    }
    else {
      listParameters = false;
      formatName = "wav";
      contentType = "audio/vnw.wave";
    }
  }
};

bool handleAudioRequest(const AudioRequestParameters &params, OutputStream &ostream, 
                        ThreadSafePlugin *plugin = nullptr) {
  if (!plugin) {
    // Recurse with a plugin from the pool, locking on it.
    // Keep trying with a delay until a timeout occurs.
    const int TIMEOUT = 5000, WAIT = 200;
    int64 startTime = Time::currentTimeMillis();

    while (Time::currentTimeMillis() < startTime + TIMEOUT) {
      int i = 0;
      while (plugin = pluginPool[i++]) {
        const ScopedTryLock pluginTryLock(plugin->crit);
        if (pluginTryLock.isLocked()) {
          DBG << "Handling with plugin " << i << endl;
          return handleAudioRequest(params, ostream, plugin);
        }
      }
      DBG << "Trying again in " << WAIT << endl;
      Thread::sleep(WAIT);
    }

    // If we were unable to obtain a lock, return failure.
    DBG << "Timeout" << endl;
    return false;
  }
  else {
    // Re-acquire the lock just in case.
    const ScopedLock pluginLock(plugin->crit);
    AudioPluginInstance *instance = plugin->instance; // unmanaged, for simplicity

    if (params.listParameters) {
      DBG << "Rendering parameter list" << endl;
      var v(new DynamicObject());
      var paramNames;
      for (int i = 0, n = instance->getNumParameters(); i < n; ++i) {
        String paramName = instance->getParameterName(i);
        paramNames.insert(i, paramName);
        // float paramValue = instance->getParameter(i);
        // v.getDynamicObject().setProperty(Identifier(paramName), paramValue);
      }
      v.getDynamicObject()->setProperty(Identifier("parameterNames"), paramNames);
      JSON::writeToStream(ostream, v);
      return true;
    }
    
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    OptionalScopedPointer<AudioFormat> outputFormat(formatManager.findFormatForFileExtension(params.formatName), false);
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
    int numBuffers = params.nSeconds * params.sampleRate / params.blockSize;
    for (int i = 0; i < numBuffers; ++i) {
      // DBG << "Processing block " << i << "..." << flush;
      instance->processBlock(buffer, midiBuffer);
      // DBG << " left RMS level " << buffer.getRMSLevel(0, 0, params.blockSize) << endl;
      writer->writeFromAudioSampleBuffer(buffer, 0 /* offset into buffer */, params.blockSize);
    }

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

  DBG << "Handling as audio request: " << uri << endl;

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
    while (didRead = mg_read(conn, postBuffer, sizeof(postBuffer))) {
      postDataBlock.append(postBuffer, didRead);
    }
    MemoryInputStream postStream(postDataBlock, false);
    parsed = JSON::parse(postStream);
  }

  DBG << "Request JSON: " << JSON::toString(parsed, true) << endl;

  AudioRequestParameters params(parsed);
  if (uri.endsWithIgnoreCase(".json")) {
    params.setListParameters(true);
  }
  // DBG << "listParameters: " << params.listParameters << endl;

  // DBG << "Starting to handle audio request" << endl;
  MemoryBlock block;
  MemoryOutputStream ostream(block, false);
  int64 startTime = Time::currentTimeMillis();
  bool result = handleAudioRequest(params, ostream);
  if (!result) {
    DBG << "Unable to handle audio request!" << endl;
    mg_printf(conn, "HTTP/1.0 500 ERROR\r\n\r\n");
    return HANDLED;
  }
  DBG << "Rendered audio request in " << (Time::currentTimeMillis() - startTime) << "ms" << endl;

  // Note: MemoryOutputStream::getDataSize() is the actual number of bytes written.
  // Do not use MemoryBlock::getSize() since this reports the memory allocated (but not initialized!)
  mg_printf(conn, "HTTP/1.0 200 OK\r\n"
            "Content-Length: %d\r\n"
            "Content-Type: %s\r\n"
            "\r\n",
            (int)ostream.getDataSize(), params.contentType.toRawUTF8());
  mg_write(conn, ostream.getData(), ostream.getDataSize());

  return HANDLED;
}

int main (int argc, char *argv[]) {
  Logger::setCurrentLogger(&DEBUG_LOGGER);

  DBG << "Loading plugin..." << endl;
  for (int numPlugs = 0; numPlugs < 3; ++numPlugs) {
    pluginPool.add(new ThreadSafePlugin(createSynthInstance()));
  }

  // Test: fire a request manually
  /*
  {
    DBG << "Firing test request" << endl;
    MemoryBlock testBlock;
    MemoryOutputStream ostream(testBlock, false);
    handleAudioRequest(AudioRequestParameters(), ostream);
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
