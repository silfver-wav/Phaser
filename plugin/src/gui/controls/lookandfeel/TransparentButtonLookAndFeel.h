#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class TransparentButtonLookAndFeel : public juce::LookAndFeel_V4 {

  juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override
  {
      return withDefaultMetrics (juce::FontOptions { juce::jmin (16.0f, (float) buttonHeight * 0.6f) });
  }

  void drawButtonBackground(juce::Graphics &g, juce::Button &button,
                            const juce::Colour &backgroundColour,
                            bool isMouseOverButton, bool isButtonDown) override {
    // Transparent background
  }

  void drawButtonText(juce::Graphics &g, juce::TextButton &button,
                      bool isMouseOverButton, bool isButtonDown) override {
    g.setColour(juce::Colours::white);
    juce:: Font font (getTextButtonFont (button, button.getHeight()));
    g.setFont (font);
    g.drawText(button.getButtonText(), button.getLocalBounds(),
               juce::Justification::centred, true);
  }

};
}