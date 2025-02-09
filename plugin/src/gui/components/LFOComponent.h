#pragma once

#include "../../utils/Utils.h"
#include "../controls/Group.h"
#include "../controls/Knob.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>

#include "../../utils/Params.h"

namespace Gui
{
    class LFOComponent : public juce::Component
    {
    public:
        LFOComponent(juce::AudioProcessorValueTreeState& parameters)
            : parameters(parameters),
              group("LFO"),
              lfoFreq("Frequency", ParamRange::lfoFreqStart, ParamRange::lfoFreqEnd, ParamRange::lfoFreqInterval,
                      ParamRange::lfoFreqDefault),
              lfoDepth("Depth", ParamRange::lfoDepthStart, ParamRange::lfoDepthEnd, ParamRange::lfoDepthInterval,
                       ParamRange::lfoDepthDefault),
              stereo("Stereo", ParamRange::stereoStart, ParamRange::stereoEnd, ParamRange::stereoInterval,
                     ParamRange::stereoDefault)
        {
            addAndMakeVisible(group);
            addAndMakeVisible(lfoFreq);
            addAndMakeVisible(lfoDepth);
            addAndMakeVisible(stereo);

            lfoFreqAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::lfoFreq, lfoFreq.slider);
            lfoDepthAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::lfoDepth, lfoDepth.slider);
            stereoAttachment =
                std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    parameters, ParamIDs::stereo, stereo.slider);


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

            int topPadding = 5;
            area = area.withTrimmedTop(topPadding);

            auto componentWidth = area.proportionOfWidth(0.33f);

            lfoFreq.setBounds(area.removeFromLeft(componentWidth));
            lfoDepth.setBounds(area.removeFromLeft(componentWidth));
            stereo.setBounds(area.removeFromLeft(componentWidth));
        }

    private:
        juce::AudioProcessorValueTreeState& parameters;
        Group group;

        Knob lfoFreq, lfoDepth, stereo;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            lfoFreqAttachment, lfoDepthAttachment, stereoAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOComponent)
    };
}
