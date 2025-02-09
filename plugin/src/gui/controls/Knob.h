#pragma once

#include "Label.h"
#include "lookandfeel/KnobLookAndFeel.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
class Knob : public juce::Component {
public:
  Knob(const std::string& name, const float start, const float end,
       const float interval, const float defaultValue) : label(name)
  {
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&knobLookAndFeel);

    slider.setRange(start, end, interval);

    slider.setPopupDisplayEnabled(false, false, this);
    slider.setValue(defaultValue);
    slider.setDoubleClickReturnValue(true, defaultValue);
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
  }

  void resized() override {
    auto area = getLocalBounds().toFloat();

    auto controlArea = area.removeFromTop(area.proportionOfHeight(0.8f));
    auto labelArea = area;

    float sliderSize = juce::jmin(controlArea.getWidth(), controlArea.getHeight());
    int sliderX = (controlArea.getWidth() - sliderSize) / 2;
    int sliderY = 0;
    slider.setBounds(sliderX, sliderY, sliderSize, sliderSize);

    label.setBounds(labelArea.getX(), labelArea.getY(), labelArea.getWidth(), labelArea.getHeight());
  }

  juce::Slider slider;
private:
  Label label;
  KnobLookAndFeel knobLookAndFeel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};
}
