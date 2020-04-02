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
#include <JuceHeader.h>

using namespace std;

class Dereverb{
private:
    
    // Alpha values used to control amount of reverb reduction
    float alpha1 = 0.8f;
    float alpha2 = 0.2f;
    
    // R1, R2 are the averaged power spectrum values
    float R1 = 0.0f;
    float R2 = 0.0f;
    float R1prev = 0.0f;
    float R2prev = 0.0f;
    
    // Masking gain
    float maskingGain = 0.0f;
    
    void setMaskingGain(float R1, float R2);
    
    
public:
    
    void setAlpha(float dereverbPercent);
    void setR1R2(float r1, float r2, float p);
    
    void processBlock(float *channelData, int numSamples);
    
};

#endif /* AdaptiveFilter_hpp */
