#pragma once

#include "PluginProcessor.h"
#include "gui/components/FiltersComponent.h"
#include "gui/components/LFOComponent.h"
#include "gui/components/OutputComponent.h"
#include "gui/components/TopComponent.h"


class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
  ~AudioPluginAudioProcessorEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
  AudioPluginAudioProcessor &processorRef;
  Gui::TopComponent topComponent;
  Gui::OutputComponent outputComponent;
  Gui::FiltersComponent filterComponent;
  Gui::LFOComponent lfoComponent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
