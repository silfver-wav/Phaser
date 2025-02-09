#pragma once

#include "FirstOrderAllpass.h"
#include "../utils/Params.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

namespace DSP
{
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
        void prepareAPFilters();
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
        int getNrStages() const
        {
            return *parameters.getRawParameterValue("nr.stages");
        }

        float getFeedback() const
        {
            return *parameters.getRawParameterValue(ParamIDs::feedback);
        }

        bool getInvertPolarity() const
        {
            return *parameters.getRawParameterValue(ParamIDs::invertPolarity);
        }

        float getDepth() const
        {
            return *parameters.getRawParameterValue(ParamIDs::depth);
        }

        float getAmountOfStereo() const
        {
            return *parameters.getRawParameterValue(ParamIDs::stereo);
        }

        float getCenter() const
        {
            return *parameters.getRawParameterValue(ParamIDs::center);
        }

        float getSpread() const
        {
            return *parameters.getRawParameterValue(ParamIDs::spread);
        }

        float getGain() const
        {
            float gainDB = *parameters.getRawParameterValue(ParamIDs::gain);
            return juce::Decibels::decibelsToGain(gainDB);
        }
        //==============================================================================
    };
}
