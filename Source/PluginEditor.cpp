/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MoToAudioProcessorEditor::MoToAudioProcessorEditor (MoToAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

  // logo = juce::ImageCache::getFromMemory(BinaryData::logo686_png, BinaryData::logo686_pngSize);

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

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setResizable(true, true);
  setSize (360, 180);
}

MoToAudioProcessorEditor::~MoToAudioProcessorEditor()
{
}

#define NX 24
#define NY 10
#define DELTAX 0.2f
#define DELTAY 0.2f
#define BORDER 0.05f
#define RED reduced(DELTAX*dux,DELTAY*duy)

//==============================================================================
void MoToAudioProcessorEditor::paint (juce::Graphics& g)
{
    float uxb = BORDER*getWidth();
    float uyb = BORDER*getHeight();
    auto ux = (1-2*BORDER)*getWidth()/NX;
    auto uy = (1-2*BORDER)*getHeight()/NY;

    auto diagonale = (getLocalBounds().getTopLeft() - getLocalBounds().getBottomRight()).toFloat();
    auto length = diagonale.getDistanceFromOrigin();
    auto perpendicular = diagonale.rotatedAboutOrigin (juce::degreesToRadians (90.0f)) / length;
    auto height = float (getWidth() * getHeight()) / length;
    auto bluegreengrey = juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.25f, 1.0f);
    juce::ColourGradient grad (bluegreengrey.darker().darker().darker(), perpendicular * height,
                           bluegreengrey, perpendicular * -height, false);
    g.setGradientFill(grad);
    g.fillAll();

    // auto r = juce::Rectangle<float>(uxb+20.5*ux,uyb+7.5*uy,4*ux,4*ux*227/269);
    // g.drawImage(logo, r);
    // g.setColour(juce::Colours::white);
    // g.drawText("MoTo",juce::Rectangle<float>(uxb+20.5*ux,uyb+6.5*uy,4*ux,uy),juce::Justification::centred);
   
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
  fb4.items.add(juce::FlexItem(logo).withFlex(0.2f).withMargin(juce::FlexItem::Margin(5.0f, 10.0f, 5.0f, 10.0f)).withAlignSelf(juce::FlexItem::AlignSelf::stretch));
  fbMain.items.add(juce::FlexItem(fb1).withFlex(1.1f).withMargin(juce::FlexItem::Margin(0.0f, 0.0f, 0.0f, 0.0f)));
  fbMain.items.add(juce::FlexItem(fb4).withFlex(1.f));
  fbMain.performLayout(getLocalBounds().toFloat());
}
