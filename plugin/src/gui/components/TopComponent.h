#pragma once

#include "../../service/PresetManger.h"
#include "../../utils/Params.h"
#include "HeaderComponent.h"
#include "VisualComponent.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>

#include <utility>

namespace Gui {
    class TopComponent : public juce::Component {
    public:
        TopComponent(juce::AudioProcessorValueTreeState& parameters, Service::PresetManger presetManger, DSP::Phaser& phaser)
        :
        headerComponent(parameters, std::move(presetManger)), visualComponent(parameters, phaser)
        {
            addAndMakeVisible (headerComponent);
            addAndMakeVisible(visualComponent);
            setBufferedToImage(true);
        }

        void paint(juce::Graphics &g) override {
            g.setColour(juce::Colours::black);
            g.fillRoundedRectangle(getLocalBounds().toFloat(), Layout::radius);
        }

        void resized() override {
            auto area = getLocalBounds();
            headerComponent.setBounds(area.removeFromTop(area.proportionOfHeight(0.16f)));
            visualComponent.setBounds(area);
        }

    private:
        HeaderComponent headerComponent;
        VisualComponent visualComponent;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopComponent)
      };
}