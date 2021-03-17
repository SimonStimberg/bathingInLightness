//
//  kinect.cpp
//  cluster01
//
//  Created by Simon Stimberg on 10.04.20.
//

#include "kinect.h"

ofEasyCam cam;

// constructor
kinectHandler::kinectHandler(){

}

void kinectHandler::setup(int size) {
    
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
        // enable depth->video image calibration
//        kinect.setRegistration(true);
        
    //    kinect.init();
        //kinect.init(true); // shows infrared instead of RGB video image
//        kinect.init(false, false); // disable video image (faster fps)
//
//        kinect.open();        // opens first available kinect
//
//        kinect.setDepthClipping(500, 1500);
//
//        ofLogNotice("Kinect Dimensions: " + ofToString(kinect.getWidth()) + "x" + ofToString(kinect.getHeight()));
    
        // the NI device initialization
        device.setLogLevel(OF_LOG_NOTICE);
        device.setup(0);
        tracker.setup(device);

    ofSetLogLevel(OF_LOG_NOTICE);

    
    // allocate memory for a certain amount of points
//    if(device.isConnected()) { pointCloud.resize(1000); }
    
    worldSize = size;
    
    // should be 640x480
//    w = kinect.getWidth();
//    h = kinect.getHeight();
    
}


void kinectHandler::exit()
{
    tracker.exit();
    device.exit();
}



void kinectHandler::update() {
    
//    kinect.update();
    device.update();
  
    
    // NEW CODE
    depthPixels = tracker.getPixelsRef(1000, 4000);
    depthTexture.loadData(depthPixels);


//    if(device.isFrameNew()) {

        pointCloud.clear();

        for (int i = 0; i < tracker.getNumUser(); i++)
         {
             ofxNiTE2::User::Ref user = tracker.getUser(i);

             const ofxNiTE2::Joint &jointLH = user->getJoint(nite::JOINT_LEFT_HAND);
             const ofxNiTE2::Joint &jointRH = user->getJoint(nite::JOINT_RIGHT_HAND);

//             pointCloud.push_back(jointRH.getGlobalPosition());
//             pointCloud.push_back(jointLH.getGlobalPosition());
            

             ofPoint newPoint = jointLH.getGlobalPosition();

             newPoint.x = ofMap(newPoint.x, 500, -500, -worldSize*0.5, worldSize*0.5);
             newPoint.y = ofMap(newPoint.y, 500, -500, -worldSize*0.5, worldSize*0.5);
             newPoint.z = ofMap(newPoint.z, -500, -1500, -worldSize*0.5, worldSize*0.5);

             pointCloud.push_back(newPoint);
             
             
             newPoint = jointRH.getGlobalPosition();

             newPoint.x = ofMap(newPoint.x, 500, -500, -worldSize*0.5, worldSize*0.5);
             newPoint.y = ofMap(newPoint.y, 500, -500, -worldSize*0.5, worldSize*0.5);
             newPoint.z = ofMap(newPoint.z, -500, -1500, worldSize*0.5, -worldSize*0.5);

             pointCloud.push_back(newPoint);
    //         joint.transformGL();


             ofPoint myHand = jointLH.getGlobalPosition();
             ofLogNotice(ofToString(pointCloud[0]));

    //         ofDrawBox(300);
    //         joint.restoreTransformGL();
         }
//    }
    
    
//    if(kinect.isFrameNew()) {
//        
//        pointCloud.clear();
//
//        int step = 20;
//        int nearClip = 500;
//        int farClip = nearClip + 1000;
//        float scaleFactor = 0.2;
//        
//        for(int y = 0; y < h; y += step) {
//            for(int x = 0; x < w; x += step) {
//                if(kinect.getDistanceAt(x, y) > nearClip && kinect.getDistanceAt(x, y) < farClip ) {
//                    
//                    ofPoint newPoint = kinect.getWorldCoordinateAt(x, y);
//                    
//                    newPoint.z -= nearClip;     // substract near Clipping Plane to match box boundaries
//                    newPoint.x *= -1;           // mirror on X Axis
//                    newPoint.z *= -1;           // mirror on Z Axis
//                    newPoint *= scaleFactor;    // resize to fit simulation
//                    
//                    // shift to match coordinates of other components
//                    newPoint.z += worldSize * 0.5;
//                    // newPoint.x += worldSize*0.5;
//                    // newPoint.y += worldSize*0.5;
//                    
//                    if (newPoint.x >= -worldSize*0.5 && newPoint.x <= worldSize*0.5) {
//                        pointCloud.push_back(newPoint);
//                    }
//                    
//    //                ofLogNotice("found point at: " + ofToString(newPoint));
//                    
//                }
//            }
//        }
//    }
    
    // ofLogNotice("number of points: " + ofToString(pointCloud.size()));

}



void kinectHandler::draw() {
    
    for(unsigned int i = 0; i < pointCloud.size(); i++) {
         ofSetColor(255,0,0);
        ofDrawSphere(pointCloud[i], 2.0);
    }
    
//    kinect.drawDepth(0, 0);
    
}

void kinectHandler::drawCam(int x, int y) {
    
//    depthPixels = tracker.getPixelsRef(1000, 4000);
//    depthTexture.loadData(depthPixels);

    // draw in 2D
    ofSetColor(255);
//    depthTexture.draw(0, 0);
    depthTexture.draw(x + depthTexture.getWidth() * 0.5, y - depthTexture.getHeight() * 0.25, -depthTexture.getWidth() * 0.5, depthTexture.getHeight() * 0.5);
////    kinect.drawDepth(x + kinect.getWidth() * 0.5, y - kinect.getHeight() * 0.25, -kinect.getWidth() * 0.5, kinect.getHeight() * 0.5);
//    
//    tracker.draw();
    
//    cam.begin();
//    ofDrawAxis(100);
//    tracker.draw3D();

    // draw box
//    ofNoFill();
//    ofSetColor(255, 0, 0);
//    for (int i = 0; i < tracker.getNumUser(); i++)
//    {
//        ofxNiTE2::User::Ref user = tracker.getUser(i);
//        const ofxNiTE2::Joint &joint = user->getJoint(nite::JOINT_RIGHT_HAND);
//
//        joint.transformGL();
//
//        ofPoint myHand = joint.getGlobalPosition();
//        ofLogNotice(ofToString(myHand));
//
//        ofDrawBox(300);
//        joint.restoreTransformGL();
//    }

//    cam.end();

}



vector<ofPoint> & kinectHandler::getPointCloud() {
    return pointCloud;
}
