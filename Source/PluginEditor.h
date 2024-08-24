/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knob.h"

//==============================================================================
/**
*/
class PhaserAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PhaserAudioProcessorEditor (PhaserAudioProcessor&);
    ~PhaserAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Component sizes
    const int sliderWidth = 150;
    const int sliderHeight = 150;
    const int textBoxWidth = sliderWidth;
    const int textBoxHeight = 14;

    const int labelHeight = 16;
    const int labelWidth = sliderWidth;
    
    const int mWindowMarginWidth = 20;
    const int mWindowMarginHeight = 20;
    const int mSpaceBetweenW = 20;
    const int spaceBetweenH = 20;
    const int mTitleHeight = 40;

    KnobLookAndFeel knobLookAndFeel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PhaserAudioProcessor& audioProcessor;

    // Labels
    juce::Label nrStagesLabel;
    juce::Label spreadLabel;
    juce::Label centerLabel;
    juce::Label feedbackLabel;
    juce::Label invertPolarityLabel;
    juce::Label depthLabel;
    juce::Label lfoFreqLabel;
    juce::Label lfoDepthLabel;
    juce::Label stereoLabel;
    juce::Label gainLabel;

    // Sliders
    juce::Slider nrStagesSlider;
    juce::Slider spreadSlider;
    juce::Slider centerSlider;
    juce::Slider feedbackSlider;
    juce::Slider depthSlider;
    juce::Slider lfoFreqSlider;
    juce::Slider lfoDepthSlider;
    juce::Slider stereoSlider;
    juce::Slider gainSlider;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> nrStagesAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> spreadAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centerAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> invertPolarityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfoDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stereoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    // Buttons
    juce::ToggleButton invertPolarityButton;
    juce::ToggleButton bypassButton;

    // Groups
    juce::GroupComponent filtersGroup;
    juce::GroupComponent lfoGroup;
    juce::GroupComponent outputGroup;

    void initGUI();
    void initRotarySlider(const std::string& name, juce::Label& label, juce::Slider& slider, float start, float end, float interval, float defaultValue);
    void initToggleButton(const std::string& name, juce::Label& label, juce::ToggleButton& button, const std::string& id);
    void initGroup(const std::string& name, juce::GroupComponent& group);
    
    juce::FlexBox createFlexBox(juce::Label& label, juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserAudioProcessorEditor)
};
