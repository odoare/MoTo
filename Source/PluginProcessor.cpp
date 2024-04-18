/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MoToAudioProcessor::MoToAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::discreteChannels(2*NUM_STEREO_OUT), true)
                     #endif
                       )
#endif
{
    choices.addArray(CHOICES);
}

MoToAudioProcessor::~MoToAudioProcessor()
{
    
}

//==============================================================================
const juce::String MoToAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MoToAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MoToAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MoToAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MoToAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MoToAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MoToAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MoToAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MoToAudioProcessor::getProgramName (int index)
{
    return {};
}

void MoToAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MoToAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i=0;i<2;i++)
    {
        smoothedMaxLevel[i].reset(sampleRate,maxDecay);
        smoothedMaxLevel[i].setCurrentAndTargetValue(-30.0f);
    }
}

void MoToAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MoToAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MoToAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Get parameter values
    auto outLevel = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level")->load())
                    * (apvts.getRawParameterValue("Mute")->load() ? 0 : 1)
                    * (apvts.getRawParameterValue("Dim")->load() ? 0.5 : 1) ;
    auto mono = apvts.getRawParameterValue("Mono")->load();

    // We clear all the output channels from 2 to totalNumInputChannels-1
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // We apply gain on the first two channels

    buffer.applyGain(0,0,buffer.getNumSamples(),outLevel);
    buffer.applyGain(1,0,buffer.getNumSamples(),outLevel);

    if (mono)
    {
        buffer.applyGain(0,0,buffer.getNumSamples(),.5f);
        buffer.addFrom(0,0,buffer,1,0,buffer.getNumSamples(),.5f);
        buffer.clear(1,0,buffer.getNumSamples());
        buffer.addFrom(1,0,buffer,0,0,buffer.getNumSamples(),1.f);
    }

    // Vu-Meter
    for (int i=0; i<2; ++i)
    {
        smoothedMaxLevel[i].skip(buffer.getNumSamples());
        maxLevel[i] = juce::Decibels::gainToDecibels(buffer.getMagnitude(i,0,buffer.getNumSamples()));
        if (maxLevel[i] < smoothedMaxLevel[i].getCurrentValue())
            smoothedMaxLevel[i].setTargetValue(maxLevel[i]);
        else
            smoothedMaxLevel[i].setCurrentAndTargetValue(maxLevel[i]);

        // maxLevel[i] = juce::Decibels::gainToDecibels(buffer.getMagnitude(i,0,buffer.getNumSamples()));
    }

    // We start from the last channel because the first channel pair
    // contains the input data. We treat them at the end

    for (int chanpair = NUM_STEREO_OUT-1; chanpair > -1; --chanpair)
    {
        float gain = apvts.getRawParameterValue(choices[chanpair])->load() ? 1.f : 0.f ;
        if (chanpair > 0)
        {
            buffer.addFrom(2*chanpair,0,buffer,0,0,buffer.getNumSamples(),gain);
            buffer.addFrom(2*chanpair+1,0,buffer,1,0,buffer.getNumSamples(),gain);
        }
        else
        {
            buffer.applyGain(0,0,buffer.getNumSamples(),gain);
            buffer.applyGain(1,0,buffer.getNumSamples(),gain);
        }
    }
}

//==============================================================================
bool MoToAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MoToAudioProcessor::createEditor()
{
    //return new juce::GenericAudioProcessorEditor(*this);
    return new MoToAudioProcessorEditor (*this);
}

//==============================================================================
void MoToAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void MoToAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data,sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}


float MoToAudioProcessor::getMaxLevel(const int channel)
{
  return maxLevel[channel];
}

float MoToAudioProcessor::getSmoothedMaxLevel(const int channel)
{
  return smoothedMaxLevel[channel].getCurrentValue();
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MoToAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MoToAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level","Level",juce::NormalisableRange<float>(-60.0f,6.f,0.1f,1.f),0.f));
    
    juce::StringArray choices;
    choices.addArray(CHOICES);
    for (int i=0; i<NUM_STEREO_OUT; ++i)
    {
        layout.add(std::make_unique<juce::AudioParameterBool>(choices[i], choices[i], i==0 ? true : false));
    }
    layout.add(std::make_unique<juce::AudioParameterBool>("Mute","Mute", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Dim","Dim", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Mono","Mono", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("Exclusive","Exclusive", true));

    return layout;
}
