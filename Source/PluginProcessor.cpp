/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MonitoringSectionAudioProcessor::MonitoringSectionAudioProcessor()
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

MonitoringSectionAudioProcessor::~MonitoringSectionAudioProcessor()
{
    
}

//==============================================================================
const juce::String MonitoringSectionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MonitoringSectionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MonitoringSectionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MonitoringSectionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MonitoringSectionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MonitoringSectionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MonitoringSectionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MonitoringSectionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MonitoringSectionAudioProcessor::getProgramName (int index)
{
    return {};
}

void MonitoringSectionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MonitoringSectionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MonitoringSectionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MonitoringSectionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MonitoringSectionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    // We start from the last channel because the first channel pair
    // contains the input data. We treat them at the end

    for (int chanpair = NUM_STEREO_OUT-1; chanpair > -1; --chanpair)
    {
        float gain = outLevel * (apvts.getRawParameterValue(choices[chanpair])->load() ? 1.f : 0.f);
        if (!mono)
        {
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
        else
        {
            if (chanpair > 0)
            {
                buffer.addFrom(2*chanpair,0,buffer,0,0,buffer.getNumSamples(),.5*gain);
                buffer.addFrom(2*chanpair+1,0,buffer,1,0,buffer.getNumSamples(),.5*gain);
                buffer.addFrom(2*chanpair,0,buffer,1,0,buffer.getNumSamples(),.5*gain);
                buffer.addFrom(2*chanpair+1,0,buffer,0,0,buffer.getNumSamples(),.5*gain);
            }
            else
            {
                buffer.applyGain(0,0,buffer.getNumSamples(),.5*gain);
                buffer.addFrom(0,0,buffer,1,0,buffer.getNumSamples(),.5*gain);
                buffer.clear(1,0,buffer.getNumSamples());
                buffer.addFrom(1,0,buffer,0,0,buffer.getNumSamples(),1.f);
            }
        }
    }
}

//==============================================================================
bool MonitoringSectionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MonitoringSectionAudioProcessor::createEditor()
{
    //return new juce::GenericAudioProcessorEditor(*this);
    return new MonitoringSectionAudioProcessorEditor (*this);
}

//==============================================================================
void MonitoringSectionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MonitoringSectionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MonitoringSectionAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MonitoringSectionAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level","Level",juce::NormalisableRange<float>(-60.0f,6.f,0.1f,1.f),0.f));
    
    juce::StringArray choices;
    choices.addArray(CHOICES);
    for (int i=0; i<NUM_STEREO_OUT; ++i)
    {
        layout.add(std::make_unique<juce::AudioParameterBool>(choices[i], choices[i], i==0 ? true : false));
    }
    layout.add(std::make_unique<juce::AudioParameterBool>("Mute","Mute", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("Dim","Dim", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("Mono","Mono", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("Exclusive","Exclusive", true));

    return layout;
}
