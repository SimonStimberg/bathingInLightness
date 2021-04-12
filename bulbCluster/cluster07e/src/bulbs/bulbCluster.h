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
        void setup(int worldSize, int numBulbs);
        void setup(int worldSize);
        void updateIntensities(const vector <ofPoint> & kinectPos, float kinectWeight, const vector <ofPoint> & flockPos, float flockWeight);
        vector <float> & getIntensities();
        vector<ofPoint> getPositions();
        void draw();        

        void createRandomBulbCluster(int boxSize, int bulbNum);
        void initPhysicalCluster(int boxSize);
        
    
    private:

        vector <bulb> cluster;
        vector <float> bulbIntensities;
            
};