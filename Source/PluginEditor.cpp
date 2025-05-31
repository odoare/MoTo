/*
  ------------------------------------------------------------------------------

    PluginEditor.cpp
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

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MoToAudioProcessorEditor::MoToAudioProcessorEditor (MoToAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

  addAndMakeVisible(levelSlider.slider);
  levelSlider.slider.setLookAndFeel(&knobLookAndFeel);
  
  addAndMakeVisible(muteButton.button);
  muteButton.button.setLookAndFeel(&buttonLookAndFeel);
  addAndMakeVisible(dimButton.button);
  dimButton.button.setLookAndFeel(&buttonLookAndFeel);
  addAndMakeVisible(monoButton.button);
  monoButton.button.setLookAndFeel(&buttonLookAndFeel);

  buttonsComponent.prepare();

  addAndMakeVisible(meterL);
  addAndMakeVisible(meterR);

  tabs.addTab("Channel selector", juce::Colours::transparentBlack, &buttonsComponent, false);
  tabs.addTab("Volume adjustement", juce::Colours::transparentBlack, &knobsComponent, false);
  tabs.setCurrentTabIndex(0);
  addAndMakeVisible(tabs);

  addAndMakeVisible(logo);

  setResizable(true, true);
  setSize (360, 180);
}

MoToAudioProcessorEditor::~MoToAudioProcessorEditor()
{
}

//==============================================================================
void MoToAudioProcessorEditor::paint (juce::Graphics& g)
{

    auto diagonale = (getLocalBounds().getTopLeft() - getLocalBounds().getBottomRight()).toFloat();
    auto length = diagonale.getDistanceFromOrigin();
    auto perpendicular = diagonale.rotatedAboutOrigin (juce::degreesToRadians (90.0f)) / length;
    auto height = float (getWidth() * getHeight()) / length;
    auto bluegreengrey = juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.25f, 1.0f);
    juce::ColourGradient grad (bluegreengrey.darker().darker().darker(), perpendicular * height,
                           bluegreengrey, perpendicular * -height, false);
    g.setGradientFill(grad);
    g.fillAll();
   
}

void MoToAudioProcessorEditor::resized()
{
  juce::FlexBox fb1, fb2, fb21, fb22, fb3, fb4, fbMain;
  fb1.flexDirection = juce::FlexBox::Direction::row;
  fb2.flexDirection = juce::FlexBox::Direction::column;
  fb21.flexDirection = juce::FlexBox::Direction::row;
  fb22.flexDirection = juce::FlexBox::Direction::row;
  fb3.flexDirection = juce::FlexBox::Direction::column;
  fb4.flexDirection = juce::FlexBox::Direction::row;
  fbMain.flexDirection = juce::FlexBox::Direction::column;

  fb1.items.add(juce::FlexItem(levelSlider.slider).withFlex(0.3f));
  fb1.items.add(juce::FlexItem(fb2).withFlex(0.7f));
  fb2.items.add(juce::FlexItem(tabs).withFlex(1.f));
  fb3.items.add(juce::FlexItem(fb22).withFlex(1.8f));
  fb22.items.add(juce::FlexItem(muteButton.button).withFlex(1.0f));
  fb22.items.add(juce::FlexItem(dimButton.button).withFlex(1.0f));
  fb22.items.add(juce::FlexItem(monoButton.button).withFlex(1.0f));
  fb3.items.add(juce::FlexItem(meterL).withFlex(1.0f));
  fb3.items.add(juce::FlexItem(meterR).withFlex(1.0f));
  fb4.items.add(juce::FlexItem(fb3).withFlex(1.0f));
  fb4.items.add(juce::FlexItem(logo).withFlex(0.2f).withMargin(juce::FlexItem::Margin(5.f, 5.f, 5.f, 5.f)).withAlignSelf(juce::FlexItem::AlignSelf::stretch));
  fbMain.items.add(juce::FlexItem(fb1).withFlex(1.1f));
  fbMain.items.add(juce::FlexItem(fb4).withFlex(1.f));
  fbMain.performLayout(getLocalBounds().toFloat());
}
