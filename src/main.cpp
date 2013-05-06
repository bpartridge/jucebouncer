
#include <iostream>
#include "JuceHeader.h"

// #include <csignal>
// #define EMBED_BREAKPOINT raise(SIGINT)

using namespace std;
using namespace juce;

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
  std::ostream &DEBUG = cout;

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
  int sampleRate = 44100, blockSize = 2056, bitDepth = 16, nChannels = 2, nSeconds = 5;
  instance->setNonRealtime(true);
  instance->prepareToPlay(sampleRate, blockSize);

  DEBUG << "Preparing output" << endl;
  File oFile = File::getCurrentWorkingDirectory().getChildFile("tmp/output.wav");
  FileOutputStream oStream(oFile);
  OptionalScopedPointer<AudioFormatWriter> writer(wavFormat->createWriterFor(&oStream, sampleRate, nChannels, bitDepth, StringPairArray(), 0), false);

  DEBUG << "Preparing MIDI" << endl;
  MidiBuffer midiBuffer;
  // TODO

  AudioSampleBuffer buffer(nChannels, blockSize);
  int numBuffers = nSeconds * sampleRate / blockSize;
  for (int i = 0; i < numBuffers; ++i) {
    DEBUG << "Processing block " << i << endl;
    instance->processBlock(buffer, midiBuffer);
    writer->writeFromAudioSampleBuffer(buffer, 0 /* offset */, blockSize);
  }

  DEBUG << "Done!" << endl;

  return 0;
}
