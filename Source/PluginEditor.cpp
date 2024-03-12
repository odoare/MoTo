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

  auto selectFunc = [this](int i){      
    if (audioProcessor.apvts.getRawParameterValue("Exclusive")->load()){
      bool state = channelButton[i].getToggleState();
      for (int j=0; j<NUM_STEREO_OUT; ++j)
      {
          if (j!=i && state) channelButton[j].setToggleState(false,juce::sendNotification);
      }
    };
  };

  logo = juce::ImageCache::getFromMemory(BinaryData::logo686_png, BinaryData::logo686_pngSize);

  addController(levelSlider, juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Colours::cyan,juce::Colours::black);
  addAndConnectLabel(levelSlider, levelLabel);
  levelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level",levelSlider);
  levelSlider.setLookAndFeel(&knobLookAndFeel);

  for (int i=0; i<NUM_STEREO_OUT; ++i)
  {
    channelLabel[i].setText(audioProcessor.choices[i], juce::NotificationType::dontSendNotification);
    channelLabel[i].setComponentID(audioProcessor.choices[i]);
    channelIndex[i] = i;

    addAndMakeVisible(channelButton[i]);
    channelButtonAttachment[i] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,audioProcessor.choices[i],channelButton[i]);
    addAndMakeVisible(channelLabel[i]);
    channelLabel[i].attachToComponent(&channelButton[i],false);
    channelButton[i].setLookAndFeel(&buttonLookAndFeel);
    channelButton[i].setClickingTogglesState(true);
    channelButton[i].setColour(0,juce::Colours::cyan);
    channelButton[i].onClick = [selectFunc, i](){
      selectFunc(i); };
  }

  addAndMakeVisible(exclusiveButton);
  exclusiveButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Exclusive",exclusiveButton);
  addAndMakeVisible(exclusiveLabel);
  exclusiveLabel.attachToComponent(&exclusiveButton,false);
  exclusiveButton.setLookAndFeel(&buttonLookAndFeel);
  exclusiveButton.setClickingTogglesState(true);
  exclusiveButton.setColour(0,juce::Colours::cyan);

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

  // verticalMeterL.setColour(juce::Colours::white);
  // addAndMakeVisible(verticalMeterLmax);
  // verticalMeterR.setColour(juce::Colours::white);
  // addAndMakeVisible(verticalMeterRmax);

  // verticalMeterL.setColour(juce::Colours::green.brighter());
  // addAndMakeVisible(verticalMeterL);
  // verticalMeterR.setColour(juce::Colours::green.brighter());
  // addAndMakeVisible(verticalMeterR);

  addAndMakeVisible(meterL);
  addAndMakeVisible(meterR);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setResizable(true, true);
    setSize (360, 150);
}

MonitoringSectionAudioProcessorEditor::~MonitoringSectionAudioProcessorEditor()
{
}

#define NX 24
#define NY 10
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

    auto r = juce::Rectangle<float>(uxb+20.5*ux,uyb+6.25*uy,4*ux,4*ux*227/269);
    g.drawImage(logo, r);
   
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
  exclusiveButton.setBounds(juce::Rectangle<int>(uxb+5.25*dux,ey,1.25*bdux,juce::jmin(bdux,duy)).RED);

  muteButton.setBounds(juce::Rectangle<int>(uxb+2*dux,uyb+duy,dux,duy).RED);
  dimButton.setBounds(juce::Rectangle<int>(uxb+3*dux,uyb+duy,dux,duy).RED);
  monoButton.setBounds(juce::Rectangle<int>(uxb+4*dux,uyb+duy,dux,duy).RED);

  meterL.setBounds(uxb,uyb+2*duy,5*dux,0.25*duy);
  meterR.setBounds(uxb,uyb+2.25*duy,5*dux,0.25*duy);
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
