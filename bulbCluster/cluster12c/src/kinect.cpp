//
//  kinect.cpp
//  cluster01
//
//  Created by Simon Stimberg on 10.04.20.
//

#include "kinect.h"


// constructor
kinectHandler::kinectHandler(){

}

void kinectHandler::setup(ofVec3f size) {


    bufferFrameLength = 15;     // the "depth" of the buffer -> 15 frames
    bufferStepSize = 20;        // amount the resolution of the depth image is shrinked (-> 20px are skipped between points while readout)
    
 
    nearClip = 1000;     // set the near clipping plane in mm
    clippingDepth = 1500;   // set the clipping depth in mm (near clipping + depth = far clipping)



    ofSetLogLevel(OF_LOG_VERBOSE);
    
    for (int i = 0; i < numKinects; i++) {

        // ofSleepMillis(100);
        
        // enable depth->video image calibration
        kinect[i].setRegistration(true);
        
        // kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
        kinect[i].init(false, false); // disable video image (faster fps)

        // Which one is picked first does not depend on the bus position, but on the Serial Number
        if(i==0) {
            kinect[0].open("A00367A00066111A");
        } else {
            kinect[i].open();        // opens first available kinect
        }


        #ifdef SAFETY_MODE
            ofLogNotice("SAFETY MODE ON. Reconnecting Kinect...");

            ofSleepMillis(100);
            kinect.close();
            ofSleepMillis(3000);
            kinect.clear();
            ofSleepMillis(3000);
            kinect.setRegistration(true);
            kinect.init(false, false);
            kinect.open("A00365919812045A");
        #endif

        ofSleepMillis(100); 

        if(kinect[i].isDeviceConnected(0)) { ofLogNotice("device " + ofToString(i) + " connected!!!"); }

        ofLogNotice("Kinect Dimensions: " + ofToString(kinect[i].getWidth()) + "x" + ofToString(kinect[i].getHeight()));

        // // if (kinect.getCurrentCameraTiltAngle() != 30) { kinect.setCameraTiltAngle(30); }      
        ofLogNotice("Current Tilt Angle: " + ofToString(kinect[i].getCurrentCameraTiltAngle()));


        kinect[i].setDepthClipping(nearClip, nearClip + clippingDepth);
        
    }

    ofSetLogLevel(OF_LOG_NOTICE);
 

    
    worldSize = size;
    
    // Kinect resolution should be 640x480
    bufferWidth = kinect[0].getWidth() / bufferStepSize;
    bufferHeight = kinect[0].getHeight() / bufferStepSize;

    // allocate memory for the max amount of points
    if(kinect[0].isConnected()) { pointCloud.reserve(bufferWidth * bufferHeight * numKinects); }

    // create the flowBuffer
    int bufferSize = bufferWidth * bufferHeight * bufferFrameLength * numKinects;
    flowBuffer.assign(bufferSize, 0.0f);
    // initialize variables
    flow = 0.0f;
    bufferHead = 0;

    demoIterator = 0;
    
}



void kinectHandler::update() {
    
    for (int i = 0; i < numKinects; i++) {
        kinect[i].update();
    }

    if(kinect[0].isFrameNew()) {
        
        computePointCloud();

        // computeFlow();

    }
    
    // ofLogNotice("number of points: " + ofToString(pointCloud.size()));

}



void kinectHandler::computePointCloud() {

    pointCloud.clear();

    float scaleFactor = 0.2;

    for(int y = 0; y < (bufferHeight * bufferStepSize); y +=  bufferStepSize) {
        for(int x = 0; x < (bufferWidth * bufferStepSize); x +=  bufferStepSize) {
            for(int i = 0; i < numKinects; i++) {

                if(kinect[i].getDistanceAt(x, y) > nearClip && kinect[i].getDistanceAt(x, y) < (nearClip + clippingDepth) ) {
                    
                    ofPoint newPoint = kinect[i].getWorldCoordinateAt(x, y);
                    
                    newPoint.z -= nearClip;     // substract near Clipping Plane to match box boundaries
                    // if(i == 0) newPoint.x *= -1;           // mirror on X Axis  - only the first Kinect
                    newPoint.rotate(-45, 0, 0);            // rotate by tilt angle
                    if(i == 1) newPoint.z *= -1;           // mirror on Z Axis  - only the second Kinect
                    newPoint.z -= 200;
                    newPoint *= scaleFactor;    // resize to fit simulation
                    
                    // shift to match coordinates of other components
                    // newPoint.z += (i == 1) ? worldSize.z * 0.5 : -worldSize.z * 0.5;
                    // newPoint.x += worldSize*0.5;
                    // newPoint.y += worldSize*0.5;
                    
                    newPoint.y -= 150;       // added on location: trail and error to compensate shift
                    
                    // clip the the point if its outside the box
                    if (newPoint.x >= -worldSize.x*0.5 && newPoint.x <= worldSize.x*0.5  &&  newPoint.y >= -worldSize.y*0.5 && newPoint.y <= worldSize.y*0.5  &&  newPoint.z >= -worldSize.z*0.5 && newPoint.z <= worldSize.z*0.5) {
                        pointCloud.push_back(newPoint);
                    }
                    
    //                ofLogNotice("found point at: " + ofToString(newPoint));
                    
                }
            }
        }
    }

}



void kinectHandler::computeFlow() {

    int frameSize = bufferWidth * bufferHeight;
       
    float overallFlow = 0.0;


    for(int k = 0; k < numKinects; k++) {
        for(int y = 0; y < bufferHeight; y++) {
            for(int x = 0; x < bufferWidth; x++) {


                int kinectIndex = frameSize * bufferFrameLength * k;

                int frameIndex = x + (y * bufferWidth);

                // write the distance into the flowBuffer at the corresponding index in the corresponding frame
                int bufferIndex = frameIndex + bufferHead * frameSize + kinectIndex;
                flowBuffer[bufferIndex] = kinect[k].getDistanceAt(x * bufferStepSize, y * bufferStepSize);


                // compute the average flow ("amount of change") at the points position -> comparing the distance at all frames
                float pointFlow = 0.0f;

                for(int i = 0; i < bufferFrameLength; i++) {


                    float oldDepth = flowBuffer[frameIndex + i * frameSize + kinectIndex];
                    int j = (i < bufferFrameLength -1)  ?  (i + 1)  :  0;       // to make the wrap-around when comparing the last frame in the buffer with the first
                    float newDepth = flowBuffer[frameIndex + j * frameSize + kinectIndex];

                    float delta = abs(newDepth - oldDepth);
                    delta = (delta < 2700) ? delta : 0;     // ignore if the values jump to much (eg. from one end of the range to the other) -> its probably noise

                    pointFlow += delta;
                }

                pointFlow /= bufferFrameLength;     // getting the average
                overallFlow += pointFlow;           // sum it up

                // ofLogNotice("point flow: " + ofToString( pointFlow ));

            }

        }
    }

    overallFlow /= frameSize;   // get the average overall flow
    // overallFlow /= frameSize * numKinects;   // makes more sense but the values are to low??

    bufferHead = (bufferHead < bufferFrameLength - 1) ? bufferHead += 1 : 0;    // move the bufferhead to the next frame
  
    // normalize the flow and write it into the global variable
    flow = ofMap(overallFlow, 30, 130, 0.0, 1.0, true);     // the values are derived from testing and are arbitrary: around 30 was the noise level without movement / around 130 was the maximum peak at heavy and fast movements

    // ofLogNotice("overall flow: " + ofToString( flow ));

}


vector<ofPoint> & kinectHandler::getDemoPoints(bool bPlay) {
    
    pointCloud.clear();

    flow = 1.0;

    // vector <ofPoint> demoPoints;
    const int stepSize = 10;

    for(int z = -worldSize.z/2 + stepSize/2; z < worldSize.z/2; z+=stepSize) {
        for(int x = -worldSize.x/2 + stepSize/2; x < worldSize.x/2; x+=stepSize) {
            
            // int y = ( ofGetElapsedTimeMillis() / 32 ) % (int)worldSize.y - worldSize.y/2;
            int y = abs(demoIterator/2) % (int)worldSize.y - worldSize.y/2;

            pointCloud.push_back(ofPoint(x, y, z));
        }
    }

    demoIterator = (bPlay) ? demoIterator+1 : demoIterator;

    return pointCloud;
}


void kinectHandler::draw() {
    
    for(unsigned int i = 0; i < pointCloud.size(); i++) {
        // ofSetColor(255,0,0);
        ofDrawSphere(pointCloud[i], 2.0);
    }
    
//    kinect.drawDepth(0, 0);
    
}

void kinectHandler::drawCam(int x, int y) {

    for (int i = 0; i < numKinects; i++) {
        kinect[i].drawDepth(x + kinect[i].getWidth() * 0.5, y - kinect[i].getHeight() * 0.25 + (kinect[i].getHeight() * 0.5 + 10) * i, -kinect[i].getWidth() * 0.5, kinect[i].getHeight() * 0.5);
    }
    // kinect[0].drawDepth(x + kinect[0].getWidth() * 0.5, y - kinect[0].getHeight() * 0.25, -kinect[0].getWidth() * 0.5, kinect[0].getHeight() * 0.5);

}



vector<ofPoint> & kinectHandler::getPointCloud() {
    return pointCloud;
}


void kinectHandler::switchLEDon(bool switcher) {
    if(switcher) {
        for (int i = 0; i < numKinects; i++) {
            kinect[i].setLed(ofxKinect::LED_DEFAULT);
        }
    } else {
        for (int i = 0; i < numKinects; i++) {
            kinect[i].setLed(ofxKinect::LED_OFF);
        }
    }    
}


void kinectHandler::exit() {
	// kinect.setCameraTiltAngle(0); // zero the tilt on exit
    for (int i = 0; i < numKinects; i++) {
        kinect[i].close();
    }
	
}