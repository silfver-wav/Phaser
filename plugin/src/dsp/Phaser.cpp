#include "Phaser.h"

namespace DSP
{
    Phaser::Phaser(juce::AudioProcessorValueTreeState& params) : parameters(params)
    {
        parameters.addParameterListener(ParamIDs::lfoFreq, this);
        parameters.addParameterListener(ParamIDs::lfoRate, this);
        parameters.addParameterListener(ParamIDs::lfoSyncMode, this);
        parameters.addParameterListener(ParamIDs::mix, this);
        parameters.addParameterListener(ParamIDs::waveForm, this);
        dryWet.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    }

    Phaser::~Phaser()
    {
        parameters.removeParameterListener(ParamIDs::lfoFreq, this);
        parameters.removeParameterListener(ParamIDs::lfoRate, this);
        parameters.removeParameterListener(ParamIDs::lfoSyncMode, this);
        parameters.removeParameterListener(ParamIDs::mix, this);
        parameters.removeParameterListener(ParamIDs::waveForm, this);
    }

    //==============================================================================
    void Phaser::prepare(juce::dsp::ProcessSpec& spec)
    {
        jassert(spec.sampleRate > 0);
        jassert(spec.numChannels > 0);

        sampleRate = static_cast<float>(spec.sampleRate);
        numChannels = spec.numChannels;

        lfoValue.reset(sampleRate, 0.05f);

        dryWet.prepare(spec);
        feedback.resize(spec.numChannels);

        auto specDown = spec;
        specDown.sampleRate /= (double)maxUpdateCounter;
        specDown.maximumBlockSize = specDown.maximumBlockSize / (juce::uint32)maxUpdateCounter + 1;

        osc.prepare(specDown);
        prepareAPFilters();

        updateFreq();
        updateOsc();
        updateDryWet();
        reset();
    }

    void Phaser::reset()
    {
        std::ranges::fill(feedback, static_cast<float>(0));

        osc.reset();
        dryWet.reset();
    }

    void Phaser::prepareAPFilters()
    {
        const int nrOfStages = getNrStages();

        for (int i = 0; i < nrOfStages; i++)
        {
            float stageFrequency = getStageFrequency(nrOfStages, i, getCenter());
            phaserStages[i].setFilterCoefficient(stageFrequency, sampleRate);
        }
    }

    //==============================================================================
    void Phaser::process(const juce::dsp::ProcessContextReplacing<float>& context)
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();

        jassert(inputBlock.getNumChannels() == numChannels);
        jassert(inputBlock.getNumSamples() == numSamples);

        dryWet.pushDrySamples(inputBlock);

        int counter = updateCounter;

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples = inputBlock.getChannelPointer(channel);
            auto* outputSamples = outputBlock.getChannelPointer(channel);
            float phaseOffset = (channel == 1) ? juce::MathConstants<float>::halfPi * getAmountOfStereo() : 0.0f;

            for (size_t i = 0; i < numSamples; ++i)
            {
                if (counter == 0)
                    lfoValue.setTargetValue(osc.processSample(0.f + phaseOffset));

                auto input = inputSamples[i];

                auto feedbackSign = getInvertPolarity() ? -1 : 1;
                float inputWithFeedback = input + static_cast<float>(feedbackSign) * feedback[channel];;

                processSampleThroughFilters(inputWithFeedback, lfoValue.getNextValue() * getLFODepth(),
                                            static_cast<int>(channel));

                outputSamples[i] = inputWithFeedback;
                feedback[channel] = inputWithFeedback * getFeedback();

                counter++;
                if (counter >= maxUpdateCounter)
                    counter = 0;
            }
        }

        dryWet.mixWetSamples(outputBlock);
        updateCounter = (updateCounter + (int)numSamples) % maxUpdateCounter;
    }


    void Phaser::processSampleThroughFilters(float& sample, float lfoValue, int channel)
    {
        int nrOfStages = getNrStages();
        auto center = getCenter();

        for (int i = 0; i < nrOfStages; i++)
        {
            float stageFreq = getStageFrequency(nrOfStages, i, center);
            float freq = juce::jlimit(minFreq, maxFreq, stageFreq * (1 + lfoValue));

            if (channel == 0)
                stageFrequenciesLeft[i] = freq;
            else
                stageFrequenciesRight[i] = freq;

            phaserStages[i].setFilterCoefficient(freq, sampleRate);
            sample = phaserStages[i].process(sample, channel);
        }
    }

    void Phaser::parameterChanged(const juce::String& parameterID,
                                  float newValue)
    {
        if (parameterID == ParamIDs::lfoFreq || parameterID == ParamIDs::lfoRate ||
            parameterID == ParamIDs::lfoSyncMode)
        {
            updateFreq();
        }
        else if (parameterID == ParamIDs::mix)
        {
            updateDryWet();
        }
        else if (parameterID == ParamIDs::waveForm)
        {
            updateOsc();
        }
    }

    inline void Phaser::updateDryWet() { dryWet.setWetMixProportion(getMix() / 2.0f); }

    void Phaser::updateFreq()
    {
        float freq;
        switch (getLFOSyncMode())
        {
        case 0:
            freq = getLFOFreq();
            break;
        case 1:
            freq = getSubdivisionFreq(getLFOSyncRate());
            break;
        default:
            freq = getLFOFreq();
            break;
        }

        osc.setFrequency(freq);
    }

    void Phaser::updateOsc()
    {
        std::function<float(float)> oscFunction;
        switch (getWaveForm())
        {
        case 0:
            oscFunction = [](float x) { return std::sin(x); };
            break;
        case 1:
            oscFunction = [](float x)
            {
                return (2 / juce::MathConstants<float>::pi) * std::asin(std::sin(x));
            };
            break;
        case 2:
            oscFunction = [](float x) { return x < 0.0f ? -1.0f : 1.0f; };
            break;
        case 3:
            oscFunction = [](float x) { return x / juce::MathConstants<float>::pi; };
            break;
        default:
            oscFunction = [](float x) { return std::sin(x); };
            break;
        }

        osc.initialise(oscFunction);
    }

    //==============================================================================
    float Phaser::getStageFrequency(int nrOfStages, int index, float centerFreq)
    {
        float spread = getSpread();
        float spacing = std::pow(1.75f, spread * 2.0f);

        // Return the center frequency if spread is zero or there is only one stage
        if (spread == 0 || nrOfStages == 1)
        {
            return centerFreq; // No spread means all frequencies are at the center
        }

        int centerIndex = nrOfStages / 2;

        if (nrOfStages % 2 == 0)
        {
            if (centerIndex <= index) // Right of center
            {
                int rightIndex = index - centerIndex + 1;
                float freq = centerFreq * std::pow(spacing, static_cast<float>(rightIndex));
                return juce::jmin(maxFreq, freq);
            }
        }
        else
        {
            if (index > centerIndex) // Right of center
            {
                int rightIndex = index - centerIndex;
                float freq = centerFreq * std::pow(spacing, static_cast<float>(rightIndex));
                return juce::jmin(maxFreq, freq);
            }
        }

        if (index < centerIndex) // Left of center
        {
            int leftIndex = centerIndex - index;
            float freq = centerFreq / std::pow(spacing, static_cast<float>(leftIndex));
            return juce::jmax(minFreq, freq);
        }

        return centerFreq;
    }

    float Phaser::getSubdivisionFreq(const int choice) const
    {
        if (BPM <= 0.0)
            return 0.0f;

        double beatDuration = 60.0 / BPM; // Quarter note (1/4) in seconds

        double multiplier = 1.0;
        switch (choice)
        {
        case 0: // Whole note (1/1)
            multiplier = 4.0;
            break;
        case 1: // Half note (1/2)
            multiplier = 2.0;
            break;
        case 2: // Quarter note (1/4)
            multiplier = 1.0;
            break;
        case 3: // Dotted-quarter note (1/4.)
            multiplier = 1.5;
            break;
        case 4: // Triplet-quarter note (1/4T)
            multiplier = 2.0 / 3.0;
            break;
        case 5: // Eighth note (1/8)
            multiplier = 0.5;
            break;
        case 6: // Dotted-eighth note (1/8.)
            multiplier = 0.75;
            break;
        case 7: // Triplet-eighth note (1/8T)
            multiplier = 1.0 / 3.0;
            break;
        case 8: // Sixteenth note (1/16)
            multiplier = 0.25;
            break;
        case 9: // Dotted-sixteenth note (1/16.)
            multiplier = 0.375;
            break;
        case 10: // Triplet-sixteenth note (1/16T)
            multiplier = 1.0 / 6.0;
            break;
        default:
            return 0.0f; // Invalid choice
        }

        double freq = 1.0 / (beatDuration * multiplier);

        return static_cast<float>(freq);
    }

    void Phaser::setBPM(double bpm)
    {
        if (std::abs(bpm - BPM) > 0.01f)
        {
            BPM = bpm;
            updateFreq();
        }
    }
}
