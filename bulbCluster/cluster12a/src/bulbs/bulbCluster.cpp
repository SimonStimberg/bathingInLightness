//
//  bulbCluster.cpp
//
//  Created by Simon Stimberg on 26.03.21
//
//

#include "bulbCluster.h"


// constructor
bulbCluster::bulbCluster(){
    noisePosition = 0.0;
    maxBrightness = 1.0;
}


void bulbCluster::setup(ofVec3f worldSize, int numBulbs){
    createRandomBulbCluster(worldSize, numBulbs);  
    bulbIntensities.resize(cluster.size());
    for(unsigned int i = 0; i < bulbIntensities.size(); i++) { 
        bulbIntensities[i] = 0.0f;
    }
}

void bulbCluster::setup(ofVec3f worldSize){
    initPhysicalCluster(worldSize);
    bulbIntensities.resize(cluster.size());
    for(unsigned int i = 0; i < bulbIntensities.size(); i++) { 
        bulbIntensities[i] = 0.0f;
    }
}





void bulbCluster::updateIntensities(const vector <ofPoint> & kinectPos, float kinectWeight, const vector <ofPoint> & flockPos, float flockWeight) {
        
    for(unsigned int i = 0; i < cluster.size(); i++){
        
        
        // compute the intensities regarding the individual weights
        float newIntensity = 0.0f;
        newIntensity += cluster[i].computeIntensity(kinectPos, kinectWeight);
        newIntensity += cluster[i].computeIntensity(flockPos, flockWeight);
        
        cluster[i].updateIntensity(newIntensity);  // update it


        // add some noise to make it more vivid (independently from the raw intensities)
        float intensity = cluster[i].getIntensity();
        float noise = ofNoise(i, noisePosition) * 0.33; // <- amplification factor for the noise           
        intensity = intensity - (noise * intensity); // before substracted from the intensity, the noise is multiplied by the intensity -> to have a stronger affect if the bulb is bright and not so much impact if its only glowing low

        intensity *= maxBrightness;
        bulbIntensities[i] = intensity;
    }

    noisePosition += 0.02; // speed of the noise     
}



vector<float> & bulbCluster::getIntensities() {

    return bulbIntensities;
}



void bulbCluster::draw() {
    for(unsigned int i = 0; i < cluster.size(); i++){
            // cluster[i].draw();
            // cluster[i].drawRadius();

            // draw with noise intensity
            ofSetColor((int)(round(bulbIntensities[i]*255)));
            ofDrawSphere(cluster[i].getPosition(), 10.0);
        }
}



vector<ofPoint> bulbCluster::getPositions() {
    vector<ofPoint> positions;

    for(unsigned int i = 0; i < cluster.size(); i++){
        positions.push_back(cluster[i].getPosition());
    }

    return positions;
}


void bulbCluster::setCheckRadius(int radius) {
    for(unsigned int i = 0; i < cluster.size(); i++){
        cluster[i].setCheckRadius(radius);
    }
}




//--------------------------------------------------------------
// creates a cluster of random positioned bulbs

// for testing / demonstration purpose when you have no physical cluster connected

void bulbCluster::createRandomBulbCluster(ofVec3f boxSize, int bulbNum) {
    
    // int bulbNum = 52;

    int clusterSize = boxSize.x;

    float scalingFactor = (clusterSize * 0.5 - bulbNum) / sqrt(bulbNum);;
    
    for(int i = 1; i < bulbNum+1; i++) {
        
        
        // DETERMINE X + Z POSITION
        // using the golden ratio (theta = 137.508 * n)
        // the so called "sunflower distribution"
        
        float r = scalingFactor * sqrt(i) + i;
        float angle = (137.508 * i) * (M_PI / 180);
        
        float x = r * sin(angle);
        float z = r * cos(angle);
        
        
        // CALCULATE Y VALUE
        // (this is a kind of dirty solution to receive an even distribution in form of a sphere
        // it uses noise and a decreasing offset from the center to the outside while swaping the shifting direction each iteration)
        
        // int yShift{0};
        // if (i % 2 == 0) {
        //     yShift = -bulbNum + 4 + i;
        // } else {
        //     yShift = bulbNum + 3 - i;
        // }
        
        // float y = boxSize * 0.5 + ofClamp((((bulbNum * 6 - i * 6) * ofNoise(i*1.1, 2.1) - (bulbNum * 3 - i * 3 )) + yShift), -bulbNum * 1.5, bulbNum * 1.5);
        

        // CALCULATE Y VALUE
        // after the paper solution

        // float rN = scalingFactor * sqrt(bulbNum) + bulbNum; 
        // float rN = clusterSize * 0.5; // maximum radius / radius for the last bulb
        float rN = boxSize.y * 0.5  -  (boxSize.y - boxSize.x) * 0.5 / bulbNum * i;
        float rV = sqrt( pow(rN, 2) - pow(r, 2) );  // vertical Radius

        float y = ofRandom(-rV, rV);


        cluster.push_back(bulb(ofPoint(x, y, z)));
    }
    
}





//--------------------------------------------------------------
// creates a cluster from an array of discrete position values

// x and z values are derived from the cluster construction
// the y positions have to be measured and manually typed into the array
// unit is millimeter

void bulbCluster::initPhysicalCluster(ofVec3f boxSize) {
    
    vector <float> positions = {
        281, 324, 0,   // Bulb No. 1
        304, 262, 1100,   // Bulb No. 2
        340, 347, 565,   // Bulb No. 3
        229, 287, 695,   // Bulb No. 4
        365, 262, 175,   // Bulb No. 5
        277, 385, 1000,   // Bulb No. 6
        256, 221, 350,   // Bulb No. 7
        400, 332, 1030,   // Bulb No. 8
        205, 346, 370,   // Bulb No. 9
        343, 197, 800,   // Bulb No. 10
        338, 409, 210,   // Bulb No. 11
        192, 237, 90,   // Bulb No. 12
        427, 272, 425,   // Bulb No. 13
        223, 410, 880,   // Bulb No. 14
        285, 167, 890,   // Bulb No. 15
        410, 393, 935,   // Bulb No. 16
        152, 306, 955,   // Bulb No. 17
        408, 192, 935,   // Bulb No. 18
        293, 457, 540,   // Bulb No. 19
        197, 176, 1010,   // Bulb No. 20
        463, 322, 760,   // Bulb No. 21
        161, 396, 760,   // Bulb No. 22
        343, 132, 440,   // Bulb No. 23
        388, 453, 765,   // Bulb No. 24
        129, 245, 785,   // Bulb No. 25
        467, 223, 860,   // Bulb No. 26
        228, 473, 190,   // Bulb No. 27
        238, 121, 180,   // Bulb No. 28
        472, 390, 140,   // Bulb No. 29
        109, 350, 605,   // Bulb No. 30
        408, 131, 725,   // Bulb No. 31
        334, 501, 420,   // Bulb No. 32
        137, 173, 475,   // Bulb No. 33
        509, 283, 645,   // Bulb No. 34
        155, 456, 615,   // Bulb No. 35
        301, 84, 635,   // Bulb No. 36
        447, 462, 305,   // Bulb No. 37
        79, 279, 360,   // Bulb No. 38
        479, 164, 295,   // Bulb No. 39
        269, 527, 735,   // Bulb No. 40
        177, 105, 725,   // Bulb No. 41
        525, 362, 425,   // Bulb No. 42
        90, 408, 240,   // Bulb No. 43
        383, 76, 260,   // Bulb No. 44
        401, 518, 580,   // Bulb No. 45
        77, 203, 640,   // Bulb No. 46
        533, 220, 485,   // Bulb No. 47
        174, 515, 435,   // Bulb No. 48
        242, 57, 520,   // Bulb No. 49
        507, 447, 670,   // Bulb No. 50
        48, 342, 475,   // Bulb No. 51
        468, 102, 575,   // Bulb No. 52
        
    };

    
    
    for(int i = 0; i < positions.size(); i += 3){
        
        float x = roundf(positions[i]   / 3 * 10) / 10 - boxSize.x * 0.5;
        float z = roundf(positions[i+1] / 3 * 10) / 10 - boxSize.z * 0.5;
        float y = roundf(positions[i+2] / 3 * 10) / 10 - boxSize.y * 0.5;
        
        cluster.push_back(bulb(ofPoint(x, y, z)));

    }
    
}