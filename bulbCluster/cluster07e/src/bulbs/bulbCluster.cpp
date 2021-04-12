//
//  bulbCluster.cpp
//
//  Created by Simon Stimberg on 26.03.21
//
//

#include "bulbCluster.h"


// constructor
bulbCluster::bulbCluster(){
    
}


void bulbCluster::setup(int worldSize, int numBulbs){
    createRandomBulbCluster(worldSize, numBulbs);  
    bulbIntensities.resize(cluster.size());
}

void bulbCluster::setup(int worldSize){
    initPhysicalCluster(worldSize);
    bulbIntensities.resize(cluster.size());
}





void bulbCluster::updateIntensities(const vector <ofPoint> & kinectPos, float kinectWeight, const vector <ofPoint> & flockPos, float flockWeight) {
        
    for(unsigned int i = 0; i < cluster.size(); i++){
        
        // cluster[i].computeIntensity(particlePositions, weight);

        float newIntensity = 0.0f;
        newIntensity += cluster[i].computeIntensity(kinectPos, kinectWeight);
        newIntensity += cluster[i].computeIntensity(flockPos, flockWeight);
        
        cluster[i].updateIntensity(newIntensity);

        bulbIntensities[i] = cluster[i].getIntensity();
    }
}



vector<float> & bulbCluster::getIntensities() {

    return bulbIntensities;
}



void bulbCluster::draw() {
    for(unsigned int i = 0; i < cluster.size(); i++){
            cluster[i].draw();
            // cluster[i].drawRadius();
        }
}



vector<ofPoint> bulbCluster::getPositions() {
    vector<ofPoint> positions;

    for(unsigned int i = 0; i < cluster.size(); i++){
        positions.push_back(cluster[i].getPosition());
    }

    return positions;
}




//--------------------------------------------------------------
// creates a cluster of random positioned bulbs

// for testing / demonstration purpose when you have no physical cluster connected

void bulbCluster::createRandomBulbCluster(int boxSize, int bulbNum) {
    
    // int bulbNum = 52;

    int clusterSize = boxSize;

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
        float rN = clusterSize * 0.5; // maximum radius / radius for the last bulb
        float rV = sqrt( pow(rN, 2) - pow(r, 2) );  // vertical Radius

        float y = ofRandom(-rV, rV);


        cluster.push_back(bulb(ofPoint(x, y, z), i));
    }
    
}





//--------------------------------------------------------------
// creates a cluster from an array of discrete position values

// x and y values are derived from the cluster construction
// the y positions have to be measured and manually typed into the array
// unit is millimeter

void bulbCluster::initPhysicalCluster(int boxSize) {
    
    // abrennec:
    // std::map<key,value> oder std::vector<glm::vec3f>
    // koenntest du dir mal anschauen, nur zur Info.
    vector <float> positions = {
        281, 324, 0,   // Bulb No. 1
        304, 262, 370,   // Bulb No. 2
        340, 347, 400,   // Bulb No. 3
        229, 287, 75,   // Bulb No. 4
        365, 262, 570,   // Bulb No. 5
        277, 385, 580,   // Bulb No. 6
        256, 221, 600,   // Bulb No. 7
        400, 332, 140,   // Bulb No. 8
        205, 346, 280,   // Bulb No. 9
        343, 197, 190,   // Bulb No. 10
        338, 409, 400,   // Bulb No. 11
        192, 237, 445,   // Bulb No. 12
        427, 272, 60,   // Bulb No. 13
        223, 410, 455,   // Bulb No. 14
        285, 167, 80,   // Bulb No. 15
        410, 393, 540,   // Bulb No. 16
        152, 306, 180,   // Bulb No. 17
        408, 192, 600,   // Bulb No. 18
        293, 457, 240,   // Bulb No. 19
        197, 176, 330,   // Bulb No. 20
        463, 322, 375,   // Bulb No. 21
        161, 396, 225,   // Bulb No. 22
        343, 132, 480,   // Bulb No. 23
        388, 453, 205,   // Bulb No. 24
        129, 245, 535,   // Bulb No. 25
        467, 223, 340,   // Bulb No. 26
        228, 473, 35,   // Bulb No. 27
        238, 121, 505,   // Bulb No. 28
        472, 390, 140,   // Bulb No. 29
        109, 350, 495,   // Bulb No. 30
        408, 131, 345,   // Bulb No. 31
        334, 501, 450,   // Bulb No. 32
        137, 173, 135,   // Bulb No. 33
        509, 283, 560,   // Bulb No. 34
        155, 456, 420,   // Bulb No. 35
        301, 84, 305,   // Bulb No. 36
        447, 462, 275,   // Bulb No. 37
        79, 279, 380,   // Bulb No. 38
        479, 164, 130,   // Bulb No. 39
        269, 527, 520,   // Bulb No. 40
        177, 105, 390,   // Bulb No. 41
        525, 362, 490,   // Bulb No. 42
        90, 408, 410,   // Bulb No. 43
        383, 76, 180,   // Bulb No. 44
        401, 518, 285,   // Bulb No. 45
        77, 203, 250,   // Bulb No. 46
        533, 220, 460,   // Bulb No. 47
        174, 515, 270,   // Bulb No. 48
        242, 57, 165,   // Bulb No. 49
        507, 447, 315,   // Bulb No. 50
        48, 342, 275,   // Bulb No. 51
        468, 102, 275   // Bulb No. 52
        
    };

    
    
    for(int i = 0; i < positions.size(); i += 3){
        
        float x = roundf(positions[i]   / 3 * 10) / 10 - boxSize * 0.5;
        float z = roundf(positions[i+1] / 3 * 10) / 10 - boxSize * 0.5;
        float y = roundf(positions[i+2] / 3 * 10) / 10 - boxSize * 0.5;
        
        cluster.push_back(bulb(ofPoint(x, y, z), i));

    }
    
}