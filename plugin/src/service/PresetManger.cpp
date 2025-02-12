#include "PresetManger.h"

#include "../utils/ProjectInfo.h"

namespace Service {
const juce::File PresetManger::defaultDirectory{
    juce::File::getSpecialLocation(
        juce::File::SpecialLocationType::commonDocumentsDirectory)
        .getChildFile(ProjectInfo::companyName)
        .getChildFile(ProjectInfo::projectName)};
const juce::String PresetManger::extension{"preset"};
const juce::String PresetManger::presetNameProperty{"presetName"};

PresetManger::PresetManger(juce::AudioProcessorValueTreeState &parameters)
    : parameters(parameters) {
  if (!defaultDirectory.exists()) {
    const auto result = defaultDirectory.createDirectory();
    if (result.failed()) {
      DBG("Could not create preset directory: " + result.getErrorMessage());
      jassertfalse;
    }
  }

  parameters.state.addListener(this);
  currentPreset.referTo(parameters.state.getPropertyAsValue(presetNameProperty, nullptr));
}

void PresetManger::savePreset(const juce::String &presetName) {
  if (presetName.isEmpty())
    return;

  currentPreset.setValue(presetName);
  const auto xml = parameters.copyState().createXml();
  const auto presetFile =
      defaultDirectory.getChildFile(presetName + "." + extension);
  if (!xml->writeTo(presetFile)) {
    DBG("Could not save preset: " + presetFile.getFullPathName());
    jassertfalse;
  }
}

void PresetManger::deletePreset(const juce::String &presetName) {
  if (presetName.isEmpty())
    return;

  const auto presetFile =
      defaultDirectory.getChildFile(presetName + "." + extension);
  if (!presetFile.existsAsFile()) {
    DBG("Preset file: " + presetFile.getFullPathName() + " does not exist");
    jassertfalse;
    return;
  }
  if (presetFile.deleteFile()) {
    DBG("Preset file: " + presetFile.getFullPathName() +
        " could not be deleted");
    jassertfalse;
    return;
  }

  currentPreset.setValue("");
}

void PresetManger::loadPreset(const juce::String &presetName) {
  if (presetName.isEmpty())
    return;

  const auto presetFile =
      defaultDirectory.getChildFile(presetName + "." + extension);
  if (!presetFile.existsAsFile()) {
    DBG("Preset file: " + presetFile.getFullPathName() + " does not exist");
    jassertfalse;
    return;
  }
  juce::XmlDocument presetXml{presetFile};
  const auto valueTreeToLoad =
      juce::ValueTree::fromXml(*presetXml.getDocumentElement());

  parameters.replaceState(valueTreeToLoad);
  currentPreset.setValue(presetName);
}

int PresetManger::loadNextPreset() {
  const auto allPresets = getAllPresets();
  if (allPresets.isEmpty())
    return -1;

  const auto currentIndex = allPresets.indexOf(currentPreset.toString());
  const auto nextIndex =
      currentIndex + 1 > (allPresets.size() - 1) ? 0 : currentIndex + 1;
  loadPreset(allPresets.getReference(nextIndex));

  return nextIndex;
}

int PresetManger::loadPreviousPreset() {
  const auto allPresets = getAllPresets();
  if (allPresets.isEmpty())
    return -1;

  const auto currentIndex = allPresets.indexOf(currentPreset.toString());
  const auto prevIndex =
      currentIndex - 1 < 0 ? (allPresets.size() - 1) : currentIndex - 1;
  loadPreset(allPresets.getReference(prevIndex));

  return prevIndex;
}

juce::StringArray PresetManger::getAllPresets() const {
  juce::StringArray presets;
  const auto fileArray = defaultDirectory.findChildFiles(
      juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
  for (const auto &file : fileArray) {
    presets.add(file.getFileNameWithoutExtension());
  }
  return presets;
}

juce::String PresetManger::getCurrentPreset() const { return currentPreset.toString(); }

void PresetManger::valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) {
  currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
}

} // namespace Service