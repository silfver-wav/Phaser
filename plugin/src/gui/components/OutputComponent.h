#pragma once

#include "../../utils/Utils.h"
#include "../controls/Group.h"
#include "../controls/Knob.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include "../../utils/Params.h"

namespace Gui {
    class OutputComponent : public juce::Component {
    public:
        OutputComponent(juce::AudioProcessorValueTreeState &parameters)
            : parameters(parameters),
              group("Output"),
              mix("Mix", ParamRange::mixStart, ParamRange::mixEnd, ParamRange::mixInterval, ParamRange::mixDefault),
              gain("Gain", ParamRange::gainStart, ParamRange::gainEnd, ParamRange::gainInterval, ParamRange::gainDefault)
        {
            addAndMakeVisible(group);
            addAndMakeVisible(mix);
            addAndMakeVisible(gain);

            mixAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, ParamIDs::mix, mix.slider);
            gainAttachment =
              std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, ParamIDs::gain, gain.slider);

            setBufferedToImage(true);
        }

        void paint(juce::Graphics &g) override {
            g.setColour(Colours::primaryColour);
            g.fillRoundedRectangle(getLocalBounds().toFloat(), Layout::radius);
        }

        void resized() override {
            auto area = getLocalBounds();
            group.setBounds(-10, 1, area.getWidth(), area.getHeight());

            int topPadding = 5;
            area = area.withTrimmedTop(topPadding);

            auto componentHeight = area.proportionOfHeight(0.5f);
            auto componentArea = area;

            mix.setBounds(componentArea.removeFromTop(componentHeight).reduced(5));
            gain.setBounds(componentArea.removeFromTop(componentHeight).reduced(5));
        }

    private:
        juce::AudioProcessorValueTreeState &parameters;
        Group group;

        Knob mix;
        Knob gain;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            mixAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
          gainAttachment;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputComponent)
      };
}