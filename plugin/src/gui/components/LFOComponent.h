#pragma once

#include "../../utils/Utils.h"
#include "../controls/Button.h"
#include "../controls/ComboBox.h"
#include "../controls/Group.h"
#include "../controls/Knob.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>

namespace Gui {
class LFOComponent : public juce::Component, juce::AudioProcessorValueTreeState::Listener {
public:
  LFOComponent(juce::AudioProcessorValueTreeState &parameters)
      : parameters(parameters),
        group("LFO"),
        lfoFreq("Frequency", ParamRange::lfoFreqStart, ParamRange::lfoFreqEnd, ParamRange::lfoFreqInterval, ParamRange::lfoFreqDefault),
        lfoSyncMode("Sync", ParamIDs::lfoSyncMode),
        lfoRate(ParamRange::lfoRates),
        lfoDepth("Depth", ParamRange::lfoDepthStart, ParamRange::lfoDepthEnd, ParamRange::lfoDepthInterval, ParamRange::lfoDepthDefault),
        waveform(ParamRange::waveformChoices),
        stereo("Stereo", ParamRange::stereoStart, ParamRange::stereoEnd, ParamRange::stereoInterval, ParamRange::stereoDefault)
  {
    addAndMakeVisible(group);
    addAndMakeVisible(lfoFreq);
    addAndMakeVisible(lfoRate);
    addAndMakeVisible(lfoSyncMode);
    addAndMakeVisible(lfoDepth);
    addAndMakeVisible(waveform);
    addAndMakeVisible(stereo);

    lfoFreqAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, ParamIDs::lfoFreq, lfoFreq.slider);
    lfoSyncModeAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, ParamIDs::lfoSyncMode, lfoSyncMode.button);
    lfoRateAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, ParamIDs::lfoRate, lfoRate.comboBox);
    lfoDepthAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, ParamIDs::lfoDepth, lfoDepth.slider);
    waveformAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, ParamIDs::waveForm, waveform.comboBox);
    stereoAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, ParamIDs::stereo, stereo.slider);

    parameters.addParameterListener(ParamIDs::lfoSyncMode, this);
    isSyncMode = static_cast<int>(*parameters.getRawParameterValue(ParamIDs::lfoSyncMode)) != 0;

    setBufferedToImage(true);
  }

  ~LFOComponent() override {
    parameters.removeParameterListener(ParamIDs::lfoSyncMode, this);
  }

  void paint(juce::Graphics &g) override {
    g.setColour(Colours::primaryColour);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), Layout::radius);
  }

  void resized() override {
    auto area = getLocalBounds();
    group.setBounds(-10, 1, area.getWidth(), area.getHeight());

    int topPadding = 5;
    area = area.withTrimmedTop(topPadding);

    auto componentWidthKnob = area.proportionOfWidth(0.22f); // 0.26f
    auto componentWidthButton = area.proportionOfWidth(0.17f); // 0.11f
    auto componentArea = area;

    if (isSyncMode) {
      lfoRate.setBounds(componentArea.removeFromLeft(componentWidthKnob));
      lfoFreq.setVisible(false);
      lfoRate.setVisible(true);
    } else {
      lfoFreq.setBounds(componentArea.removeFromLeft(componentWidthKnob));
      lfoFreq.setVisible(true);
      lfoRate.setVisible(false);
    }

    lfoSyncMode.setBounds(componentArea.removeFromLeft(componentWidthButton));
    lfoDepth.setBounds(componentArea.removeFromLeft(componentWidthKnob));
    waveform.setBounds(componentArea.removeFromLeft(componentWidthButton));
    stereo.setBounds(componentArea.removeFromLeft(componentWidthKnob));
  }


private:
  void parameterChanged(const juce::String& parameterID, float newValue) override {
    if (parameterID == ParamIDs::lfoSyncMode) {
      updateLFOMode();
    }
  }

  void updateLFOMode() {
    isSyncMode = static_cast<int>(*parameters.getRawParameterValue(ParamIDs::lfoSyncMode)) != 0;
    resized();  // Refresh the layout
  }

  juce::AudioProcessorValueTreeState &parameters;
  Group group;

  Knob lfoFreq;
  Button lfoSyncMode;
  ComboBoxKnob lfoRate;
  Knob lfoDepth;
  ComboBoxKnob waveform;
  Knob stereo;

  bool isSyncMode = false;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      lfoFreqAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
    lfoSyncModeAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    lfoRateAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      lfoDepthAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    waveformAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      stereoAttachment;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOComponent)
};
}