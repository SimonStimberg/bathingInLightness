//
//  bulb.cpp
//  particles3D
//
//  Created by Simon Stimberg on 25.06.19.
//
//

#include "bulb.h"



bulb::bulb(){
    
}


bulb::bulb(ofPoint initPosition, int thisId){
    pos = initPosition;
    id = thisId;
    intensity = 0.0;
    isEngaged = false;
    checkRadius = 40.0;
    noisePosition = 0.0;
}





float bulb::computeIntensity(const vector <ofPoint> & particlePositions, float weight) {
    
    float newIntensity = 0.0;

    
    // iterate over the positions of all particles and compute bulb intensity according to the distance
    
    for(unsigned int i = 0; i < particlePositions.size(); i++){

        // calculate distance
        ofPoint delta = pos - particlePositions[i];
        float dist = delta.length();
        
        // if particle in check-radius: normalize distance and add it to intensity
        if(dist < checkRadius) {
            
            dist = ofMap(dist, 0., checkRadius, weight, 0.01);
            newIntensity += dist;
        }
        
    }

    newIntensity = ofClamp(newIntensity, 0., weight);

    return newIntensity;
    
}


float bulb::updateIntensity(float newIntsty) {
    
    float newIntensity = ofClamp(newIntsty, 0., 1.0);
        
    // smooth values, by only changing a bit
    intensity = intensity * 0.85 + newIntensity * 0.15;


    float noise = ofNoise(id, noisePosition) * 0.3; // <- amplification factor for the noise
    noisePosition += 0.01; // speed of the noise
    
    noisyIntensity = intensity - (noise * intensity); // before substracted from the intensity, the noise is multiplied by the intensity -> to have a stronger affect if the bulb is bright and not so much impact if its only glowing low


    return noisyIntensity;
}


void bulb::draw(){
    
    intensity = ofClamp(intensity, 0., 1.); // for safety

    // draw bulb with its brightness according to the intensity
    ofSetColor((int)(round(noisyIntensity*255)));
    ofDrawSphere(pos, 10.0);
    
}


void bulb::drawRadius() {

    ofSetColor(200, 0, 0, 10);
    ofDrawSphere(pos, checkRadius);

}