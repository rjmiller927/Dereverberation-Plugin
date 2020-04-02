//
//  AdaptiveFilter.hpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//
// This class implements a blind dereverberation effect inspired by the following paper:
//
// K. Ohtani, T. Komatsu, T. Nishino, K. Takeda, "Adaptive Dereverberation method based on complementary wiener filter and modulation transfer function", REVERB Workshop 2014
//
// Modifications include removal of spectral subtraction block and user-controlled alpha values instead of adaptive alpha values. Issues in implementing the adaptive algorithm, in particular the frequency response of the maskingGain as shown in Equation (16) in the paper, rendered the adaptive functionality non-working, so user controlled alpha's was implemented to control the amount of de-reverberation



#ifndef Dereverb_hpp
#define Dereverb_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <JuceHeader.h>

using namespace std;

class Dereverb{
public:
    
    void setAlpha(float dereverbPercent); // dereverbPercent == [0.f,100.f]
    void processBlock(float *fftChannelData, int numSamples);

private:
    
    // Alpha values used to control amount of reverb reduction
    float alpha1 = 0.8f;
    float alpha2 = 0.2f;
    
    // FFT bin magnitude squared value
    float P = 0.f;
    
    // R1, R2 are the averaged power spectrum values. Calculated using exponential moving average
    float R1 = 0.0f;
    float R2 = 0.0f;
    float R1prev = 0.0f;
    float R2prev = 0.0f;
    
    // Masking gain
    float maskingGain = 0.0f;
    
    // R1, R2, maskingGain update methods
    void setR1R2(float inputPower);
    void setMaskingGain(float R1, float R2);
    
};

#endif /* AdaptiveFilter_hpp */
