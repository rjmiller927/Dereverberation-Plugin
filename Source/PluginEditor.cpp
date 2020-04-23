/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/



#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DereverbAudioProcessorEditor::DereverbAudioProcessorEditor (DereverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // ===========================================
    // REVERB REDUCTION SLIDER
    // ===========================================
    reverbReductionSlider.setSliderStyle(Slider::LinearHorizontal);
    reverbReductionSlider.setRange(0.0f, 100.0f, 1.f);
    reverbReductionSlider.setBounds(125, 30, 250, 75);
    reverbReductionSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 25);
    addAndMakeVisible(reverbReductionSlider);
    
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(
                                                                        processor.state, "DEREVERB", reverbReductionSlider));
    
    reverbSliderLabel.setText("Reverb Reduction(%)", dontSendNotification);
    reverbSliderLabel.attachToComponent(&reverbReductionSlider, true);
    reverbSliderLabel.setJustificationType(Justification::left);
    addAndMakeVisible(reverbSliderLabel);
    
    // ===========================================
    // MAKEUP GAIN SLIDER
    // ===========================================
    makeupGainSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    makeupGainSlider.setRange(-6.0f, 6.0f, 0.01f); // Value in dB
    makeupGainSlider.setBounds(125, 130, 115, 115);
    makeupGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(makeupGainSlider);
    
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(
                                                                processor.state, "MAKEUPGAIN", makeupGainSlider));
    
    makeupGainLabel.setText("Make-up Gain (dB)", dontSendNotification);
    makeupGainLabel.attachToComponent(&makeupGainSlider, false);
    makeupGainLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(makeupGainLabel);
    
    // ===========================================
    // BYPASS BUTTON
    // ===========================================
    //bypassButton.addListener(this);
    bypassButton.setBounds(50, 130, 75, 30);
    bypassButton.setClickingTogglesState(true);
    addAndMakeVisible(bypassButton);
    
    bypassLabel.setText("Bypass", dontSendNotification);
    bypassLabel.attachToComponent(&bypassButton, false);
    bypassLabel.setJustificationType(Justification::left);
    addAndMakeVisible(bypassLabel);
    
    buttonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.state, "BYPASS", bypassButton);

    // ===========================================
    // VU METER
    // ===========================================
    inputMeter.setBounds(300, 100, 20, 165);
    addAndMakeVisible(inputMeter);
    
    outputMeter.setBounds(350, 100, 20, 165); // x, y, width, height
    addAndMakeVisible(outputMeter);
    
    startTimer(60);
    
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
    g.setFont(14.0f);
    g.drawFittedText("Input", 290, 75, 40, 20, Justification::left, 1);
    g.drawFittedText("Output", 340, 75, 40, 20, Justification::left, 1);
    
    
}

void DereverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void DereverbAudioProcessorEditor::sliderValueChanged(Slider *slider){
    
}

void DereverbAudioProcessorEditor::buttonClicked(Button *button){
    
}

void DereverbAudioProcessorEditor::timerCallback(){
    inputMeter.update(processor.inputMeterValue);
    outputMeter.update(processor.outputMeterValue);
}
