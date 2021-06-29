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

    
    // allocate memory for a certain amount of points
    if(kinect.isConnected()) { pointCloud.resize(1000); }
    
    worldSize = size;
    
    // should be 640x480
    w = kinect.getWidth();
    h = kinect.getHeight();
    
}



void kinectHandler::update() {
    
    kinect.update();
    
    if(kinect.isFrameNew()) {
        
        pointCloud.clear();

        int step = 20;
        float scaleFactor = 0.2;
        
        for(int y = 0; y < h; y += step) {
            for(int x = 0; x < w; x += step) {
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
                    newPoint.rotate(49, 0, 0);
                    newPoint.y -= 25;       // added on location: trail and error to compensate shift
                    
                    // clip the the point if its outside the box
                    if (newPoint.x >= -worldSize.x*0.5 && newPoint.x <= worldSize.x*0.5  &&  newPoint.y >= -worldSize.y*0.5 && newPoint.y <= worldSize.y*0.5  &&  newPoint.z >= -worldSize.z*0.5 && newPoint.z <= worldSize.z*0.5) {
                        pointCloud.push_back(newPoint);
                    }
                    
    //                ofLogNotice("found point at: " + ofToString(newPoint));
                    
                }
            }
        }
    }
    
    // ofLogNotice("number of points: " + ofToString(pointCloud.size()));

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