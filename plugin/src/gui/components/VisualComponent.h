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

            // Logarithmic scaling for the X-axis with padding
            auto logToX = [width, this](float freq)
            {
                float logFreq = std::log10(freq);
                float logMin = std::log10(minFreq);
                float logMax = std::log10(maxFreq);

                float reducedWidth = width * (1.0f - 2.0f * paddingPercentage);
                float x = width * paddingPercentage + reducedWidth * (logFreq - logMin) / (logMax - logMin);
                return x;
            };

            g.setColour(juce::Colours::grey);
            float freqMarkers[] = {20.0f, 50.0f, 100.0f, 200.0f, 500.0f, 1000.0f, 2000.0f, 5000.0f, 10000.0f, 20000.0f};

            float y = height / 2.0f;

            // Frequency markers
            for (auto freq : freqMarkers)
            {
                float x = logToX(freq);
                g.drawLine(x, y - 20, x, y + 20, 1.0f);
                g.setFont(12.0f);
                if (freq >= 1000.0f)
                    g.drawText(juce::String(freq / 1000.0f) + " kHz", (int)x - 25, (int)y + 20, 50, 20,
                               juce::Justification::centred);
                else
                    g.drawText(juce::String(freq) + " Hz", (int)x - 25, (int)y + 20, 50, 20,
                               juce::Justification::centred);
            }

            // Frequency points for each stage
            for (int i = 0; i < nrOfStages; i++)
            {
                auto freqLeft = frequencies[0][i];
                auto freqRight = frequencies[1][i];

                float xLeft = logToX(freqLeft);
                float xRight = logToX(freqRight);

                g.setColour(juce::Colours::white);
                g.fillEllipse(xLeft - 5, y - 5, 10, 10); // Left channel
                g.fillEllipse(xRight - 5, y - 5, 10, 10); // Right channel
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

        float minFreq = 20.0f, maxFreq = 20000.0f, paddingPercentage = 0.075f;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VisualComponent)
    };
}
