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
}





float bulb::update(const vector <ofPoint> & particlePositions) {
    
    float newIntensity = 0.0;

    
    // iterate over the positions of all particles and compute bulb intensity according to the distance
    
    for(unsigned int i = 0; i < particlePositions.size(); i++){

        // calculate distance
        ofPoint delta = pos - particlePositions[i];
        float dist = delta.length();
        
        // if particle in check-radius: normalize distance and add it to intensity
        if(dist < 50.0 ) {
            
            dist = ofMap(dist, 0., 50., 0.8, 0.01);
            newIntensity += dist;
        }
        
    }
    

    if(isEngaged) {
        newIntensity = ofClamp(newIntensity, 0., 1.);
    } else {
        newIntensity = ofClamp(newIntensity, 0., 0.8);
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
