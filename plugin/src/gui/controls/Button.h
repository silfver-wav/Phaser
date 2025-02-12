#pragma once

#include "Label.h"
#include "lookandfeel/InvertPolarityLookAndFeel.h"
#include "lookandfeel/ToggleButtonLookAndFeel.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class Button : public juce::Component {
public:
  Button(const std::string& name, auto id) : label(name) {
    addAndMakeVisible(button);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds();

    auto controlArea = area.removeFromTop(area.proportionOfHeight(0.8f));
    auto labelArea = area;

    int buttonSize = 25;
    int buttonX = (controlArea.getWidth() - buttonSize) / 2;
    int buttonY = (getLocalBounds().getHeight() - buttonSize) / 2;
    button.setBounds(buttonX, buttonY, buttonSize, buttonSize);

    label.setBounds(labelArea.getX(), labelArea.getY(), labelArea.getWidth(), labelArea.getHeight());
  }


  juce::ToggleButton button;
private:
  Label label;
  InvertPolarityLookAndFeel invertPolarityLookAndFeel;
  ToggleButtonLookAndFeel toggleButtonLookAndFeel;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
};
}