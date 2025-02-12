#pragma once

#include "lookandfeel/TransparentButtonLookAndFeel.h"
#include "lookandfeel/TransparentComboBoxLookAndFeel.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class PresetPanel : public juce::Component,
                    juce::Button::Listener,
                    juce::ComboBox::Listener {
public:
  PresetPanel(Service::PresetManger &presetManger) : presetManger(presetManger) {
    configureButton(saveButton, "Save");
    configureButton(deleteButton, "Delete");
    configureButton(previousPresetButton, "<");
    configureButton(nextPresetButton, ">");

    presetList.setTextWhenNothingSelected("No Preset Selected");
    presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(presetList);
    presetList.addListener(this);
    presetList.setLookAndFeel(&transparentComboBoxLookAndFeel);
    updatePresetList();
  }

  ~PresetPanel() {
    saveButton.removeListener(this);
    deleteButton.removeListener(this);
    previousPresetButton.removeListener(this);
    nextPresetButton.removeListener(this);
    presetList.removeListener(this);
  }

  void resized() override {
    const auto container = getLocalBounds();
    auto bounds = container;
    bounds.removeFromTop(proportionOfHeight(0.2f));

    saveButton.setBounds(
        bounds.removeFromLeft(container.proportionOfWidth(0.15f)));
    previousPresetButton.setBounds(
        bounds.removeFromLeft(container.proportionOfWidth(0.1f)));
    presetList.setBounds(
        bounds.removeFromLeft(container.proportionOfWidth(0.5f)));
    nextPresetButton.setBounds(
        bounds.removeFromLeft(container.proportionOfWidth(0.1f)));
    deleteButton.setBounds(bounds);
  }

private:
  void buttonClicked(juce::Button *button) override {
    if (button == &saveButton) {
      fileChooser = std::make_unique<juce::FileChooser>(
          "Please enter the name of the preset you want to save",
          Service::PresetManger::defaultDirectory,
          "*." + Service::PresetManger::extension);

      fileChooser->launchAsync(juce::FileBrowserComponent::saveMode,
                               [&](const juce::FileChooser &chooser) {
                                 const auto resultFile = chooser.getResult();
                                 presetManger.savePreset(
                                     resultFile.getFileNameWithoutExtension());
                                 updatePresetList();
                               });
    } else if (button == &previousPresetButton) {
      int currIndex = presetManger.loadPreviousPreset();
      presetList.setSelectedItemIndex(currIndex,
                                juce::dontSendNotification);
    } else if (button == &nextPresetButton) {
      int currIndex =  presetManger.loadNextPreset();
      presetList.setSelectedItemIndex(currIndex,
                          juce::dontSendNotification);
    } else if (button == &deleteButton) {
      // Make confirmation
      presetManger.deletePreset(presetManger.getCurrentPreset());
      updatePresetList();
    }
  }

  void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override {
    if (comboBoxThatHasChanged == &presetList) {
      presetManger.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
    }
  }

  void configureButton(juce::Button &button, const juce::String &buttonText) {
    button.setButtonText(buttonText);
    button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    button.setLookAndFeel(&transparentButtonLookAndFeel);
    addAndMakeVisible(button);
    button.addListener(this);
  }

  void updatePresetList() {
    presetList.clear();
    const auto allPresets = presetManger.getAllPresets();
    const auto currentPreset = presetManger.getCurrentPreset();
    presetList.addItemList(allPresets, 1);
    presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset),
                                    juce::dontSendNotification);
  }

  juce::TextButton saveButton, deleteButton, previousPresetButton,
      nextPresetButton;
  juce::ComboBox presetList;
  Service::PresetManger presetManger;
  std::unique_ptr<juce::FileChooser> fileChooser;

  TransparentComboBoxLookAndFeel transparentComboBoxLookAndFeel;
  TransparentButtonLookAndFeel transparentButtonLookAndFeel;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};
}
