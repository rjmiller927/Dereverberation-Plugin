//
//  AdaptiveFilter.cpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//
//




#include "Dereverb.hpp"

void Dereverb::setAlpha(float dereverbPercent){
    
    // Normalize the de-reverb percentage to scale of 0-1
    float normPercent = dereverbPercent / 100.f;
    
    // Update alpha1 and alpha2. NOTE:
    //  o alpha1 + alpha2 == 1.0
    //  o alpha1 = [0.0, 0.5]
    //  o alpha2 = [0.5, 1.0]
    alpha2 = 0.5f + normPercent*0.49f;
    alpha1 = 1.f - alpha2;
    
}

void Dereverb::setR1R2(float r1, float r2, float p){
    
    R1 = (1.f-alpha1)*p + alpha1*R1prev;
    R2 = (1.f-alpha2)*p + alpha2*R2prev;
    
    R1prev = R1;
    R2prev = R2;
    
    // Set new masking gain after updating R1, R2
    setMaskingGain(R1, R2);
    
}

void Dereverb::setMaskingGain(float R1, float R2){
    
    if ((R1 / R2) >= 1){
        maskingGain = 0.f;
    }
    else{
        if (R2 == 0.f){
            maskingGain = 0.f;
        }
        else{
            maskingGain = R1 / R2;
        }
    }
    
}
