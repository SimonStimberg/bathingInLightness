//
//  objDetector.cpp
//  clusterSerial_openCV
//
//  Created by Simon Paul Stimberg on 15.07.19.
//
//

#include "objDetector.h"


objDetector::objDetector(){
    
    // use a low resolution image size to increase performance
    camWidth = 150;
    camHeight = 120;
    
    threshold = 50;
    
    
    // list video devices
    vidGrabber.setVerbose(true);
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    
    // choose the first device found and initialize it
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    
    while(!vidGrabber.isInitialized());  // wait till the vidGrabber is initialized
    int grabW = vidGrabber.getWidth();
    int grabH = vidGrabber.getHeight();
    printf("asked for 150 by 120 - actual size is %i by %i", grabW, grabH);
    

    // allocate meomory for images as buffer
    colorImg.allocate(camWidth,camHeight);
    grayImage.allocate(camWidth,camHeight);
    grayBg.allocate(camWidth,camHeight);
    grayDiff.allocate(camWidth,camHeight);
    
}



ofPoint objDetector::getPosition(){
    
    
    pos = ofPoint(10,10,10);
    
    // only do the computation if the frame is in fact new
    bool bNewFrame = false;
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    
    
    if (bNewFrame && vidGrabber.isInitialized()){
        
        
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImage = colorImg;

        
        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);

        
        // find contours which are between the size of 10 pixels and 1/3 the w*h pixels.
        contourFinder.findContours(grayDiff, 10, (camWidth*camHeight)/3, 1, false);	// find holes set to false
    
        
        // if there is at least one Blob (Binary Large Object) found
        // take the very first (usually the biggest one) and get the center of it

        if(contourFinder.nBlobs > 0) {

            // normalize the values and reverse/mirror the X position
            pos.x = 1 - (contourFinder.blobs[0].boundingRect.getCenter().x / camWidth);
            pos.y = contourFinder.blobs[0].boundingRect.getCenter().y / camHeight;
            pos.z = 1;
            
        }
  
    }
    
    return pos;
}



void objDetector::draw() {
    
    // for debugging draw the detected image

    ofSetColor(255);
    grayImage.draw(0,0);
    grayDiff.draw(0,130);
//    contourFinder.draw(0,260);

    
    // also draw the computed object position in that image
    ofSetColor(0, 0, 255);
    ofDrawCircle((1 - pos.x) * camWidth + 0, pos.y * camHeight + 0, 6);
    
}



void objDetector::learnBG() {
    
    // "learn background"
    // save and use the current frame as reference image
    
    vidGrabber.update();
    colorImg.setFromPixels(vidGrabber.getPixels());
    grayBg = colorImg;
    
}


