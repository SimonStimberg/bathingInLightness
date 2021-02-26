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


bulb::bulb(ofPoint initPosition){
    pos = initPosition;
    intensity = 0.0;
    isEngaged = false;

    scanRadius = 70.0;
    weightIntensity = 0.4;
}





float bulb::update(const vector <ofPoint> & particlePositions) {
    
    float newIntensity = 0.0;

    
    // iterate over the positions of all particles and compute bulb intensity according to the distance
    
    for(unsigned int i = 0; i < particlePositions.size(); i++){

        // calculate distance
        ofPoint delta = pos - particlePositions[i];
        float dist = delta.length();
        
        // if particle in check-radius: normalize distance and add it to intensity
        if(dist < scanRadius ) {
            
            dist = ofMap(dist, 0., scanRadius, weightIntensity, 0.01);
            newIntensity += dist;
        }
        
    }
    

    if(isEngaged) {
        newIntensity = ofClamp(newIntensity, 0., 1.);
    } else {
        newIntensity = ofClamp(newIntensity, 0., 1.);
    }
    
    // newIntensity = ofClamp(newIntensity, 0., 1.);
    
    
    // smooth values, by only changing a bit
    intensity = intensity * 0.9 + newIntensity * 0.1;
    
    
    // if in NEGATIVE mood let the bulbs flicker a bit to evoke angryness

    
    
    return intensity;
    
}





void bulb::draw(){
    
    intensity = ofClamp(intensity, 0., 1.); // for safety

    // draw bulb with its brightness according to the intensity 
    ofSetColor((int)(round(intensity*255)));
    ofDrawSphere(pos, 8.0);
    
}


void bulb::drawCable(const int & worldSize) {

    ofSetColor(0);
    ofSetLineWidth(2.35);      
    ofDrawLine(pos, ofPoint(pos.x, -worldSize * 0.5, pos.z));
    ofSetLineWidth(1);   
    ofDrawCylinder(pos.x, pos.y - 14, pos.z, 7, 16);

}
