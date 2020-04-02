//
//  AdaptiveFilter.cpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//
//


#include "Dereverb.hpp"

using namespace std;

void Dereverb::processBlock(float *fftChannelData, int numSamples){
    
    
    for (int i = 0; i < numSamples; i+=2){
        
        // Take the magnitude squared of the FFT input
        P = sqrtf(pow(fftChannelData[i],2) + pow(fftChannelData[i+1],2));
        
        // Calculate R1 and R2. 'setR1R2' also updates 'maskingGain'
        setR1R2(P);
        
        // Apply masking gain to real and imaginary parts
        fftChannelData[i] *= maskingGain;
        fftChannelData[i+1] *= maskingGain;
        
    }
}

void Dereverb::setAlpha(float dereverbPercent){
    
    // Normalize the de-reverb percentage to scale of 0-1
    float normPercent = dereverbPercent / 100.f;
    
    // Update alpha1 and alpha2. NOTE:
    //  o alpha1 + alpha2 ~= 1.0
    //  o alpha1 = [0.0, 0.49]
    //  o alpha2 = [0.5, 0.99]
    alpha2 = 0.5f + normPercent*0.49f;
    alpha1 = 1.f - alpha2;
    
}

void Dereverb::setR1R2(float inputPower){
    
    R1 = (1.f-alpha1)*inputPower + alpha1*R1prev;
    R2 = (1.f-alpha2)*inputPower + alpha2*R2prev;
    
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
        // Check if R2 is <= 0 to avoid division by 0 or negative values
        // float epsilon = numeric_limits<double>::epsilon();
        if (R2 <= 0.f){
            maskingGain = 0.f;
        }
        else{
            maskingGain = R1 / R2;
        }
    }
    
}
