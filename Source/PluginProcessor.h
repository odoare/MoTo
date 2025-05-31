/*
  ------------------------------------------------------------------------------

    PluginProcessor.h
    Author:  Olivier Doaré
    github.com/odoare

    (c) 2023-2025 Olivier Doaré

    Licenced under the GNU Lesser General Public License (LGPL) Version 3.0
    SPDX-License-Identifier: LGPL-3.0-or-later

  ------------------------------------------------------------------------------
    This file is part of the MoTo plugin.

    MoTo is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MoTo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with MoTo. If not, see <https://www.gnu.org/licenses/>.
  ------------------------------------------------------------------------------
*/

#pragma once

#include <JuceHeader.h>

#include <iostream>

#define CHOICES {"A", "B", "C", "D", "E", "F", "G"}
#define CHOICESL {"levelA", "levelB", "levelC", "levelD", "levelE", "levelF", "levelG"}
#define NUM_STEREO_OUT 5
#define NUM_METER_CHANNELS 2

//==============================================================================
/**
*/
class MoToAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MoToAudioProcessor();
    ~MoToAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float getSmoothedMaxLevel(const int channel);
    float getMaxLevel(const int channel);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();  
    juce::AudioProcessorValueTreeState apvts{*this,nullptr,"Parameters",createParameters()};

    juce::StringArray choices, choicesL;

private:
    juce::LinearSmoothedValue<float> smoothedMaxLevel[NUM_METER_CHANNELS]; 
    float maxLevel[NUM_METER_CHANNELS];
    float maxDecay{2.f};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoToAudioProcessor)
};
