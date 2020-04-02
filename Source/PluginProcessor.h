/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/



#pragma once

#include <JuceHeader.h>
#include "Dereverb.hpp"


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
    
    // Checks if plugin is in bypass state (-1) or not (1)
    int bypassCheck = 1;
    
    // Mix corresponds to reverb reduction amount. When reduction is 100%, that means that the output is entirely the adaptive filter output. When reduction is 0%, then the output is the same as the input
    float mix;
    float makeupGain;
    float reverbReductionPercent = 50.f;
    
    // Adaptive Filter Object
    Dereverb *dereverbFilter;
    
    enum {
        fftOrder = 12, // FFT Size is 2^fftOrder
        fftSize = 1 << fftOrder
    };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DereverbAudioProcessor)
    
    // Sampling Rate
    int Fs;
    float input;
    
    //==============================================================================
    // FFT Object from JUCE DSP module
    dsp::FFT fft;
    // dsp::FFT fft2{fftOrder}; // Initialize directly with int order (FFT size is 2^fftOrder)
    float fifo[2][fftSize];
    float fftData[2][2*fftSize];
    int fifoIndex[2] = {0};
    bool nextFFTBlockReady = false;
    
    dsp::WindowingFunction<float> window;
    
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill, int channel);
    void pushNextSampleIntoFifo(float sample, int channel) noexcept;
    
    int N;
    
    //==============================================================================
};
