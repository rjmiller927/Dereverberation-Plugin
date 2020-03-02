//
//  AdaptiveFilter.cpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//



#include "Dereverb.hpp"

void Dereverb::setMu(float mu){
    this->mu = mu;
}

float Dereverb::getMu(){
    return mu;
}


void Dereverb::updateCoefficients(vector<float> &w,float mu,float e, vector<float> x){
    
}
