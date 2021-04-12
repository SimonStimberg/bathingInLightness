//
//  bulbCluster.h
//
//  Created by Simon Stimberg on 26.03.21
//
//

#pragma once

#include "ofMain.h"
#include "bulb.h"


class bulbCluster{
    
    public:
    
        // constructor
        bulbCluster();
        // bulb(ofPoint initPosition, int thisId);
    
        // member functions
        void setup(ofVec3f worldSize, int numBulbs);
        void setup(ofVec3f worldSize);
        void updateIntensities(const vector <ofPoint> & kinectPos, float kinectWeight, const vector <ofPoint> & flockPos, float flockWeight);
        vector <float> & getIntensities();
        vector<ofPoint> getPositions();
        void draw();        

        void createRandomBulbCluster(ofVec3f boxSize, int bulbNum);
        void initPhysicalCluster(ofVec3f boxSize);
        
    
    private:

        vector <bulb> cluster;
        vector <float> bulbIntensities;
            
};