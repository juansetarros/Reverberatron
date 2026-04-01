/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Delay.h"

//==============================================================================
ReverberatronAudioProcessor::ReverberatronAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    Dprom = sqrt(0.65 / Vr);
    // Dprom = ((4 * juce::double_Pi * Vr) / (pow(343, 3))) / (4 * (1 / Fs));

    D1 = predelay + Dprom*1.1;
    D2 = predelay + Dprom*(1.f/1.1f);
    D3 = predelay + Dprom*1.2;
    D4 = predelay + Dprom*(1.f/1.2f);

    g1 = pow(10, (-3 * D1) / Rt);
    g2 = pow(10, (-3 * D2) / Rt);
    g3 = pow(10, (-3 * D3) / Rt);
    g4 = pow(10, (-3 * D4) / Rt);

}

ReverberatronAudioProcessor::~ReverberatronAudioProcessor()
{
}

//==============================================================================
const juce::String ReverberatronAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverberatronAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverberatronAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverberatronAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverberatronAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverberatronAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverberatronAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverberatronAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverberatronAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverberatronAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverberatronAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    Fs = sampleRate;
    vectorDelays1.resize(2);
    for (int i = 0; i < vectorDelays1.size(); i++)
    {
        vectorDelays1[i].setSampleRate(sampleRate, 5);
        vectorDelays1[i].setDelayTime(D1);
    }
    vectorDelays2.resize(2);
    for (int i = 0; i < vectorDelays2.size(); i++)
    {
        vectorDelays2[i].setSampleRate(sampleRate, 5);
        vectorDelays2[i].setDelayTime(D2);
    }
    vectorDelays3.resize(2);
    for (int i = 0; i < vectorDelays3.size(); i++)
    {
        vectorDelays3[i].setSampleRate(sampleRate, 5);
        vectorDelays3[i].setDelayTime(D3);
    }
    vectorDelays4.resize(2);
    for (int i = 0; i < vectorDelays4.size(); i++)
    {
        vectorDelays4[i].setSampleRate(sampleRate, 5);
        vectorDelays4[i].setDelayTime(D4);
    }
    APF1.resize(2);
    for (int i = 0; i < APF1.size(); i++)
    {
        APF1[i].setSampleRate(sampleRate, 4);
        APF1[i].setDelayTime(0.001f);
    }
    APF2.resize(2);
    for (int i = 0; i < APF2.size(); i++)
    {
        APF2[i].setSampleRate(sampleRate, 4);
        APF2[i].setDelayTime(0.001f);
    }
}

void ReverberatronAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverberatronAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReverberatronAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        Dprom = sqrt(0.65 / Vr);

        D1 = predelay + Dprom * 1.1;
        D2 = predelay + Dprom * (1.f / 1.1f);
        D3 = predelay + Dprom * 1.2;
        D4 = predelay + Dprom * (1.f / 1.2f);

        g1 = pow(10, (-3 * D1) / Rt);
        g2 = pow(10, (-3 * D2) / Rt);
        g3 = pow(10, (-3 * D3) / Rt);
        g4 = pow(10, (-3 * D4) / Rt);

        LPF.recalculateFilter();

        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            if (channel == 0)
            {
                float* channelData = buffer.getWritePointer(channel);
                float combs = vectorDelays1[channel].processSample(channelData[sample], g1)
                    + vectorDelays2[channel].processSample(channelData[sample], g2)
                    + vectorDelays3[channel].processSample(channelData[sample], g3)
                    + vectorDelays4[channel].processSample(channelData[sample], g4);
                float apf1 = (-0.3 * channelData[sample]) + APF1[channel].processSample(combs, 0.3);
                float apf2 = (-0.3 * channelData[sample]) + APF2[channel].processSample(apf1, 0.3);
                channelData[sample] = (channelData[sample] * (1 - dw)) + (LPF.filterSignal(apf2) * dw);
            }
            else if (channel == 1)
            {
                float* channelData = buffer.getWritePointer(channel);
                float combs = vectorDelays1[channel].processSample(channelData[sample], g1)
                    + vectorDelays2[channel].processSample(channelData[sample], g2)
                    + vectorDelays3[channel].processSample(channelData[sample], g3)
                    + vectorDelays4[channel].processSample(channelData[sample], g4);
                float apf1 = (-0.45 * channelData[sample]) + APF1[channel].processSample(combs, 0.45);
                float apf2 = (-0.45 * channelData[sample]) + APF2[channel].processSample(apf1, 0.45);
                channelData[sample] = (channelData[sample] * (1 - dw)) + (LPF.filterSignal(apf2) * dw);
            }
        }
    }
}

//==============================================================================
bool ReverberatronAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverberatronAudioProcessor::createEditor()
{
    return new ReverberatronAudioProcessorEditor (*this);
}

//==============================================================================
void ReverberatronAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverberatronAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverberatronAudioProcessor();
}
