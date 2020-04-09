/*
 ==============================================================================
 Code by Juan Gil <https://juangil.com/>.
 Copyright (C) 2017-2019 Juan Gil.
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "Dereverb.hpp"

//==============================================================================


class STFT
{
public:
    enum windowTypeIndex {
        windowTypeRectangular = 0,
        windowTypeBartlett,
        windowTypeHann,
        windowTypeHamming,
    };
    
    //======================================
    
    STFT();
    virtual ~STFT();
    
    std::unique_ptr<Dereverb> dereverbFilter;
    
    //======================================
    
    void setup (const int numInputChannels);
    void updateParameters (const int newFftSize, const int newOverlap, const int newWindowType);
    
    //======================================
    
    void processBlock (AudioSampleBuffer& block);
    
    
private:
    //======================================
    
    void updateFftSize (const int newFftSize);
    void updateHopSize (const int newOverlap);
    void updateWindow (const int newWindowType);
    
    //======================================
    
    void analysis (const int channel);
    virtual void modification();
    void synthesis (const int channel);
    
    
protected:
    //======================================
    int numChannels;
    int numSamples;
    
    int fftSize;
    std::unique_ptr<dsp::FFT> fft;
    
    int inputBufferLength;
    AudioSampleBuffer inputBuffer;
    
    int outputBufferLength;
    AudioSampleBuffer outputBuffer;
    
    HeapBlock<float> fftWindow;
    HeapBlock<dsp::Complex<float>> timeDomainBuffer;
    HeapBlock<dsp::Complex<float>> frequencyDomainBuffer;
    
    int overlap;
    int hopSize;
    float windowScaleFactor;
    
    int inputBufferWritePosition;
    int outputBufferWritePosition;
    int outputBufferReadPosition;
    int samplesSinceLastFFT;
    
    int currentInputBufferWritePosition;
    int currentOutputBufferWritePosition;
    int currentOutputBufferReadPosition;
    int currentSamplesSinceLastFFT;
};

//==============================================================================
