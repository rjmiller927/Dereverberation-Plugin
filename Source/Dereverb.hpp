//
//  AdaptiveFilter.hpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//



#ifndef Dereverb_hpp
#define Dereverb_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Dereverb{
private:
    float rt60 = 1.f;
    
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
    
    
};

#endif /* AdaptiveFilter_hpp */
