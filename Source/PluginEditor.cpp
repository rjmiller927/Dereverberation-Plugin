/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/



#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AdaptiveFilter.hpp"

//==============================================================================
DereverbAudioProcessorEditor::DereverbAudioProcessorEditor (DereverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 250);
    
    // REVERB REDUCTION SLIDER
    reverbReductionSlider.addListener(this);
    reverbReductionSlider.setSliderStyle(Slider::LinearHorizontal);
    reverbReductionSlider.setRange(0.0f, 100.0f, 0.01f);
    reverbReductionSlider.setBounds(125, 30, 250, 75);
    reverbReductionSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 25);
    addAndMakeVisible(reverbReductionSlider);
    
    reverbSliderLabel.setText("Reverb Reduction(%)", dontSendNotification);
    reverbSliderLabel.attachToComponent(&reverbReductionSlider, true);
    reverbSliderLabel.setJustificationType(Justification::left);
    addAndMakeVisible(reverbSliderLabel);
    
    // MAKEUP GAIN SLIDER
    makeupGainSlider.addListener(this);
    makeupGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    makeupGainSlider.setRange(-6.0f, 6.0f, 0.01f); // Value in dB
    makeupGainSlider.setBounds(150, 130, 115, 115);
    makeupGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(makeupGainSlider);
    
    makeupGainLabel.setText("Make-up Gain (dB)", dontSendNotification);
    makeupGainLabel.attachToComponent(&makeupGainSlider, false);
    makeupGainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(makeupGainLabel);

    
}

DereverbAudioProcessorEditor::~DereverbAudioProcessorEditor()
{
}

//==============================================================================
void DereverbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll (Colours::dodgerblue);

    // Plug-In Title
    g.setColour (Colours::whitesmoke);
    g.setFont (30.0f);
    g.drawFittedText ("De-Reverberator", getLocalBounds(), Justification::centredTop, 1);
    g.setFont(12.0f);
    g.drawFittedText("Ryan Miller", getLocalBounds(), Justification::bottomRight, 1);
    
    
}

void DereverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void DereverbAudioProcessorEditor::sliderValueChanged(Slider *slider){
    
    // Reverb Reduction Slider
    if (slider == &reverbReductionSlider){
        processor.mix = slider->getValue() / 100.0f; // Convert % to 0-1 scale
    }
    
    // Make-up gain Slider
    if (slider == &makeupGainSlider){
        processor.gain = slider -> getValue();
    }
}
