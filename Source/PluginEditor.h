/*
  ------------------------------------------------------------------------------

    PluginEditor.h
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
#include "PluginProcessor.h"
#include "Components/knobsComponent.h"
#include "Components/buttonsComponent.h"
#include "Components/FxmeLogo.h"

//==============================================================================
/**
*/
class MoToAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MoToAudioProcessorEditor (MoToAudioProcessor&);
    ~MoToAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MoToAudioProcessor& audioProcessor;

    void addController(juce::Slider&, juce::Slider::SliderStyle, juce::Colour, juce::Colour);
    void addAndConnectLabel(juce::Slider&, juce::Label&);

    fxme::FxmeKnob levelSlider{ audioProcessor.apvts, "Level", juce::Colours::cyan};
    fxme::FxmeButton muteButton{audioProcessor.apvts, "Mute", juce::Colours::red};
    fxme::FxmeButton dimButton{audioProcessor.apvts, "Dim", juce::Colours::yellow};
    fxme::FxmeButton monoButton{audioProcessor.apvts, "Mono", juce::Colours::green};


    fxme::FxmeLookAndFeel buttonLookAndFeel;
    fxme::FxmeLookAndFeel knobLookAndFeel;

    fxme::HorizontalVuMeter meterL{[&]() { return audioProcessor.getMaxLevel(0); },
                                [&]() { return audioProcessor.getSmoothedMaxLevel(0); }},
                         meterR{[&]() { return audioProcessor.getMaxLevel(1); },
                                [&]() { return audioProcessor.getSmoothedMaxLevel(1); }};

    int lastChangedChannelIndex;

    // juce::Image logo;
    FxmeLogo logo{ProjectInfo::projectName+juce::String(" ")+ProjectInfo::versionString};

    juce::TabbedComponent tabs{juce::TabbedButtonBar::TabsAtTop};
    ButtonsComponent buttonsComponent{audioProcessor.apvts, audioProcessor.choices, "Exclusive", buttonLookAndFeel};
    KnobsComponent knobsComponent{audioProcessor.apvts, audioProcessor.choicesL, knobLookAndFeel};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoToAudioProcessorEditor)
};
