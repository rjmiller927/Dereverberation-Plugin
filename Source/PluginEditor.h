/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/



#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Dereverb.hpp"
#include "SimpleMeter.h"


//==============================================================================
/**
*/
class DereverbAudioProcessorEditor  :   public AudioProcessorEditor,
                                        public Slider::Listener,
                                        public Button::Listener,
                                        public Timer
{
public:
    DereverbAudioProcessorEditor (DereverbAudioProcessor&);
    ~DereverbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider *slider) override;
    void buttonClicked(Button *button) override;
    void timerCallback() override;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DereverbAudioProcessor& processor;
    
    
    //==============================================================================
    // Define GUI controls
    //==============================================================================
    Slider reverbReductionSlider;
    Slider makeupGainSlider;
    ToggleButton bypassButton;
    
    
    //==============================================================================
    // GUI componenet attachements
    //==============================================================================
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> sliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    
    
    //==============================================================================
    // GUI Meters
    //==============================================================================
    SimpleMeter inputMeter;
    SimpleMeter outputMeter;
    
    
    //==============================================================================
    // GUI component labels
    //==============================================================================
    Label reverbSliderLabel;
    Label makeupGainLabel;
    Label bypassLabel;
    Label inputMeterLabel;
    Label outputMeterLabel;
    

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DereverbAudioProcessorEditor)
};
