#pragma once

#include "../../utils/Params.h"
#include "../controls/LogoButton.h"
#include "../controls/PresetPanel.h"
#include "../controls/lookandfeel/PowerButtonLookAndFeel.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>

namespace Gui {
class HeaderComponent : public juce::Component {
public:
  HeaderComponent(juce::AudioProcessorValueTreeState& parameters, Service::PresetManger presetManger) : presetPanel(presetManger) {
    addAndMakeVisible(presetPanel);
    powerButton.setLookAndFeel(&powerButtonLookAndFeel);
    powerButton.setClickingTogglesState(true);
    addAndMakeVisible(powerButton);

    powerButtonAttachment =
    std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, ParamIDs::power, powerButton);

    addAndMakeVisible(logoButton);
  }

  void paint (juce::Graphics& g) override {
    g.setColour(juce::Colours::black);
  }

  void resized() override {
    auto area = getLocalBounds();

    area.removeFromTop(area.getHeight() * 0.15f);

    auto powerWidth = area.proportionOfWidth(0.09f);
    auto rightButtonWidth = area.proportionOfWidth(0.09f);
    auto gapWidth = area.proportionOfWidth(0.09f);
    auto presetWidth = area.getWidth() - powerWidth - rightButtonWidth - (2 * gapWidth);

    auto powerButtonArea = area.removeFromLeft(powerWidth);
    int buttonWidth = powerButtonArea.getWidth() - 7, buttonHeight = powerButtonArea.getHeight() - 7;
    int buttonX = (powerButtonArea.getWidth() - buttonWidth) / 2;
    int buttonY = (powerButtonArea.getHeight() - buttonHeight) / 2;
    powerButton.setBounds(buttonX, buttonY + 2, buttonWidth, buttonHeight);

    area.removeFromLeft(gapWidth);

    auto presetArea = area.removeFromLeft(presetWidth);
    int presetPanelWidth = presetArea.getWidth();
    int presetPanelHeight = presetArea.getHeight();
    int presetPanelY = (presetArea.getHeight() - presetPanelHeight) / 2;
    presetPanel.setBounds(area.getWidth(), presetPanelY, presetPanelWidth, presetPanelHeight);
    area.removeFromLeft(gapWidth);
    logoButton.setBounds(area.removeFromLeft(rightButtonWidth));
  }


private:
  juce::ToggleButton powerButton;
  PresetPanel presetPanel;
  LogoButton logoButton;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> powerButtonAttachment;


  PowerButtonLookAndFeel powerButtonLookAndFeel;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};
}