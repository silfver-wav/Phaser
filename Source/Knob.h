/*
  ==============================================================================

    Knob.h
    Created: 22 Aug 2024 6:59:38pm
    Author:  Linus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        // Radius of knob
        auto radius = juce::jmin(width / 2, height / 2) - 5.0f;
        // Centre point (centreX, centreY) of knob
        auto centreX = x + width * 0.5f;
        auto centreY = y + radius + 12.f;

        // Current angle of the slider
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Draw path of the slider background (in darker background colour)
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        juce::Colour myColour = juce::Colours::black;

        g.setColour(myColour);
        g.strokePath(backgroundArc, juce::PathStrokeType(3.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Draw path of slider foreground (in blue)
        juce::Path foregroundArc;
        foregroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
        g.setColour(juce::Colours::blue);
        g.strokePath(foregroundArc, juce::PathStrokeType(3.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Pointer
        juce::Path p;
        auto pointerLength = radius * 1.f;
        auto pointerThickness = 4.0f;

        p.addRectangle(-pointerThickness * 0.5f, -radius - 1.3f , pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // Set color pointer to black
        g.setColour(juce::Colours::black);

        // Draw the pointer
        g.fillPath(p);
    }


	// Slider textbox
	void drawLabel(juce::Graphics& g, juce::Label& label) override
	{
		g.setColour(juce::Colours::white);

		juce::String text = label.getText();
		int width = label.getWidth();
		int height = label.getHeight();
		g.setFont(juce::Font(static_cast<float>(height), juce::Font::plain));
		g.drawFittedText(text, 0, 0, width, height, juce::Justification::centred, 1);
	}
};