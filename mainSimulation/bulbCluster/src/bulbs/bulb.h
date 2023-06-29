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
        float computeIntensity(const vector <ofPoint> & particlePositions, float weight);
        void updateIntensity(float newIntsty);
        void draw();
        void drawRadius();
        ofPoint getPosition() { return pos; }
        void setCheckRadius(float r) { checkRadius = r; }
        float getIntensity() { return intensity; };
    
    
    private:
    
        ofPoint pos;
        float intensity;
        float checkRadius;
    
};