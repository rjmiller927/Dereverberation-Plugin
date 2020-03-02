//
//  AdaptiveFilter.cpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//



#include "AdaptiveFilter.hpp"

void AdaptiveFilter::setMu(float mu){
    this->mu = mu;
}

float AdaptiveFilter::getMu(){
    return mu;
}


void AdaptiveFilter::updateCoefficients(vector<float> &w,float mu,float e, vector<float> x){
    
}
