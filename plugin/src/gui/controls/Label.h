#pragma once


#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class Label : public juce::Component {
public:
  Label(const std::string& name) {
    label.setText(name, juce::dontSendNotification);
    label.setColour(juce::Label::textColourId, juce::Colours::black);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds();
    label.setBounds(0, 0, area.getWidth(), area.getHeight());
  }
private:
  juce::Label label;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Label)
};
}