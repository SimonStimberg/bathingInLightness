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
        bulb(ofPoint initPosition, int thisId);
    
        // member functions
        float computeIntensity(const vector <ofPoint> & particlePositions, float weight);
        float updateIntensity(float newIntsty);
        void draw();
        void drawRadius();
        ofPoint & getPosition() { return pos; }
        void setCheckRaidus(float r) { checkRadius = r; }
        float getIntensity() { return intensity; };
    
        bool isEngaged;
    
    
    private:
    
        ofPoint pos;
        int id;
        float intensity;
        float checkRadius;
        float noisePosition;
        float noisyIntensity;
    
};