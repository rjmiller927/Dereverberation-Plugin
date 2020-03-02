//
//  AdaptiveFilter.hpp
//  De-Reverb - AU
//
//  Created by Ryan Miller on 2/8/20.
//



#ifndef AdaptiveFilter_hpp
#define AdaptiveFilter_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class AdaptiveFilter{
private:
    vector<float> w;    // Adaptive Filter Coefficients
    float mu;           // Learning rate
    float e;            // Error between filter output and desired signal
    int M;              // Filter order
    
    
public:
    float getMu();
    void setMu(float mu);
    void updateCoefficients(vector<float> &w,float mu,float e, vector<float> x);
    
};

#endif /* AdaptiveFilter_hpp */
