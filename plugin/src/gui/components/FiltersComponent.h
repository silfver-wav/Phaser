#pragma once

#include "../../utils/Utils.h"
#include "../controls/Button.h"
#include "../controls/Group.h"
#include "../controls/Knob.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>

#include "../../utils/Params.h"

namespace Gui
{
    class FiltersComponent : public juce::Component
    {
    public:
        FiltersComponent(juce::AudioProcessorValueTreeState& parameters)
            : parameters(parameters),
              group("Filters"),
              nrOfStages("Nr Of Stages", ParamRange::nrStagesStart, ParamRange::nrStagesEnd,
                         ParamRange::nrStagesInterval, ParamRange::nrStagesDefault),
              spread("Spread", ParamRange::spreadStart, ParamRange::spreadEnd, ParamRange::spreadInterval,
                     ParamRange::spreadDefault),
              center("Center", ParamRange::centerStart, ParamRange::centerEnd, ParamRange::centerInterval,
                     ParamRange::centerDefault),
              feedback("Feedback", ParamRange::feedbackStart, ParamRange::feedbackEnd,
                       ParamRange::feedbackInterval, ParamRange::feedbackDefault),
              invPolarity("Invert Polarity", ParamIDs::invertPolarity)
        {
            addAndMakeVisible(group);
            addAndMakeVisible(nrOfStages);
            addAndMakeVisible(spread);
            addAndMakeVisible(center);
            addAndMakeVisible(feedback);
            addAndMakeVisible(invPolarity);

            nrStagesAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::nrStages, nrOfStages.slider);
            spreadAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::spread, spread.slider);
            centerAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::center, center.slider);
            feedbackAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::feedback, feedback.slider);
            invPolarityAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                    parameters, ParamIDs::invertPolarity, invPolarity.button);

            setBufferedToImage(true);
        }

        void paint(juce::Graphics& g) override
        {
            g.setColour(Colours::primaryColour);
            g.fillRoundedRectangle(getLocalBounds().toFloat(), Layout::radius);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            group.setBounds(-10, 1, area.getWidth(), area.getHeight());

            int topPadding = 10;
            area = area.withTrimmedTop(topPadding);

            auto componentWidth = area.proportionOfWidth(0.20f);

            nrOfStages.setBounds(area.removeFromLeft(componentWidth));
            spread.setBounds(area.removeFromLeft(componentWidth));
            center.setBounds(area.removeFromLeft(componentWidth));
            feedback.setBounds(area.removeFromLeft(componentWidth));
            invPolarity.setBounds(area.removeFromLeft(componentWidth));
        }

    private:
        juce::AudioProcessorValueTreeState& parameters;
        Group group;

        Knob nrOfStages, spread, center, feedback;
        Button invPolarity;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            nrStagesAttachment, spreadAttachment, centerAttachment, feedbackAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        invPolarityAttachment;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FiltersComponent)
    };
}
