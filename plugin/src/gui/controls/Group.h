#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class Group : public juce::Component {
public:
  Group(const std::string& name) {
    group.setText(name);
    // group.setColour(juce::GroupComponent::outlineColourId, juce::Colours::white);
    group.setColour(juce::GroupComponent::outlineColourId, juce::Colours::transparentWhite);
    group.setColour(juce::GroupComponent::textColourId, juce::Colours::black);
    addAndMakeVisible(group);
  }

  void resized() override {
    auto area = getLocalBounds();
    group.setBounds(area);
  }

  juce::GroupComponent group;
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Group)
};
}