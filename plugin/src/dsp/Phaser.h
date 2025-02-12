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
        void process(const juce::dsp::ProcessContextReplacing<float>& context);
        void setBPM(double bpm);
        void reset();
    private:
        juce::AudioProcessorValueTreeState& parameters;
        juce::dsp::Oscillator<float> osc;
        std::array<FirstOrderAllPass, 23> phaserStages;
        juce::dsp::DryWetMixer<float> dryWet;
        std::vector<float> feedback{2};

        float lfoFrequency = 0.1f, minFreq = 20.f, maxFreq = 20000.0f;
        juce::LinearSmoothedValue<float> lfoValue { ParamRange::lfoFreqDefault }, center { ParamRange::centerDefault };

        float sampleRate = 44100.f;
        int numChannels = 0;
        double BPM = 0;

        int updateCounter = 0;
        static constexpr int maxUpdateCounter = 4;

        void prepareAPFilters();
        void processSampleThroughFilters(float& sample, float lfoValue, int channel);

        void parameterChanged(const juce::String& parameterID, float newValue) override;
        float getStageFrequency(int nrOfStages, int index);
        void updateOsc();
        void updateFreq();
        [[nodiscard]] float getSubdivisionFreq(int choice) const;
        void updateDryWet();

        [[nodiscard]] int getNrStages() const
        {
            return *parameters.getRawParameterValue("nr.stages");
        }

        [[nodiscard]] float getSpread() const
        {
            return *parameters.getRawParameterValue(ParamIDs::spread);
        }

        [[nodiscard]] float getFeedback() const
        {
            return *parameters.getRawParameterValue(ParamIDs::feedback);
        }

        [[nodiscard]] bool getInvertPolarity() const
        {
            return *parameters.getRawParameterValue(ParamIDs::invertPolarity);
        }

        [[nodiscard]] float getLFOFreq() const {
            return *parameters.getRawParameterValue(ParamIDs::lfoFreq);
        }

        [[nodiscard]] int getLFOSyncRate() const {
            return static_cast<int>(*parameters.getRawParameterValue(ParamIDs::lfoRate));
        }

        [[nodiscard]] int getLFOSyncMode() const {
            return static_cast<int>(*parameters.getRawParameterValue(ParamIDs::lfoSyncMode));
        }

        [[nodiscard]] float getLFODepth() const {
            return *parameters.getRawParameterValue(ParamIDs::lfoDepth);
        }

        [[nodiscard]] int getWaveForm() const {
            return static_cast<int>(*parameters.getRawParameterValue(ParamIDs::waveForm));
        }

        [[nodiscard]] float getAmountOfStereo() const
        {
            return *parameters.getRawParameterValue(ParamIDs::stereo);
        }

        [[nodiscard]] float getMix() const
        {
            return *parameters.getRawParameterValue(ParamIDs::mix);
        }

        [[nodiscard]] float getGain() const
        {
            float gainDB = *parameters.getRawParameterValue(ParamIDs::gain);
            return juce::Decibels::decibelsToGain(gainDB);
        }
        //==============================================================================
    };
}
