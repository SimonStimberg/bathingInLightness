// Based on: ofxCV addon example

#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup()
{
    
	ofSetVerticalSync(true);

//	cam.setup(640, 480);
    
    
    // KINECT INITIALISATION
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    // kinect.init();
    // kinect.init(true); // shows infrared instead of RGB video image
    kinect.init(false, false); // disable video image (faster fps)
    kinect.open();
    ofLogNotice("Dimensions: " + ofToString(kinect.getWidth()) + "x" + ofToString(kinect.getHeight()));

    grayImg.allocate(kinect.getWidth(), kinect.getHeight());
    grayImg.setUseTexture(true);
    flowPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_PIXELS_RGBA);
    

    
    mioFlow.setup(kinect.getWidth(), kinect.getHeight());
        mioFlow.enabled = true;
        mioFlow.doShaderBlur = true; // smooth optical flow
        mioFlow.doShaderRepos = false; // warp original image
        mioFlow.doReadback = true; // needed for vector drawing
        
        mioFlow.lambda = 0.1;
        mioFlow.blurAmount = 5.0;
        mioFlow.displaceAmount = 1000;
    //        mioFlow.drawVectorsStep = 100;
    
    
	stepSize = 20;
	ySteps = kinect.getHeight() / stepSize;
	xSteps = kinect.getWidth() / stepSize;
       
	for (int y = 0; y < ySteps; y++)
    {
		for (int x = 0; x < xSteps; x++)
        {
            flowAmount.push_back(ofVec2f(0,0));
		}
	}
    
    
    
    setupAudio();
    setupOscs();
    
    ofHideCursor();
    
    blurOn = true;

}



void ofApp::update()
{
	kinect.update();

    if(kinect.isFrameNew())
    {
        
//        grayImg = kinect.getDepthPixels();
        grayImg.setFromPixels(kinect.getDepthPixels());
//        grayImg.resize(320, 240);
        grayImg.mirror(false, true);
        
        int nearThreshold = 230;
        int farThreshold = 170;
        
        ofPixels & pix = grayImg.getPixels();
        int numPixels = pix.size();
        for(int i = 0; i < numPixels; i++) {
            if(pix[i] < nearThreshold && pix[i] > farThreshold) {
//                pix[i] = 255;
            } else {
                pix[i] = 0;
            }
        }
        grayImg.updateTexture();
        
        if(blurOn) {
            grayImg.blur(13);
            grayImg.threshold(100);
            grayImg.blur(5);
    //        grayImg
        }
        
        
        mioFlow.update(grayImg.getTexture());
//        mioFlow.update(kinect.getDepthTexture());
        
        
        ofTexture & tex = mioFlow.getFlowBlurTexture();
        
        tex.readToPixels(flowPix);
        
        
        // the code is derived from the drawVectors member function of the MIOflow addOn
        // it derives from the color values of the Flow texture a vector of the flow
        int drawVectorsScale = 1000;
        int k = 0;
        
        for(int j = 0; j < kinect.getHeight(); j += stepSize) {
            for(int i = 0; i < kinect.getWidth(); i += stepSize) {
                ofFloatColor c = flowPix.getColor(i, j);
                if (c.a >0.95)  c.b *= -1.0;
                
                ofVec2f v = ofVec2f(c.r - c.g, c.b);
//                v *= drawVectorsScale;
                
                if(v.length() < 0.1) {
                    v = ofVec2f(0, 0);
                } else {
//                    v.normalize();
                    v.scale(ofMap(v.length(), 0.1, 0.2, 0.0, 1.0));
                }
                flowAmount[k] = v;
                k++;
                
            }
        }


        
        
        
        
        
        
        
        
        
        
//        // TODO: add documentation
//		flow.setWindowSize(stepSize);
//		flow.calcOpticalFlow(grayImg);
//
//		int i = 0;
//        int iTotal = xSteps * ySteps;
//		float distortionStrength = 6;
//
////        vector<float> newAmp[8];
//        float newAmp[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//
//
//
//		for(int y = 1; y + 1 < ySteps; y++)
//        {
//			for(int x = 1; x + 1 < xSteps; x++)
//            {
//
//				int i = y * xSteps + x;
//                glm::vec2 position(x * stepSize, y * stepSize);
//                ofRectangle area(position - glm::vec2(stepSize, stepSize) / 2, stepSize, stepSize);
////                glm::vec2 offset = flow.getTotalFlowInRegion(area);
//                ofVec2f offset = flow.getAverageFlowInRegion(area);
//
////                flowAmount[i] = offset;
//
//                if(offset.length() < 3) {
////                    offset = ofVec2f(0, 0);
//                    offset.scale(ofMap(offset.length(), 0, 3, 0.0, 0.1));
//                } else {
////                    offset.normalize();
//                    offset.scale(ofMap(offset.length(), 0, 20, 0.0, 1.0));
//
//                    ofVec2f v1(-1,1);
////                    ofVec2f v2(0,1);
//                    float angle = v1.angle(offset)+180;
//
//                    ofLogNotice(ofToString(angle));
//
//
//                    if (angle < 90) {
//                        newAmp[0] += 1.0 / iTotal;
//                        newAmp[4] += offset.length() / iTotal * 0.25;
//                    } else if (angle < 180) {
//                        newAmp[1] += 1.0 / iTotal;
//                        newAmp[5] += offset.length() / iTotal * 0.25;
//                    } else if (angle < 270) {
//                        newAmp[2] += 1.0 / iTotal;
//                        newAmp[6] += offset.length() / iTotal * 0.25;
//                    } else  {
//                        newAmp[3] += 1.0 / iTotal;
//                        newAmp[7] += offset.length() / iTotal * 0.25;
//                    }
//                }
//
//                flowAmount[i] = offset;
//
//				i++;
//			}
//		}
        
//        ofLogNotice(ofToString(oscAmp));
        
//        for (int i = 0; i < 8; i++) {
//            float amp = synth.getAmplitude(i) * 0.8 + newAmp[i] * 0.2;
////            float amp = ofLerp(synth.getAmplitude(i), newAmp[i], 0.1);
//            synth.setAmplitude(i, amp);
//        }
        
        

        

	}
    

    

}

void ofApp::draw()
{
    
	ofBackground(0);
    ofFill();
    
    
    

    

    
//    ofPushStyle();
//    ofPushMatrix();
//    int drawVectorsScale = 1000;
//
//    for(int j = 0; j < kinect.getHeight(); j += stepSize) {
//        for(int i = 0; i < kinect.getWidth(); i += stepSize) {
//            ofFloatColor c = flowPix.getColor(i, j);
//            if (c.a >0.95)  c.b *= -1.0;
//
//            ofVec2f v = ofVec2f(c.r - c.g, c.b);
//            v *= drawVectorsScale;
//            ofLine(i, j, i+v.x, j+v.y);
//        }
//    }
//    ofPopMatrix();
//    ofPopStyle();
    
    

    

    
    float xScale = ofGetScreenWidth() / kinect.getWidth();
    float yScale = ofGetScreenHeight() / kinect.getHeight();

    int i = 0;
    for(int y = 1; y + 1 < ySteps; y++)
    {
        for(int x = 1; x + 1 < xSteps; x++)
        {
            int i = y * xSteps + x;

            auto shift_x = stepSize * x * yScale;  //  * xScale
            auto shift_y = stepSize * y * yScale;  //  * yScale

//            ofRectangle(shift_x, shift_y, );

//            ofSetLineWidth(1);

//            ofDrawLine(shift_x, shift_y, shift_x + (flowAmount[i].x * stepSize * xScale), shift_y + (flowAmount[i].y * stepSize * yScale));
            ofDrawCircle(shift_x, shift_y, ofClamp(flowAmount[i].length(), 0.0, 1.0) * stepSize);
            i++;

        }
    }
    
    
    grayImg.draw(ofGetWidth()-kinect.getWidth(), ofGetHeight()-kinect.getHeight());
    
    //    mioFlow.drawReposition(0, 0);
        mioFlow.drawReposition(mioFlow.getWidth(), 0);
        mioFlow.drawVectors(mioFlow.getWidth(), 0);
    //
        mioFlow.drawFlowGridRaw(0, mioFlow.getHeight());
//        mioFlow.drawFlowGrid(ofGetWidth()-kinect.getWidth(), ofGetHeight()-kinect.getHeight());
    
    

    ofDrawBitmapString("Fps: " + ofToString(ofGetFrameRate()), 10, 10);
}



//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    
    synth.updateSoundBuffer(outBuffer);

    
    // THREAD INFO
    // lock_name is the "var" name of the lock guard, kind of
    // a variable that is being constructed with a mutex (audioMutex),
    // locks the mutex at its construction and unlocks the mutex
    // when it is being destroyed, i.e., at the end of the scope
    std::unique_lock<std::mutex> lock_name(audioMutex);
    lastBuffer = outBuffer;
}



//--------------------------------------------------------------
void ofApp::setupAudio() {
    
    // start the sound stream with a sample rate of 44100 Hz, and a buffer
    // size of 512 samples per audioOut() call
    ofSoundStreamSettings settings;
    settings.numOutputChannels = 2;
    settings.sampleRate = 44100;
    settings.bufferSize = 512;
    settings.numBuffers = 4;
    settings.setOutListener(this);
    
    // the following setup function initiates the whole audio connection
    // it invokes the underlying RTAudioSoundStream to
    // - create an RtAudio object
    // - connect the object to the RtAudioCallback function
    // - start the stream and hence have a continious connection to audio in & out
    soundStream.setup(settings); // RtAudioCallback is called by Apple's CoreAudio
    
//    synth1.setSampleRate(settings.sampleRate);

}


void ofApp::setupOscs() {
    
     float freq1 = 110;
     synth.setFrequency(1, freq1);
     
     float freq2 = 220 * pow(2,(3/12.f));
     synth.setFrequency(2, freq2);
     
     float freq3 = 220 * pow(2,(10/12.f));
     synth.setFrequency(3, freq3);
     
     float freq4 = 220 * pow(2,(7/12.f));
     synth.setFrequency(4, freq4);
    

     float freq5 = 55;
     synth.setFrequency(5, freq5);

     float freq6 = 220 * pow(2,(19/12.f));
     synth.setFrequency(6, freq6);

     float freq7 = 220 * pow(2,(14/12.f));
     synth.setFrequency(7, freq7);

     float freq8 = 220 * pow(2,(15/12.f));
     synth.setFrequency(8, freq8);
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch(key) {
        case 'b':
            blurOn = !blurOn;
            break;
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}
