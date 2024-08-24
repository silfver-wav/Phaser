/*
  ==============================================================================

    Phaser.h
    Created: 14 Aug 2024 5:37:59pm
    Author:  Linus

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FirstOrderAllpass.h"
#include "ParamsHelper.h"

class Phaser : public juce::AudioProcessorValueTreeState::Listener {
public:
    Phaser(juce::AudioProcessorValueTreeState& params);
    ~Phaser();
    void prepare(juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);

private:
    // Varibels

    // Buffers
    juce::AudioBuffer<float> drySignal;
    std::vector<float> feedback;

    // LFO related
    float lfoFrequency = 0.1f;
    float lfoDepth = 1.0f;

    float minFreqLFO = 20.f;
    float maxFreqLFO = 20000.0f;
    juce::dsp::Oscillator<float> lfo;

    // Allpass filters related
    std::array<FirstOrderAllPass, 23> phaserStages;

    int numChannels = 0;
    float sampleRate = 44100.f;
    float minFreq = 20.f;
    float maxFreq = 20000.0f;
    juce::AudioProcessorValueTreeState& parameters;

    // Functions
    //==============================================================================
    void prepareDrySignal(const juce::dsp::ProcessSpec& spec);
    void prepareAPFilters();
    void prepareFeedback();
    void prepareLFO(const juce::dsp::ProcessSpec& spec);
    //==============================================================================
  
    //==============================================================================
    void processAllPassFilters(juce::AudioBuffer<float>& buffer, int channel);
    void processSampleThroughFilters(float& sample, float lfoValue, int channel);
    void processDepth(juce::AudioBuffer<float>& buffer);
    //==============================================================================
    
    //==============================================================================
    void parameterChanged(const juce::String& parameterID, float newValue);
    float getStageFrequency(int nrOfStages, int index);
    //==============================================================================

    //==============================================================================
    int getNrStages();
    float getFeedback();
    bool getInvertPolarity();
    float getDepth();
    bool getBypass();
    float getAmountOfStereo();
    float getCenter();
    float getSpread();
    //==============================================================================
};
