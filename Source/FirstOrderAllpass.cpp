/*
  ==============================================================================

    FirstOrderAllpass.cpp
    Created: 14 Aug 2024 5:40:20pm
    Author:  Linus

  ==============================================================================
*/

#include "FirstOrderAllpass.h"

float FirstOrderAllPass::process(const float& inputSample, int channel)
{
    jassert(prevInput.size() > 0 || prevOutput.size() > 0);

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