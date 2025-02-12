
#include "FirstOrderAllpass.h"

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

namespace DSP
{
    float FirstOrderAllPass::process(const float& inputSample, int channel)
    {
        jassert(!prevInput.empty() || !prevOutput.empty());

        // y[n] = a*x[n] + x[n−1]−a*y[n−1] (a = filter coefficient)
        float outSample = coefficient * inputSample + prevInput[channel] - coefficient * prevOutput[channel];

        prevOutput[channel] = outSample;
        prevInput[channel] = inputSample;

        return outSample;
    }

    void FirstOrderAllPass::setFilterCoefficient(const float& cutoffFrequency, float sampleRate)
    {
        // fc = cutoffFrequency, fs = sampleRate
        // c = (tan(pi*fc/fs)-1) / (tan(pi*fc/fs)+1)
        float tanCal = std::tan((juce::MathConstants<float>::pi * cutoffFrequency) / sampleRate);
        float coefficient = (tanCal - 1) / (tanCal + 1);

        this->coefficient = juce::jlimit(-1.0f, 1.0f, coefficient);
    }
}
