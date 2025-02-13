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

            auto bounds = getLocalBounds();
            auto width = bounds.getWidth();
            auto height = bounds.getHeight();

            auto frequencies = phaser.getStageFrequencies();
            int nrOfStages = (int)*parameters.getRawParameterValue(ParamIDs::nrStages);

            auto range = juce::NormalisableRange<float>(
                                                        ParamRange::centerStart, ParamRange::centerEnd,
                                                        ParamRange::centerInterval , ParamRange::centerSkew);

            g.setColour(juce::Colours::grey);

            float x20Hz = width * range.convertTo0to1(20.0f) * 0.9f + width * 0.05f;
            float x20kHz = width * range.convertTo0to1(20000.0f) * 0.9f + width * 0.05f;

            float y = height / 2.0f; // Centered vertically

            g.drawLine(x20Hz, y-20, x20Hz, y+20, 1.0f);
            g.drawLine(x20kHz, y-20, x20kHz, y+20, 1.0f);

            g.setFont(12.0f);

            g.drawText("20 Hz", (int)x20Hz - 25, (int)y + 20, 50, 20, juce::Justification::centred);
            g.drawText("20 kHz", (int)x20kHz - 25, (int)y + 20, 50, 20, juce::Justification::centred);

            for (int i = 0; i < nrOfStages; i++)
            {
                auto freq = frequencies[0][i];
                DBG("freq = " << freq);
                float normFreq = range.convertTo0to1(freq);

                DBG("normFreq = " << normFreq);
                float x = width * normFreq * 0.9f + width * 0.05f;

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
