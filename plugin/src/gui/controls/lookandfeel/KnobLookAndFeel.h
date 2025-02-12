#pragma once

#include "../../../utils/Utils.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class KnobLookAndFeel : public juce::LookAndFeel_V4 {
public:
  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider &slider) override {

    const auto radius =
        static_cast<float>(juce::jmin(width / 2, height / 2)) - 5.0f;
    const auto centreX = static_cast<float>(x + width) * 0.5f;
    const auto centreY = static_cast<float>(y) + radius + 12.f;

    // Current angle of the slider
    auto angle =
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Draw path of the slider background (in lighter background colour)
    drawArc(g, centreX, centreY, radius, rotaryStartAngle, rotaryEndAngle,
            Colours::thirdaryColour);

    // Draw path of slider foreground (in black)
    drawArc(g, centreX, centreY, radius, rotaryStartAngle, angle,
            juce::Colours::black);

    // Draw the value in the center of the knob
    g.setColour(juce::Colours::black);
    g.setFont(radius * 0.40f);

    juce::String valueText = slider.getTextFromValue(slider.getValue());
    g.drawSingleLineText(valueText, static_cast<int>(centreX),
                         static_cast<int>(centreY) + 5.0f,
                         juce::Justification::centred);
  }

  static void drawArc(juce::Graphics &g, auto centreX, auto centreY,
                      auto radius, float rotaryStartAngle, float rotaryEndAngle,
                      const juce::Colour colour) {
    juce::Path arc;
    arc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle,
                      rotaryEndAngle, true);

    g.setColour(colour);
    g.strokePath(arc, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved,
                                           juce::PathStrokeType::rounded));
  }
};
} // namespace Gui