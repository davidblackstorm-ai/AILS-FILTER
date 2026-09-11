#include "PluginEditor.h"

AILSFilterHPEditor::AILSFilterHPEditor (AILSFilterHPProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (360, 220);
    setResizable (false, false);

    setupKnob (cutoff, cutoffLabel, "CUTOFF", " Hz");
    cutoff.setNumDecimalPlacesToDisplay (0);

    setupKnob (resonance, resonanceLabel, "RESONANCE", "");
    resonance.setNumDecimalPlacesToDisplay (2);

    slope.addItemList ({ "12 dB/oct", "24 dB/oct", "36 dB/oct", "48 dB/oct" }, 1);
    slope.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (slope);

    slopeLabel.setText ("SLOPE", juce::dontSendNotification);
    slopeLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (slopeLabel);

    addAndMakeVisible (bypass);

    cutoffAttachment = std::make_unique<SliderAttachment> (processor.apvts, "cutoff", cutoff);
    resonanceAttachment = std::make_unique<SliderAttachment> (processor.apvts, "q", resonance);
    slopeAttachment = std::make_unique<ComboAttachment> (processor.apvts, "slope", slope);
    bypassAttachment = std::make_unique<ButtonAttachment> (processor.apvts, "bypass", bypass);
}

void AILSFilterHPEditor::setupKnob (juce::Slider& s, juce::Label& label, const juce::String& text, const juce::String& suffix)
{
    s.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 82, 20);
    s.setTextValueSuffix (suffix);
    addAndMakeVisible (s);

    label.setText (text, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (label);
}

void AILSFilterHPEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (14, 16, 20));

    auto header = getLocalBounds().removeFromTop (42);
    g.setColour (juce::Colour::fromRGB (30, 33, 39));
    g.fillRect (header);

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (18.0f, juce::Font::bold));
    g.drawText ("AILS FILTER HP", 16, 0, 210, 42, juce::Justification::centredLeft);

    g.setColour (juce::Colour::fromRGB (125, 185, 255));
    g.setFont (juce::FontOptions (11.0f));
    g.drawText ("VST3 • HIGH PASS", 218, 0, 125, 42, juce::Justification::centredRight);
}

void AILSFilterHPEditor::resized()
{
    cutoffLabel.setBounds (18, 54, 120, 18);
    cutoff.setBounds (18, 72, 120, 120);

    resonanceLabel.setBounds (138, 54, 120, 18);
    resonance.setBounds (138, 72, 120, 120);

    slopeLabel.setBounds (270, 58, 76, 18);
    slope.setBounds (268, 80, 80, 28);
    bypass.setBounds (272, 132, 80, 26);
}
