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

//==============================================================================
/**
*/
class DereverbAudioProcessorEditor  :   public AudioProcessorEditor,
                                        public Slider::Listener,
                                        public Button::Listener
{
public:
    DereverbAudioProcessorEditor (DereverbAudioProcessor&);
    ~DereverbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider *slider) override;
    
    void buttonClicked(Button *button) override;
    

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
    // GUI component labels
    //==============================================================================
    Label reverbSliderLabel;
    Label makeupGainLabel;
    Label bypassLabel;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DereverbAudioProcessorEditor)
};
