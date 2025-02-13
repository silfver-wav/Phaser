#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace DSP
{
    class Phaser;
}

namespace Gui {
    class VisualComponent :
      public juce::Component
    {
    public:
        VisualComponent(juce::AudioProcessorValueTreeState& parameters, DSP::Phaser& phaser)
        : parameters(parameters), phaser(phaser)
        {
            setBufferedToImage(true);
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::black);  // Background

            auto bounds = getLocalBounds();
            auto width = bounds.getWidth();
            auto height = bounds.getHeight();

            // Define frequency range
            float minFreq = 20.0f;
            float maxFreq = 20000.0f;

            // Get stage frequencies

            auto frequencies = phaser.getStageFrequencies();

            for (int i = 0; i < (int)*parameters.getRawParameterValue(ParamIDs::nrStages); i++)
            {
                auto freq = frequencies[i];
                // Map log frequency to screen X coordinate
                float x = juce::jmap(std::log10(freq),
                                     std::log10(minFreq),
                                     std::log10(maxFreq),
                                     0.0f, (float)width);

                float y = height / 2.0f;  // Centered

                g.setColour(juce::Colours::white);
                g.fillEllipse(x - 5, y - 5, 10, 10);  // Draw dot
            }

        }

    private:
        juce::AudioProcessorValueTreeState &parameters;
        DSP::Phaser& phaser;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VisualComponent)
      };
}
