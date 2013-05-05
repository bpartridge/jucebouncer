
#include <iostream>
#include "JuceHeader.h"

#include <csignal>
#define EMBED_BREAKPOINT raise(SIGINT)

using namespace std;
using namespace juce;

String resolveRelativePath(String relativePath) {
  return File::getCurrentWorkingDirectory().getChildFile(relativePath).getFullPathName();
}

void scanPlugins(KnownPluginList &list) {
  AudioPluginFormatManager formatManager;
  formatManager.addDefaultFormats();
  // AudioPluginFormat *format = formatManager.getFormat(1);
  // if (!format) {
  //   cout << "No format" << endl;
  //   return;
  // }
  // cout << "format: " << format->getName() << endl;

  // FileSearchPath searchPath(resolveRelativePath("plugins"));
  // bool searchRecursively(true);
  // const File deadMansPedalFile(resolveRelativePath("tmp/deadMansPedalFile"));
  // PluginDirectoryScanner scanner(list, *format, searchPath, searchRecursively, deadMansPedalFile);
  // bool result;
  // do {
  //   result = scanner.scanNextFile(true);
  //   cout << "scanned, result: " << result << endl;
  // } while(result);

  OwnedArray<PluginDescription> typesFound;
  String filename = resolveRelativePath("plugins/FreeAlpha.vst");

  StringArray filenames(filename);
  list.scanAndAddDragAndDroppedFiles(formatManager, filenames, typesFound);
}

int main (int argc, char *argv[]) {
  // cout << "Message thread: " << MessageManager::getInstance()->isThisTheMessageThread() << endl;

  KnownPluginList pluginList;
  scanPlugins(pluginList);
  cout << "Known types: " << pluginList.getNumTypes() << endl;
  if (!pluginList.getNumTypes()) return 1;

  return 0;
}
