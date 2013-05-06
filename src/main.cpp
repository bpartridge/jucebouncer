
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

String resolveRelativePath(String relativePath) {
  return File::getCurrentWorkingDirectory().getChildFile(relativePath).getFullPathName();
}

// The caller is responsible for deleting the object that is returned
AudioPluginInstance *getSynthInstance(AudioPluginFormatManager &formatManager) {
  PluginDescription desc;
  desc.fileOrIdentifier = resolveRelativePath("plugins/FreeAlpha.vst");
  desc.uid = 0;

  String errorMessage;
  return formatManager.createPluginInstance(desc, errorMessage);
}

int main (int argc, char *argv[]) {
  Logger::setCurrentLogger(&DEBUG_LOGGER);

  DEBUG << "Creating plugin manager" << endl;
  AudioPluginFormatManager pluginManager;
  pluginManager.addDefaultFormats();

  DEBUG << "Creating audio plugin instance" << endl;
  ScopedPointer<AudioPluginInstance> instance(getSynthInstance(pluginManager));
  if (!instance) return 1;
  DEBUG << instance->getName() << endl;

  DEBUG << "Creating audio format manager" << endl;
  AudioFormatManager formatManager;
  formatManager.registerBasicFormats();
  OptionalScopedPointer<AudioFormat> wavFormat(formatManager.findFormatForFileExtension("wav"), false);
  
  DEBUG << "Preparing plugin" << endl;
  int sampleRate = 44100, blockSize = 2056, bitDepth = 16, nChannels = 2, nSeconds = 3;
  instance->setNonRealtime(true);
  instance->prepareToPlay(sampleRate, blockSize);

  DEBUG << "Preparing output" << endl;
  File oFile(resolveRelativePath("tmp/output.wav"));
  oFile.deleteFile();
  // The writer will delete the output stream when it is deleted.
  // Note that passing a locally-scoped OutputStream to the writer creation will result in a double-free.
  OptionalScopedPointer<FileOutputStream> oStream(oFile.createOutputStream(), false);
  OptionalScopedPointer<AudioFormatWriter> writer(wavFormat->createWriterFor(oStream, sampleRate, nChannels, bitDepth, StringPairArray(), 0), true);

  DEBUG << "Preparing MIDI" << endl;
  int midiChannel = 1;
  float noteSeconds = 1;
  MidiBuffer midiBuffer;
  midiBuffer.addEvent(MidiMessage::noteOn(midiChannel, 60, 1.0f), 0);
  midiBuffer.addEvent(MidiMessage::noteOn(midiChannel, 67, 1.0f), 0);
  midiBuffer.addEvent(MidiMessage::noteOn(midiChannel, 76, 1.0f), 0);
  midiBuffer.addEvent(MidiMessage::allNotesOff(midiChannel), noteSeconds * sampleRate);

  AudioSampleBuffer buffer(nChannels, blockSize);
  int numBuffers = nSeconds * sampleRate / blockSize;
  for (int i = 0; i < numBuffers; ++i) {
    DEBUG << "Processing block " << i << "..." << flush;
    instance->processBlock(buffer, midiBuffer);
    DEBUG << " left RMS level " << buffer.getRMSLevel(0, 0, blockSize) << endl;
    writer->writeFromAudioSampleBuffer(buffer, 0 /* offset */, blockSize);
  }

  DEBUG << "Done!" << endl;

  return 0;
}
