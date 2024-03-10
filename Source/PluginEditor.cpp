/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MonitoringSectionAudioProcessorEditor::MonitoringSectionAudioProcessorEditor (MonitoringSectionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
  addController(levelSlider, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::cyan,juce::Colours::black);
  addAndConnectLabel(levelSlider, levelLabel);
  levelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level",levelSlider);
  //directLevelSlider.onDragEnd = updateFunc;
  levelSlider.setLookAndFeel(&knobLookAndFeel);

  for (int i=0; i<NUM_STEREO_OUT; ++i)
  {
    channelLabel[i].setText(audioProcessor.choices[i], juce::NotificationType::dontSendNotification);
    channelLabel[i].setComponentID(audioProcessor.choices[i]);

    addAndMakeVisible(channelButton[i]);
    channelButtonAttachment[i] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,audioProcessor.choices[i],channelButton[i]);
    addAndMakeVisible(channelLabel[i]);
    channelLabel[i].attachToComponent(&channelButton[i],false);
    channelButton[i].setLookAndFeel(&buttonLookAndFeel);
    channelButton[i].setClickingTogglesState(true);
    channelButton[i].setColour(0,juce::Colours::cyan);

  }

  addAndMakeVisible(muteButton);
  muteButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Mute",muteButton);
  addAndMakeVisible(muteLabel);
  muteLabel.attachToComponent(&muteButton,false);
  muteButton.setLookAndFeel(&buttonLookAndFeel);
  muteButton.setClickingTogglesState(true);
  muteButton.setColour(0,juce::Colours::red);

  addAndMakeVisible(dimButton);
  dimButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Dim",dimButton);
  addAndMakeVisible(dimLabel);
  dimLabel.attachToComponent(&dimButton,false);
  dimButton.setLookAndFeel(&buttonLookAndFeel);
  dimButton.setClickingTogglesState(true);
  dimButton.setColour(0,juce::Colours::green);

  addAndMakeVisible(monoButton);
  monoButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Mono",monoButton);
  addAndMakeVisible(monoLabel);
  monoLabel.attachToComponent(&monoButton,false);
  monoButton.setLookAndFeel(&buttonLookAndFeel);
  monoButton.setClickingTogglesState(true);
  monoButton.setColour(0,juce::Colours::blue);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (360, 144);
}

MonitoringSectionAudioProcessorEditor::~MonitoringSectionAudioProcessorEditor()
{
}

#define NX 20
#define NY 8
#define DELTAX 0.2f
#define DELTAY 0.2f
#define BORDER 0.05f
#define RED reduced(DELTAX*dux,DELTAY*duy)

//==============================================================================
void MonitoringSectionAudioProcessorEditor::paint (juce::Graphics& g)
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

}

void MonitoringSectionAudioProcessorEditor::resized()
{
  float uxb = BORDER*getWidth();
  float uyb =BORDER*getHeight();
  auto ux = (1-2*BORDER)*getWidth()/NX;
  auto uy = (1-2*BORDER)*getHeight()/NY;
  auto dux=4*ux;
  auto duy=4*uy;

  levelSlider.setBounds(juce::Rectangle<int>(uxb,uyb+0.125*duy,2*dux,2*duy).RED);

  float ex = uxb+2*dux;
  float ey = uyb+0.125*duy;
  float bdux = (3*dux/NUM_STEREO_OUT);
  for (int i=0; i<NUM_STEREO_OUT; ++i)
  {
    channelButton[i].setBounds(juce::Rectangle<int>(ex+i*bdux,ey,bdux,juce::jmin(bdux,duy)).RED);
  }


  muteButton.setBounds(juce::Rectangle<int>(uxb+2*dux,uyb+duy,dux,duy).RED);
  //muteLabel.setTopLeftPosition(uxb+dux*2.25f,uyb+duy*0.95f);

  dimButton.setBounds(juce::Rectangle<int>(uxb+3*dux,uyb+duy,dux,duy).RED);
  //dimLabel.setTopLeftPosition(uxb+dux*3.25f,uyb+duy*0.95f);

  monoButton.setBounds(juce::Rectangle<int>(uxb+4*dux,uyb+duy,dux,duy).RED);
  //monoLabel.setTopLeftPosition(uxb+dux*4.25f,uyb+duy*0.95f);

}

void MonitoringSectionAudioProcessorEditor::addController(juce::Slider& slider,
                                                    juce::Slider::SliderStyle style,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(style);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,100,20);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  addAndMakeVisible(slider);
}

void MonitoringSectionAudioProcessorEditor::addAndConnectLabel(juce::Slider& slider,
                                                juce::Label& label)
{
  addAndMakeVisible(label);
  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
}
