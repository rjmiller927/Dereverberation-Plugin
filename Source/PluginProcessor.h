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
    
    // Checks if plugin is in bypass state (-1) or not (1)
    int bypassCheck = 1;
    
    // Mix corresponds to reverb reduction amount. When reduction is 100%, that means that the output is entirely the adaptive filter output. When reduction is 0%, then the output is the same as the input
    float mix;
    float makeupGain;
    float reverbReductionPercent = 50.f;
    

    STFT stft;
    
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DereverbAudioProcessor)
    
    void applyMakeupGain(AudioBuffer<float> &buffer, float linGain);
    
   
    
    //==============================================================================
    // FFT Object from JUCE DSP module
    /*dsp::FFT fft;
    dsp::FFT fft2{fftOrder}; // Initialize directly with int order (FFT size is 2^fftOrder)
    float fifo[2][fftSize];
    float fftData[2][2*fftSize];
    int fifoIndex[2] = {0};
    bool nextFFTBlockReady = false;
    
    dsp::WindowingFunction<float> window;
    
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill, int channel);
    void pushNextSampleIntoFifo(float sample, int channel) noexcept;
    
    int N;
     */
    
    //==============================================================================
    // FFT Object from GitHub Freq Domain Template (https://github.com/juandagilc/Audio-Effects/tree/master/Template%20Frequency%20Domain)
    //==============================================================================
    class PassThrough : public STFT {
    private:
        void modification() override
        {
            
            // Forward FFT
            fft->perform (timeDomainBuffer, frequencyDomainBuffer, false);

            for (int index = 0; index < fftSize / 2 + 1; ++index) {
                float magnitude = abs (frequencyDomainBuffer[index]);
                float phase = arg (frequencyDomainBuffer[index]); // Returns the same as atan2(x.imag(),x.real())

                frequencyDomainBuffer[index].real (magnitude * cosf (phase));
                frequencyDomainBuffer[index].imag (magnitude * sinf (phase));

                // Set negative frequencies to the complex conjugate (except for DC)
                if (index > 0 && index < fftSize / 2) {
                    frequencyDomainBuffer[fftSize - index].real (magnitude * cosf (phase));
                    frequencyDomainBuffer[fftSize - index].imag (magnitude * sinf (-phase));
                }
            }

            // Frequency Domain Processing
            
            
            // Inverse FFT
            fft->perform (frequencyDomainBuffer, timeDomainBuffer, true);
        }
    };
    
    
    const int fftSize = 4096;
    const int hopSize = 2; // Hop size factor, hop samples = fftSize / hopSize
    
    //==============================================================================
};
