/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/



#pragma once

#include <JuceHeader.h>
#include "Dereverb.hpp"
#include "STFT.h"


//==============================================================================
/**
*/
class DereverbAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DereverbAudioProcessor();
    ~DereverbAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    //==============================================================================
    // USER DEFINED VARIABLES
    //==============================================================================
    
    // Holds the states of all parameters
    AudioProcessorValueTreeState state;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    

    //==============================================================================
    // FFT Object from GitHub Freq Domain Template (https://github.com/juandagilc/Audio-Effects/tree/master/Template%20Frequency%20Domain)
    //==============================================================================
    STFT stft;
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DereverbAudioProcessor)
    
    void applyMakeupGain(AudioBuffer<float> &buffer, float linGain);
    
    // FFT Parameters
    const int fftSize = 4096;
    const int hopSize = 8; // Hop size factor; hop samples = fftSize / hopSize
    
    // Parameter smoothing
    float gainSmooth = 0.f;
    
    float alpha = 0.999f;
    
    //==============================================================================
};
