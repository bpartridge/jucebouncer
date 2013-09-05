
#include <iostream>
#include "JuceHeader.h"

using namespace std;
using namespace juce;

static std::ostream &DBG = cerr;

AudioPluginInstance *createSynthInstance(const char *filename = nullptr) {
  AudioPluginFormatManager pluginManager;
  pluginManager.addDefaultFormats();

  PluginDescription desc;

  if (filename) desc.fileOrIdentifier = filename;
  else desc.fileOrIdentifier = "plugins/FreeAlpha.vst";

  desc.uid = 0;

  String errorMessage;
  AudioPluginInstance *instance = pluginManager.createPluginInstance(desc, errorMessage);
  
  if (!instance) {
    DBG << "Error creating plugin instance: " << errorMessage << endl;
    exit(1);
  }

  // Force initialization on the main thread. This preparation will redone with proper values later.
  instance->prepareToPlay(44100, 512);

  DBG << "Number of presets: " << instance->getNumPrograms() << endl;

  return instance;
}

NamedValueSet getPluginParameters(AudioPluginInstance *instance) {
  NamedValueSet set;
  if (!instance) return set;
  for (int i = 0, n = instance->getNumParameters(); i < n; ++i) {
    Identifier name(instance->getParameterName(i));
    float currentValue = instance->getParameter(i);
    set.set(name, currentValue);
  }
  return set;
}

String setToJSON(const NamedValueSet &set) {
  DynamicObject *obj = new DynamicObject(); // will be freed automatically
  obj->getProperties() = set;
  return JSON::toString(var(obj), true);
}

void sampleProgram(AudioPluginInstance *instance, int program = -1) {

  if (program >= 0) {
    DBG << "Changing program to: " << program << endl;
    instance->setCurrentProgram(program);
    // miniTERA MAY not perform a program change unless getCurrentProgram() is called after setting?
    // And FreeAlpha doesn't even listen to this :(
    // DBG << "Prog. after change:  " << instance->getCurrentProgram() << endl;
    if (instance->getCurrentProgram() != program) DBG << "WARNING: UNABLE TO CHANGE PRESET!!!!" << endl;
  }

  String str = setToJSON(getPluginParameters(instance));

  DBG << "Params: " << str.substring(0, 70) << "..." << endl;
  DBG << "Hash of Params JSON: " << DefaultHashFunctions::generateHash(str, 1000) << endl;

  DBG << endl;
}

int main (int argc, char *argv[]) {
  AudioPluginInstance *instance;

  instance = createSynthInstance("/Library/Audio/Plug-Ins/VST/Sylenth1Demo.vst");
  DBG << endl;
  sampleProgram(instance);
  sampleProgram(instance, 0);
  sampleProgram(instance, 3);
  sampleProgram(instance, 0);
  DBG << "======\n" << endl;

  instance = createSynthInstance("/Library/Audio/Plug-Ins/VST/miniTERA.vst");
  DBG << endl;
  sampleProgram(instance);
  sampleProgram(instance, 0);
  sampleProgram(instance, 3);
  sampleProgram(instance, 0);
  DBG << "======\n" << endl;

  instance = createSynthInstance();
  DBG << endl;
  sampleProgram(instance);
  sampleProgram(instance, 0);
  sampleProgram(instance, 3);
  sampleProgram(instance, 0);
}
