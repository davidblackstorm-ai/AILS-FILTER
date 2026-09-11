#pragma once
#include <JuceHeader.h>

class AILSFilterHPProcessor final : public juce::AudioProcessor
{
public:
    AILSFilterHPProcessor();
    ~AILSFilterHPProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    using APVTS = juce::AudioProcessorValueTreeState;
    APVTS apvts;
    static APVTS::ParameterLayout createParameterLayout();

private:
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coeffs = juce::dsp::IIR::Coefficients<float>;

    static constexpr int maxStages = 4;
    Filter filters[2][maxStages];

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> cutoffSmoothed;
    double currentSampleRate = 44100.0;
    float lastQ = -1.0f;
    int lastStages = -1;
    float lastCutoffForCoeffs = -1.0f;

    void updateCoefficients (float cutoffHz, float q, int stages);
    static int slopeIndexToStages (int choiceIndex) { return juce::jlimit (1, maxStages, choiceIndex + 1); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AILSFilterHPProcessor)
};
