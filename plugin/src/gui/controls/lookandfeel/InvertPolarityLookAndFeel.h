#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class InvertPolarityLookAndFeel : public juce::LookAndFeel_V4 {

  void drawToggleButton(juce::Graphics& g,
                        juce::ToggleButton& toggleButton,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override {
    juce::Path button;
    auto bounds = toggleButton.getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6;
    auto r = bounds.withSizeKeepingCentre(size, size).toFloat();
    size -= 9;

    button.startNewSubPath(r.getX(), r.getY() + r.getHeight());
    button.lineTo(r.getX() + r.getWidth(), r.getY());

    juce::PathStrokeType pst(2.0f, juce::PathStrokeType::curved);
    auto colour = toggleButton.getToggleState() ? juce::Colours::black : Colours::thirdaryColour;

    g.setColour(colour);
    g.strokePath(button, pst);
    g.drawEllipse(r, 2);
  }

};
}