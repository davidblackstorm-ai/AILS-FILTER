#include "PluginProcessor.h"
#include "PluginEditor.h"

AILSFilterHPProcessor::AILSFilterHPProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

AILSFilterHPProcessor::APVTS::ParameterLayout AILSFilterHPProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;

    p.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "cutoff", 1 }, "Cutoff",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.25f), 30.0f,
        juce::AudioParameterFloatAttributes().withLabel ("Hz")));

    p.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "q", 1 }, "Resonance",
        juce::NormalisableRange<float> (0.5f, 2.0f, 0.001f, 0.6f), 0.70710678f));

    p.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "slope", 1 }, "Slope",
        juce::StringArray { "12 dB/oct", "24 dB/oct", "36 dB/oct", "48 dB/oct" }, 1));

    p.push_back (std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID { "bypass", 1 }, "Bypass", false));

    return { p.begin(), p.end() };
}

bool AILSFilterHPProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;
    return layouts.getMainInputChannelSet() == out;
}

void AILSFilterHPProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 1 };

    for (auto& channel : filters)
        for (auto& f : channel)
        {
            f.prepare (spec);
            f.reset();
        }

    cutoffSmoothed.reset (sampleRate, 0.025);
    cutoffSmoothed.setCurrentAndTargetValue (*apvts.getRawParameterValue ("cutoff"));
    lastQ = -1.0f;
    lastStages = -1;
    lastCutoffForCoeffs = -1.0f;
}

void AILSFilterHPProcessor::updateCoefficients (float cutoffHz, float q, int stages)
{
    const float nyquistSafe = static_cast<float> (currentSampleRate * 0.49);
    cutoffHz = juce::jlimit (20.0f, nyquistSafe, cutoffHz);
    q = juce::jlimit (0.5f, 2.0f, q);

    auto coeff = Coeffs::makeHighPass (currentSampleRate, cutoffHz, q);
    for (int ch = 0; ch < 2; ++ch)
        for (int stage = 0; stage < stages; ++stage)
            filters[ch][stage].coefficients = coeff;
}

void AILSFilterHPProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const bool bypass = apvts.getRawParameterValue ("bypass")->load() > 0.5f;
    if (bypass)
        return;

    const float cutoffTarget = apvts.getRawParameterValue ("cutoff")->load();
    const float q = apvts.getRawParameterValue ("q")->load();
    const int slopeChoice = static_cast<int> (apvts.getRawParameterValue ("slope")->load());
    const int stages = slopeIndexToStages (slopeChoice);

    cutoffSmoothed.setTargetValue (cutoffTarget);

    const int channels = juce::jmin (2, buffer.getNumChannels());
    const int samples = buffer.getNumSamples();

    for (int sample = 0; sample < samples; ++sample)
    {
        const float cutoff = cutoffSmoothed.getNextValue();
        if (std::abs (cutoff - lastCutoffForCoeffs) > 0.25f || q != lastQ || stages != lastStages)
        {
            updateCoefficients (cutoff, q, stages);
            lastCutoffForCoeffs = cutoff;
            lastQ = q;
            lastStages = stages;
        }

        for (int ch = 0; ch < channels; ++ch)
        {
            float x = buffer.getSample (ch, sample);
            for (int stage = 0; stage < stages; ++stage)
                x = filters[ch][stage].processSample (x);
            buffer.setSample (ch, sample, x);
        }
    }
}

void AILSFilterHPProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void AILSFilterHPProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorEditor* AILSFilterHPProcessor::createEditor()
{
    return new AILSFilterHPEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AILSFilterHPProcessor();
}
