#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class ComboBoxKnob : public juce::Component {
public:
  ComboBoxKnob(const juce::StringArray& options) {
    for (int i = 0; i < options.size(); ++i) {
      comboBox.addItem(options[i], i + 1);
    }

    comboBox.setColour(juce::ComboBox::textColourId, juce::Colours::black);
    comboBox.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentWhite);
    comboBox.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentWhite);
    comboBox.setLookAndFeel(&transparentComboBoxLookAndFeel);
    comboBox.setColour(juce::ComboBox::arrowColourId, juce::Colours::black);
    addAndMakeVisible(comboBox);
  }

  void resized() override {
    auto area = getLocalBounds();

    float comboBoxHeight = area.getHeight() * 0.3f;
    float comboBoxWidth = area.getWidth();

    int comboBoxX = (area.getWidth() - comboBoxWidth) / 2;
    int comboBoxY = (area.getHeight() - comboBoxHeight) / 2; // Center it properly

    comboBox.setBounds(comboBoxX, comboBoxY, comboBoxWidth, comboBoxHeight);
  }


  juce::ComboBox comboBox;
private:
  TransparentComboBoxLookAndFeel transparentComboBoxLookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBoxKnob)
};
}