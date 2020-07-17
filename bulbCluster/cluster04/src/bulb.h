//
//  bulb.hpp
//  particles3D
//
//  Created by Simon Stimberg on 25.06.19.
//
//

#pragma once

#include "ofMain.h"


class bulb{
    
    public:
    
        // constructor
        bulb();
        bulb(ofPoint initPosition);
    
        // member functions
        float update(const vector <ofPoint> & particlePositions);
        void draw();
    
        bool isEngaged;
    
    
    private:
    
        ofPoint pos;
        float intensity;
    
};