
#include <iostream>
#include "JuceHeader.h"

#include <csignal>
#define EMBED_BREAKPOINT raise(SIGINT)

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
  AudioPluginFormatManager formatManager;
  formatManager.addDefaultFormats();

  ScopedPointer<AudioPluginInstance> instance(getSynthInstance(formatManager));
  if (!instance) return 1;
  cout << instance->getName() << endl;

  return 0;
}
