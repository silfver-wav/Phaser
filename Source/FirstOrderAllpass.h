/*
  ==============================================================================

    FirstOrderAllpass.h
    Created: 14 Aug 2024 5:40:20pm
    Author:  Linus
    
    IIR First Order Allpass filter
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FirstOrderAllPass
{
public:
    FirstOrderAllPass() = default;
    ~FirstOrderAllPass() = default;
    float process(const float& inputSample, int channel);
    void setFilterCoefficient(const float& cutoffFrequency, float sampleRate);

private:
    std::vector<float> prevInput = { 0.0f, 0.0f };
    std::vector<float> prevOutput = { 0.0f, 0.0f };
    float coefficient = 0.0f;
};
