#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class AILSFilterHPEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AILSFilterHPEditor (AILSFilterHPProcessor&);
    ~AILSFilterHPEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AILSFilterHPProcessor& processor;

    juce::Slider cutoff;
    juce::Slider resonance;
    juce::ComboBox slope;
    juce::ToggleButton bypass { "BYPASS" };

    juce::Label cutoffLabel;
    juce::Label resonanceLabel;
    juce::Label slopeLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> cutoffAttachment;
    std::unique_ptr<SliderAttachment> resonanceAttachment;
    std::unique_ptr<ComboAttachment> slopeAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;

    void setupKnob (juce::Slider& s, juce::Label& label, const juce::String& text, const juce::String& suffix);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AILSFilterHPEditor)
};
