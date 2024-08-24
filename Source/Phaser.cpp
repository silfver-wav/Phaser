/*
  ==============================================================================

    Phaser.cpp
    Created: 14 Aug 2024 5:37:59pm
    Author:  Linus

  ==============================================================================
*/

#include "Phaser.h"

Phaser::Phaser(juce::AudioProcessorValueTreeState& params) : parameters(params)
{
    parameters.addParameterListener(ParamIDs::lfoFreq, this);
    parameters.addParameterListener(ParamIDs::lfoDepth, this);
}

Phaser::~Phaser()
{
    parameters.removeParameterListener(ParamIDs::lfoFreq, this);
    parameters.removeParameterListener(ParamIDs::lfoDepth, this);
}

//==============================================================================
void Phaser::prepare(juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    numChannels = spec.numChannels;

    // Prepare components
    prepareDrySignal(spec);
    prepareAPFilters();
    prepareFeedback();
    prepareLFO(spec);
}

void Phaser::prepareDrySignal(const juce::dsp::ProcessSpec& spec)
{
    drySignal.setSize(numChannels, spec.maximumBlockSize);
}

void Phaser::prepareAPFilters()
{
    int nrOfStages = getNrStages();

    for (int i = 0; i < nrOfStages; i++) {
        float stageFrequency = getStageFrequency(nrOfStages, i);
        phaserStages[i].setFilterCoefficient(stageFrequency, sampleRate);
    }
}

void Phaser::prepareFeedback()
{
    feedback.resize(numChannels, 0.f);
}

void Phaser::prepareLFO(const juce::dsp::ProcessSpec& spec)
{
    lfo.initialise([](float x) { return std::sin(x); }, 128);
    lfo.prepare(spec);
    lfo.setFrequency(lfoFrequency);
}

//==============================================================================
void Phaser::process(juce::AudioBuffer<float>& buffer)
{
    if (getBypass())
        return;

    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        drySignal.copyFrom(channel, 0, buffer.getReadPointer(channel), buffer.getNumSamples());
        processAllPassFilters(buffer, channel);
    }

    processDepth(buffer);

    buffer.applyGain(getGain());
}


void Phaser::processAllPassFilters(juce::AudioBuffer<float>& buffer, int channel)
{
    const float* inputSamples = buffer.getReadPointer(channel);
    float* outputSamples = buffer.getWritePointer(channel);

    float amountOfFeedback = getFeedback();
    float phaseOffset = (channel == 1) ? juce::MathConstants<float>::halfPi * getAmountOfStereo() : 0.0f;

    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        float feedbackInput = feedback[channel] * amountOfFeedback * (getInvertPolarity() ? -1 : 1);
        float inputSample = inputSamples[sampleIndex] + feedbackInput;

        float lfoValue = lfo.processSample(0.f + phaseOffset);
        processSampleThroughFilters(inputSample, lfoValue, channel);
        
        outputSamples[sampleIndex] = inputSample;
        feedback[channel] = inputSample;
    }
}

void Phaser::processSampleThroughFilters(float& sample, float lfoValue, int channel)
{
    int nrOfStages = getNrStages();

    for (int i = 0; i < nrOfStages; i++)
    {
        float freq = getStageFrequency(nrOfStages, i) + lfoValue * (maxFreqLFO - minFreqLFO) * lfoDepth;
        float modulatedFrequency = juce::jlimit(minFreq, maxFreq, freq);
        phaserStages[i].setFilterCoefficient(modulatedFrequency, sampleRate);
        sample = phaserStages[i].process(sample, channel);
    }
}

void Phaser::processDepth(juce::AudioBuffer<float>& buffer)
{
    float gain = 1 - getDepth();
    buffer.applyGain(getDepth());

    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        buffer.addFromWithRamp(channel, 0, drySignal.getReadPointer(channel), drySignal.getNumSamples(), gain, gain);
    }
}
//==============================================================================

void Phaser::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == ParamIDs::lfoFreq)
    {
        lfo.setFrequency(newValue);
    }
    else if (parameterID == ParamIDs::lfoDepth)
    {
        lfoDepth = newValue;
    }
}

//==============================================================================
float Phaser::getStageFrequency(int nrOfStages, int index)
{
    float centerFrequency = getCenter();
    float ratio = 1.5f;
    float spacing = 4.0f * getSpread();

    // Return the center frequency if spread is zero or there is only one stage
    if (spacing == 0 || nrOfStages == 1)
    {
        return centerFrequency;
    }

    int halfStages = nrOfStages / 2;
    int centerIndex1 = halfStages - 1;
    int centerIndex2 = nrOfStages % 2 == 0 ? halfStages : halfStages;

    if (index < centerIndex1) // Left of center
    { 
        int leftIndex = centerIndex1 - index;
        float freq = centerFrequency / std::pow(ratio, leftIndex / spacing);
        return juce::jmax(minFreq, freq); // Makes sure freq is not less than min frequency
    }
    else if (index > centerIndex2) // Right of center
    {
        int rightIndex = index - centerIndex2;
        float freq = centerFrequency * std::pow(ratio, rightIndex / spacing);
        return juce::jmin(maxFreq, freq); // Makes sure freq is not more than max frequency
    }
    else if (nrOfStages % 2 == 0 && (index == centerIndex1 || index == centerIndex2)) // Even number of stages
    {
        float offset = 0.5f;

        float freq = index == centerIndex1
            ? centerFrequency / std::pow(ratio, offset / spacing)
            : centerFrequency * std::pow(ratio, offset / spacing);
    
        return juce::jlimit(minFreq, maxFreq, freq); // Makes sure freq is in range
    }

    return centerFrequency;
}
//==============================================================================

int Phaser::getNrStages()
{
    return *parameters.getRawParameterValue("nr.stages");
}

float Phaser::getFeedback()
{
    return *parameters.getRawParameterValue(ParamIDs::feedback);
}

bool Phaser::getInvertPolarity()
{
    return *parameters.getRawParameterValue(ParamIDs::invertPolarity);
}

float Phaser::getDepth()
{
    return *parameters.getRawParameterValue(ParamIDs::depth);
}

bool Phaser::getBypass()
{
    return *parameters.getRawParameterValue(ParamIDs::bypass);
}

float Phaser::getAmountOfStereo()
{
    return *parameters.getRawParameterValue(ParamIDs::stereo);
}

float Phaser::getCenter()
{
    return *parameters.getRawParameterValue(ParamIDs::center);
}

float Phaser::getSpread()
{
    return *parameters.getRawParameterValue(ParamIDs::spread);
}

float Phaser::getGain()
{
    return *parameters.getRawParameterValue(ParamIDs::gain);
}
//==============================================================================