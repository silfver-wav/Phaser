#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class PowerButtonLookAndFeel : public juce::LookAndFeel_V4 {

  void drawToggleButton(juce::Graphics& g,
                        juce::ToggleButton& toggleButton,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override {
    juce::Path powerButton;
    auto bounds = toggleButton.getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6;
    auto r = bounds.withSizeKeepingCentre(size, size).toFloat();
    auto ang = 30.f;
    size -= 9;

    powerButton.addCentredArc(r.getCentreX(),
                                r.getCentreY(),
                                size * 0.5f,
                                size * 0.5f,
                                0.f,
                                juce::degreesToRadians(ang),
                                juce::degreesToRadians(360.f - ang),
                                true);

    powerButton.startNewSubPath(r.getCentreX(), r.getY());
    powerButton.lineTo(r.getCentre());

    juce::PathStrokeType pst(1.2f, juce::PathStrokeType::curved);

    auto colour = toggleButton.getToggleState() ? toggleButton.findColour(juce::ToggleButton::tickColourId) : juce::Colours::dimgrey;

    g.setColour(colour);
    g.strokePath(powerButton, pst);
    g.drawEllipse(r, 1.5f);
  }

};
}