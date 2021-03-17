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
        void drawRadius();
        ofPoint & getPosition() { return pos; }
        void setCheckRaidus(float r) { checkRadius = r; }
    
        bool isEngaged;
    
    
    private:
    
        ofPoint pos;
        float intensity;
        float checkRadius;
    
};