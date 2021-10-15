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
    
 
    nearClip = 600;     // set the near clipping plane in mm
    clippingDepth = 1000;   // set the clipping depth in mm (near clipping + depth = far clipping)


    ofSetLogLevel(OF_LOG_VERBOSE);
        

        // ofSleepMillis(100);
        
        // enable depth->video image calibration
        kinect.setRegistration(true);
        
        // kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
        kinect.init(false, false); // disable video image (faster fps)

        kinect.open();        // opens first available kinect


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





        kinect.setDepthClipping(nearClip, nearClip + clippingDepth);
  
        ofLogNotice("Kinect Dimensions: " + ofToString(kinect.getWidth()) + "x" + ofToString(kinect.getHeight()));

        // if (kinect.getCurrentCameraTiltAngle() != 30) { kinect.setCameraTiltAngle(30); }
        
        ofLogNotice("Current Tilt Angle: " + ofToString(kinect.getCurrentCameraTiltAngle()));


    ofSetLogLevel(OF_LOG_NOTICE);

    if(kinect.isDeviceConnected(0)) { ofLogNotice("dev connected!!!"); }

    

    
    worldSize = size;
    
    // Kinect resolution should be 640x480
    bufferWidth = kinect.getWidth() / bufferStepSize;
    bufferHeight = kinect.getHeight() / bufferStepSize;

    // allocate memory for the max amount of points
    if(kinect.isConnected()) { pointCloud.reserve(bufferWidth * bufferHeight); }

    // create the flowBuffer
    int bufferSize = bufferWidth * bufferHeight * bufferFrameLength;
    flowBuffer.assign(bufferSize, 0.0f);
    // initialize variables
    flow = 0.0f;
    bufferHead = 0;
    
}



void kinectHandler::update() {
    
    kinect.update();
    
    if(kinect.isFrameNew()) {
        
        computePointCloud();

        computeFlow();

    }
    
    // ofLogNotice("number of points: " + ofToString(pointCloud.size()));

}



void kinectHandler::computePointCloud() {

    pointCloud.clear();

    float scaleFactor = 0.2;

    for(int y = 0; y < (bufferHeight * bufferStepSize); y +=  bufferStepSize) {
        for(int x = 0; x < (bufferWidth * bufferStepSize); x +=  bufferStepSize) {
            if(kinect.getDistanceAt(x, y) > nearClip && kinect.getDistanceAt(x, y) < (nearClip + clippingDepth) ) {
                
                ofPoint newPoint = kinect.getWorldCoordinateAt(x, y);
                
                newPoint.z -= nearClip;     // substract near Clipping Plane to match box boundaries
                newPoint.x *= -1;           // mirror on X Axis
                newPoint.z *= -1;           // mirror on Z Axis
                newPoint *= scaleFactor;    // resize to fit simulation
                
                // shift to match coordinates of other components
                newPoint.z += worldSize.z * 0.5;
                // newPoint.x += worldSize*0.5;
                // newPoint.y += worldSize*0.5;

                // if the camera is tilted, rotate the point by the the tilt angle
                // newPoint.rotate(49, 0, 0);
                // newPoint.y -= 25;       // added on location: trail and error to compensate shift
                
                // clip the the point if its outside the box
                if (newPoint.x >= -worldSize.x*0.5 && newPoint.x <= worldSize.x*0.5  &&  newPoint.y >= -worldSize.y*0.5 && newPoint.y <= worldSize.y*0.5  &&  newPoint.z >= -worldSize.z*0.5 && newPoint.z <= worldSize.z*0.5) {
                    pointCloud.push_back(newPoint);
                }
                
//                ofLogNotice("found point at: " + ofToString(newPoint));
                
            }
        }
    }

}



void kinectHandler::computeFlow() {

    int frameSize = bufferWidth * bufferHeight;
       
    float overallFlow = 0.0;


    for(int y = 0; y < bufferHeight; y++) {
        for(int x = 0; x < bufferWidth; x++) {

            int frameIndex = x + (y * bufferWidth);

            // write the distance into the flowBuffer at the corresponding index in the corresponding frame
            int bufferIndex = frameIndex + bufferHead * frameSize;
            flowBuffer[bufferIndex] = kinect.getDistanceAt(x * bufferStepSize, y * bufferStepSize);


            // compute the average flow ("amount of change") at the points position -> comparing the distance at all frames
            float pointFlow = 0.0f;

            for(int i = 0; i < bufferFrameLength; i++) {


                float oldDepth = flowBuffer[frameIndex + i * frameSize];
                int j = (i < bufferFrameLength -1)  ?  (i + 1)  :  0;       // to make the wrap-around when comparing the last frame in the buffer with the first
                float newDepth = flowBuffer[frameIndex + j * frameSize];

                float delta = abs(newDepth - oldDepth);
                delta = (delta < 2700) ? delta : 0;     // ignore if the values jump to much (eg. from one end of the range to the other) - its probably noise

                pointFlow += delta;
            }

            pointFlow /= bufferFrameLength;     // getting the average
            overallFlow += pointFlow;           // sum it up

        }
    }

    overallFlow /= frameSize;   // get the average overall flow

    bufferHead = (bufferHead < bufferFrameLength - 1) ? bufferHead += 1 : 0;    // move the bufferhead to the next frame
  
    // normalize the flow and write it into the global variable
    flow = ofMap(overallFlow, 30, 130, 0.0, 1.0, true);     // the values are derived from testing and arbitrary: around 30 was the noise level without movement / around 130 was the maximum peak at heavy and fast movements

    ofLogNotice("overall flow: " + ofToString( flow ));

}



void kinectHandler::draw() {
    
    for(unsigned int i = 0; i < pointCloud.size(); i++) {
        // ofSetColor(255,0,0);
        ofDrawSphere(pointCloud[i], 2.0);
    }
    
//    kinect.drawDepth(0, 0);
    
}

void kinectHandler::drawCam(int x, int y) {

    kinect.drawDepth(x + kinect.getWidth() * 0.5, y - kinect.getHeight() * 0.25, -kinect.getWidth() * 0.5, kinect.getHeight() * 0.5);

}



vector<ofPoint> & kinectHandler::getPointCloud() {
    return pointCloud;
}


void kinectHandler::switchLEDon(bool switcher) {
    if(switcher) {
        kinect.setLed(ofxKinect::LED_DEFAULT);
    } else {
        kinect.setLed(ofxKinect::LED_OFF);
    }    
}


void kinectHandler::exit() {
	// kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}