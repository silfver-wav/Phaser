#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../utils/Params.h"

namespace DSP
{
    class Phaser;
}

namespace Gui
{
    class VisualComponent :
        public juce::Component,
        public juce::Timer
    {
    public:
        VisualComponent(juce::AudioProcessorValueTreeState& parameters, DSP::Phaser& phaser)
            : parameters(parameters), phaser(phaser)
        {
            auto fps = 24.f;
            startTimer(static_cast<int>(1000.f / fps));

            setBufferedToImage(true);
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::black);

            auto bounds = getLocalBounds().reduced(2);
            auto width = bounds.getWidth();
            auto height = bounds.getHeight();

            auto frequencies = phaser.getStageFrequencies();
            int nrOfStages = (int)*parameters.getRawParameterValue(ParamIDs::nrStages);

            float skewFactor = 0.2f;

            for (int i = 0; i < nrOfStages; i++)
            {
                auto freq = frequencies[i];

                auto range = juce::NormalisableRange<float>(20.0f, 20000.0f, 0.0f, 0.2f);
                float normFreq = range.convertTo0to1(freq);

                DBG("freq = " << normFreq);

                // Map the normalized frequency (0 to 1) to the screen width.
                // The factor 0.9f compresses the range slightly for better alignment.
                // The offset (-0.251239f) corrects the skewed mapping so that 20 Hz starts near 0.
                float x = width * normFreq * 0.9f - 0.251239f;
                float y = height / 2.0f; // Centered vertically

                g.setColour(juce::Colours::white);
                g.fillEllipse(x - 5, y - 5, 10, 10);
            }
        }

        void timerCallback() override
        {
            // make a check if the parameters has changed to reduce repaint calls
            repaint();
        }

    private:
        juce::AudioProcessorValueTreeState& parameters;
        DSP::Phaser& phaser;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VisualComponent)
    };
}
