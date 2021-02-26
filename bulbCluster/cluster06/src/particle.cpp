//
// based on the OpenFrameworks example project "particlesExample"
// found in the folder "/examples/math" inside the OF root folder
//


#include "particle.h"

//------------------------------------------------------------------
particle::particle(){
    attractPoints = NULL;
}

//------------------------------------------------------------------
void particle::setMode(particleMode newMode){
    mode = newMode;
}

void particle::setAttractor(ofPoint * attractor){
    singleAttractor = attractor;
}


//------------------------------------------------------------------
void particle::reset(const int& containerSize){
    
    //the unique val allows us to set properties slightly differently for each particle
    uniqueVal = ofRandom(-10000, 10000);
    
    pos.x = ofRandom(containerSize);
    pos.y = ofRandom(containerSize);
    pos.z = ofRandom(containerSize);
    
    vel.x = ofRandom(-3.9, 3.9);
    vel.y = ofRandom(-3.9, 3.9);
    vel.z = ofRandom(-3.9, 3.9);
    
    frc   = ofPoint(0,0,0); // frc means "force"
    
    scale = ofRandom(1.5, 3.0);
    
    drag  = ofRandom(0.90, 0.958);
    
}



//------------------------------------------------------------------
void particle::update(const int& containerSize){
    
    //1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
    

    float dragMod, frcFactor1, frcFactor2 = 0.0;

    if( mode == POSITIVE ){
        dragMod = drag - 0.01;
        frcFactor1 = 0.6;
        frcFactor2 = 0.3;
    } else if (mode == NEGATIVE) {
        dragMod = drag + 0.04;
        frcFactor1 = 0.2;
        frcFactor2 = 0.1;
    }


        
    frc = *singleAttractor-pos; // we get the attraction force/vector by looking at the ATTRACTOR pos relative to our pos
    float dist = frc.length();
    frc.normalize(); // by normalizing we disregard how close the particle is to the attraction point
    
    vel *= dragMod; //apply drag
    
    if( dist < 400 ){
        vel += frc * frcFactor1; //apply force
    } else {
        
        //if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
        frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
        frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
        frc.z = ofSignedNoise(uniqueVal, pos.z * 0.01, ofGetElapsedTimef()*0.2);
        vel += frc * frcFactor2;
    }
        
    
    
    //2 - UPDATE OUR POSITION
    
    pos += vel;
    
    
    //3 - LIMIT THE PARTICLES TO STAY INSIDE THE BOUNDING BOX

    float halfContainer = containerSize * 0.5;

    if( pos.x > halfContainer ){
        pos.x = halfContainer;
        vel.x *= -1.0;
    }else if( pos.x < -halfContainer ){
        pos.x = -halfContainer;
        vel.x *= -1.0;
    }
    if( pos.y > halfContainer ){
        pos.y = halfContainer;
        vel.y *= -1.0;
    }
    else if( pos.y < -halfContainer ){
        pos.y = -halfContainer;
        vel.y *= -1.0;
    }
    if( pos.z > halfContainer ){
        pos.z = halfContainer;
        vel.z *= -1.0;
    }
    else if( pos.z < -halfContainer ){
        pos.z = -halfContainer;
        vel.z *= -1.0;
    }
    
}



//------------------------------------------------------------------
void particle::draw(){
    
    if( mode == POSITIVE ){        
        ofSetColor(99, 63, 255);
    }
    else if( mode == NEUTRAL ){
        ofSetColor(255, 63, 180);
    }
    else if( mode == NEGATIVE ){
        ofSetColor(208, 255, 63);
    }
    
    ofDrawSphere(pos.x, pos.y, pos.z, scale * 4.0);
}

