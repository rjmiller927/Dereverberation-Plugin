/*
  ==============================================================================

    STFT.cpp
    Created: 8 Apr 2020 3:44:45pm
    Author:  Ryan Miller

  ==============================================================================
*/

#include "STFT.h"
#include "PluginProcessor.h"



//======================================

STFT::STFT() : numChannels (1)
{
    dereverbFilter = std::make_unique<Dereverb>();
}

STFT::~STFT()
{
}


//======================================

void STFT::setup (const int numInputChannels)
{
    numChannels = (numInputChannels > 0) ? numInputChannels : 1;
}

void STFT::updateParameters (const int newFftSize, const int newOverlap, const int newWindowType)
{
    updateFftSize (newFftSize);
    updateHopSize (newOverlap);
    updateWindow (newWindowType);
}

//======================================

void STFT::processBlock (AudioSampleBuffer& block)
{
    
    
    numSamples = block.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel) {
        float* channelData = block.getWritePointer (channel);
        
        currentInputBufferWritePosition = inputBufferWritePosition;
        currentOutputBufferWritePosition = outputBufferWritePosition;
        currentOutputBufferReadPosition = outputBufferReadPosition;
        currentSamplesSinceLastFFT = samplesSinceLastFFT;
        
        for (int sample = 0; sample < numSamples; ++sample) {
            const float inputSample = channelData[sample];
            inputBuffer.setSample (channel, currentInputBufferWritePosition, inputSample);
            if (++currentInputBufferWritePosition >= inputBufferLength)
                currentInputBufferWritePosition = 0;
            
            channelData[sample] = outputBuffer.getSample (channel, currentOutputBufferReadPosition);
            
            outputBuffer.setSample (channel, currentOutputBufferReadPosition, 0.0f);
            if (++currentOutputBufferReadPosition >= outputBufferLength)
                currentOutputBufferReadPosition = 0;
            
            if (++currentSamplesSinceLastFFT >= hopSize) {
                currentSamplesSinceLastFFT = 0;
                
                //======================================
                analysis (channel);
                modification();
                synthesis (channel);
                //======================================
                
            }
        }
    }
    
    inputBufferWritePosition = currentInputBufferWritePosition;
    outputBufferWritePosition = currentOutputBufferWritePosition;
    outputBufferReadPosition = currentOutputBufferReadPosition;
    samplesSinceLastFFT = currentSamplesSinceLastFFT;
}




//======================================

void STFT::updateFftSize (const int newFftSize)
{
    fftSize = newFftSize;
    fft = std::make_unique<dsp::FFT>(log2 (fftSize));
    
    inputBufferLength = fftSize;
    inputBuffer.clear();
    inputBuffer.setSize (numChannels, inputBufferLength);
    
    outputBufferLength = fftSize;
    outputBuffer.clear();
    outputBuffer.setSize (numChannels, outputBufferLength);
    
    fftWindow.realloc (fftSize);
    fftWindow.clear (fftSize);
    
    timeDomainBuffer.realloc (fftSize);
    timeDomainBuffer.clear (fftSize);
    
    frequencyDomainBuffer.realloc (fftSize);
    frequencyDomainBuffer.clear (fftSize);
    
    inputBufferWritePosition = 0;
    outputBufferWritePosition = 0;
    outputBufferReadPosition = 0;
    samplesSinceLastFFT = 0;
}

void STFT::updateHopSize (const int newOverlap)
{
    overlap = newOverlap;
    if (overlap != 0) {
        hopSize = fftSize / overlap;
        outputBufferWritePosition = hopSize % outputBufferLength;
    }
}

void STFT::updateWindow (const int newWindowType)
{
    switch (newWindowType) {
        case windowTypeRectangular: {
            for (int sample = 0; sample < fftSize; ++sample)
                fftWindow[sample] = 1.0f;
            break;
        }
        case windowTypeBartlett: {
            for (int sample = 0; sample < fftSize; ++sample)
                fftWindow[sample] = 1.0f - fabs (2.0f * (float)sample / (float)(fftSize - 1) - 1.0f);
            break;
        }
        case windowTypeHann: {
            for (int sample = 0; sample < fftSize; ++sample)
                fftWindow[sample] = 0.5f - 0.5f * cosf (2.0f * M_PI * (float)sample / (float)(fftSize - 1));
            break;
        }
        case windowTypeHamming: {
            for (int sample = 0; sample < fftSize; ++sample)
                fftWindow[sample] = 0.54f - 0.46f * cosf (2.0f * M_PI * (float)sample / (float)(fftSize - 1));
            break;
        }
    }
    
    float windowSum = 0.0f;
    for (int sample = 0; sample < fftSize; ++sample)
        windowSum += fftWindow[sample];
    
    windowScaleFactor = 0.0f;
    if (overlap != 0 && windowSum != 0.0f)
        windowScaleFactor = 1.0f / (float)overlap / windowSum * (float)fftSize;
}

//======================================

void STFT::analysis (const int channel)
{
    int inputBufferIndex = currentInputBufferWritePosition;
    for (int index = 0; index < fftSize; ++index) {
        // Time domain buffer created. Beginning of buffer starts from the currentInputBufferWritePosition and then wraps around the inputBufferLength. Window filtering is also applied.
        timeDomainBuffer[index].real (fftWindow[index] * inputBuffer.getSample (channel, inputBufferIndex));
        timeDomainBuffer[index].imag (0.0f);
        
        if (++inputBufferIndex >= inputBufferLength)
            inputBufferIndex = 0;
    }
}

// Add any frequency domain processing into this class
void STFT::modification()
{
    // Forward fourier transform
    fft->perform (timeDomainBuffer, frequencyDomainBuffer, false);
    
    for (int index = 0; index < fftSize / 2 + 1; ++index) {
        float magnitude = abs (frequencyDomainBuffer[index]);
        float phase = arg (frequencyDomainBuffer[index]);
        
        frequencyDomainBuffer[index].real (magnitude * cosf (phase));
        frequencyDomainBuffer[index].imag (magnitude * sinf (phase));
        if (index > 0 && index < fftSize / 2) {
            frequencyDomainBuffer[fftSize - index].real (magnitude * cosf (phase));
            frequencyDomainBuffer[fftSize - index].imag (magnitude * sinf (-phase));
        }
    }
    
    // Peform de-reverberation
    dereverbFilter->processBuffer(frequencyDomainBuffer, fftSize);
    
    // Inverse Fourier Transform
    fft->perform (frequencyDomainBuffer, timeDomainBuffer, true);
}
    
void STFT::synthesis (const int channel)
{
    int outputBufferIndex = currentOutputBufferWritePosition;
    for (int index = 0; index < fftSize; ++index) {
        float outputSample = outputBuffer.getSample (channel, outputBufferIndex);
        outputSample += timeDomainBuffer[index].real() * windowScaleFactor;
        outputBuffer.setSample (channel, outputBufferIndex, outputSample);
        
        if (++outputBufferIndex >= outputBufferLength)
            outputBufferIndex = 0;
    }
    
    currentOutputBufferWritePosition += hopSize;
    if (currentOutputBufferWritePosition >= outputBufferLength)
        currentOutputBufferWritePosition = 0;
}
    
    
    

