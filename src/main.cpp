
#include <iostream>
#include "JuceHeader.h"

// #include <csignal>
// #define EMBED_BREAKPOINT raise(SIGINT)

using namespace std;
using namespace juce;

static std::ostream &DEBUG = cerr;
class DebugLogger : public Logger {
  void logMessage(const String &message) {
    DEBUG << message << endl;
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
  int sampleRate, blockSize, bitDepth;
  int nChannels, nSeconds;
  int midiChannel, midiPitch, midiVelocity;
  float noteSeconds;
  String formatName;

  AudioRequestParameters(const var &params = var::null) {
    #define AUDIO_REQUEST_PARAMETERS_DEFAULT(name, default) \
      if (params[#name]) {name = params[#name];} else {name = default;}
    AUDIO_REQUEST_PARAMETERS_DEFAULT(sampleRate, 44100)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(blockSize, 2056)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(bitDepth, 16)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(nChannels, 2)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(nSeconds, 3)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(midiChannel, 1)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(midiPitch, 60)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(midiVelocity, 120)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(noteSeconds, 1.0f)
    AUDIO_REQUEST_PARAMETERS_DEFAULT(formatName, "wav")
  }
};

bool handleAudioRequest(const AudioRequestParameters &params, MemoryBlock &memory, 
                        ThreadSafePlugin *plugin = nullptr) {
  if (!plugin) {
    // Recurse with a plugin from the pool, locking on it.
    int i = 0;
    while (plugin = pluginPool[i++]) {
      const ScopedTryLock pluginTryLock(plugin->crit);
      if (pluginTryLock.isLocked()) {
        return handleAudioRequest(params, memory, plugin);
      }
    }

    // We were unable to acquire the lock on this plugin instance.
    // TODO: try again until a timeout.
    return false;
  }
  else {
    // Re-acquire the lock just in case.
    const ScopedLock pluginLock(plugin->crit);
    AudioPluginInstance *instance = plugin->instance; // unmanaged, for simplicity
    
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    OptionalScopedPointer<AudioFormat> outputFormat(formatManager.findFormatForFileExtension(params.formatName), false);

    instance->setNonRealtime(true);
    instance->prepareToPlay(params.sampleRate, params.blockSize);
    instance->setNonRealtime(true);

    // The writer takes ownership of the output stream; the writer will delete it when the writer leaves scope.
    // Note that passing a locally-scoped OutputStream to the writer creation will result in a double-free.
    ScopedPointer<AudioFormatWriter> writer(outputFormat->createWriterFor(
      new MemoryOutputStream(memory, false /* don't append */), 
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
      // DEBUG << "Processing block " << i << "..." << flush;
      instance->processBlock(buffer, midiBuffer);
      // DEBUG << " left RMS level " << buffer.getRMSLevel(0, 0, params.blockSize) << endl;
      writer->writeFromAudioSampleBuffer(buffer, 0 /* offset into buffer */, params.blockSize);
    }

    return true;
  }
}

int main (int argc, char *argv[]) {
  Logger::setCurrentLogger(&DEBUG_LOGGER);

  DEBUG << "Loading plugin..." << endl;
  pluginPool.add(new ThreadSafePlugin(createSynthInstance()));

  // Test: fire a request manually
  DEBUG << "Firing test request" << endl;
  MemoryBlock testBlock;
  handleAudioRequest(AudioRequestParameters(), testBlock);
  File testFile(resolveRelativePath("tmp/output2.wav"));
  testFile.replaceWithData(testBlock.getData(), testBlock.getSize());

  return 0;
}
