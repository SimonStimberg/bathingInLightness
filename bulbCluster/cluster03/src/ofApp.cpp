#include "ofApp.h"

// INITIALIZATION

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    
    boxSize = 200; // size of the bounding box the particles live in
    int num = 100; // amount of Particles
    refreshRate = 50; // interval sending to Arduino in Milliseconds (40ms = "25fps")
    
    
    
    // GLOBAL FLAGS
    
    drawParticles = true;
    drawBulbs = true;
    drawCam = false;
    drawSynthControls = false;
    bLearnBakground = true;
    attractorFound = false; // flag for attractor detection
    attractorActive = true;
    
    currentMode = POSITIVE;
    nextJump = 0;
    
    
    
    // CREATE a vector array of PARTICLES and initializes them
    
    p.assign(num, particle());
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
        p[i].reset(boxSize);
    }
//    detector = objDetector();
    


    kinectToPoints.setup(boxSize);
    
    updateAttractor();
    // createRandomBulbCluster();
    initPhysicalCluster();


    // vector <int> pitches;
    // // vector <int> scale = {36, 43, 48, 55, 60, 63, 67, 70, 74, 79, 84, 87, 91, 94, 98, 48, 60, 67, 72, 96} ;
    // vector <int> scale = {60, 63, 67, 70, 74};
    // for(int i = 0; i < 52; i++) {
    //     pitches.push_back(scale[(int) ofRandom(0,scale.size())]);
    // }

    // synth.setup( pitches );
    
    
    
    // INITIALIZE SERIAL COMMUNICATION
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 9600;
    serial.setup(0, baud); //open the first device
    
    timeCheck = ofGetElapsedTimeMillis();
    lastDetection = ofGetElapsedTimeMillis();
    
    

    
}





//--------------------------------------------------------------
// creates a cluster of random positioned bulbs

// for testing / demonstration purpose when you have no physical cluster connected

void ofApp::createRandomBulbCluster() {
    
    int bulbNum = 52;
    
    for(int i = 1; i < bulbNum+1; i++) {
        
        
        // DETERMINE X + Z POSITION
        // using the golden ratio (theta = 137.508 * n)
        // the so called "sunflower distribution"
        
        float r = 12 * sqrt(i);
        float angle = (137.508 * i) * (M_PI / 180);
        
        float x = boxSize * 0.5 + r * sin(angle);
        float z = boxSize * 0.5 + r * cos(angle);
        
        
        // CALCULATE Y VALUE
        // (this is a kind of dirty solution to receive an even distribution in form of a sphere
        // it uses noise and a decreasing offset from the center to the outside while swaping the shifting direction each iteration)
        
        int yShift{0};
        if (i % 2 == 0) {
            yShift = -bulbNum + 4 + i;
        } else {
            yShift = bulbNum + 3 - i;
        }
        
        float y = boxSize * 0.5 + ofClamp((((bulbNum * 6 - i * 6) * ofNoise(i*1.1, 2.1) - (bulbNum * 3 - i * 3 )) + yShift), -bulbNum * 1.5, bulbNum * 1.5);
        
        
        bulbCluster.push_back(bulb(ofPoint(x, y, z)));
    }
    
}





//--------------------------------------------------------------
// creates a cluster from an array of discrete position values

// x and y values are derived from the cluster construction
// the y positions have to be measured and manually typed into the array
// unit is millimeter

void ofApp::initPhysicalCluster() {
    
    // abrennec:
    // std::map<key,value> oder std::vector<glm::vec3f>
    // koenntest du dir mal anschauen, nur zur Info.
    vector <float> positions = {
        281, 324, 0,   // Bulb No. 1
        304, 262, 300,   // Bulb No. 2
        340, 347, 400,   // Bulb No. 3
        229, 287, 75,   // Bulb No. 4
        365, 262, 570,   // Bulb No. 5
        277, 385, 580,   // Bulb No. 6
        256, 221, 600,   // Bulb No. 7
        400, 332, 140,   // Bulb No. 8
        205, 346, 280,   // Bulb No. 9
        343, 197, 265,   // Bulb No. 10
        338, 409, 400,   // Bulb No. 11
        192, 237, 445,   // Bulb No. 12
        427, 272, 60,   // Bulb No. 13
        223, 410, 455,   // Bulb No. 14
        285, 167, 80,   // Bulb No. 15
        410, 393, 540,   // Bulb No. 16
        152, 306, 180,   // Bulb No. 17
        408, 192, 600,   // Bulb No. 18
        293, 457, 240,   // Bulb No. 19
        197, 176, 330,   // Bulb No. 20
        463, 322, 375,   // Bulb No. 21
        161, 396, 225,   // Bulb No. 22
        343, 132, 480,   // Bulb No. 23
        388, 453, 205,   // Bulb No. 24
        129, 245, 535,   // Bulb No. 25
        467, 223, 340,   // Bulb No. 26
        228, 473, 35,   // Bulb No. 27
        238, 121, 505,   // Bulb No. 28
        472, 390, 335,   // Bulb No. 29
        109, 350, 495,   // Bulb No. 30
        408, 131, 500,   // Bulb No. 31
        334, 501, 450,   // Bulb No. 32
        137, 173, 135,   // Bulb No. 33
        509, 283, 560,   // Bulb No. 34
        155, 456, 420,   // Bulb No. 35
        301, 84, 305,   // Bulb No. 36
        447, 462, 470,   // Bulb No. 37
        79, 279, 380,   // Bulb No. 38
        479, 164, 130,   // Bulb No. 39
        269, 527, 520,   // Bulb No. 40
        177, 105, 390,   // Bulb No. 41
        525, 362, 490,   // Bulb No. 42
        90, 408, 410,   // Bulb No. 43
        383, 76, 340,   // Bulb No. 44
        401, 518, 285,   // Bulb No. 45
        77, 203, 250,   // Bulb No. 46
        533, 220, 460,   // Bulb No. 47
        174, 515, 270,   // Bulb No. 48
        242, 57, 165,   // Bulb No. 49
        507, 447, 315,   // Bulb No. 50
        48, 342, 275,   // Bulb No. 51
        468, 102, 275   // Bulb No. 52
        
    };

    vector <int> pitches;
    vector <float> panPositions;
    
    
    for(int i = 0; i < positions.size(); i += 3){
        
        float x = roundf(positions[i]   / 3 * 10) / 10;
        float z = roundf(positions[i+1] / 3 * 10) / 10;
        float y = roundf(positions[i+2] / 3 * 10) / 10;
        
        bulbCluster.push_back(bulb(ofPoint(x, y, z)));


        // set panPosition of the note according to the x-position of the bulb
        float pan = ofMap(x, 0.f, 200.f, -1.f, 1.f);
        panPositions.push_back( pan );


        // add a note - choosing the pitch according to bulbs position in the cluster
        if(y < 67) {    // upper part of bulbs

            if( ( (int)ofRandom(0,2) ) == 0) {
                pitches.push_back(70);  // base minor 7th
            } else {
                pitches.push_back(74);  // base 9th
            }

        } else if(y < 133) {    // middle

            if(x < 100) {   // left
                
                int pick = (int)ofRandom(0,3);
                if( pick == 0) {
                    pitches.push_back(63);  // minor 3rd
                } else if(pick == 1) {
                    pitches.push_back(79);  // 5th +1 octave
                } else {
                    pitches.push_back(68);  // minor 6th
                }

            } else {        // right

                if( ( (int)ofRandom(0,2) ) == 0) {
                    pitches.push_back(72);  // 5th
                } else {
                    pitches.push_back(75);  // minor 3rd +1 octave
                } 

            }

        } else {    // lower part

            int pick = (int)ofRandom(0,3);
            if(pick == 0) {
                pitches.push_back(60);  // base tone
            } else if(pick == 1) {
                pitches.push_back(48);  // base tone -1 octave
            } else {
                pitches.push_back(55);  // 5th -1 octave
            }
            

        }
    }


    synth.setup( pitches );
    synth.setVoicePans( panPositions );
    
}






//--------------------------------------------------------------
void ofApp::update(){
    
    
    kinectToPoints.update();
    
    
    
    
    
    
    // UPDATE ATTRACTOR
    // update the position of the attraction point at a certain time interval
    // since this involves the capturing of a camera image and its analysation (object recognition)
    // better don't do so every frame ;)
    
//    if (lastDetection + 200 < ofGetElapsedTimeMillis()) {
       updateAttractor();
//        lastDetection = ofGetElapsedTimeMillis();
//    }
    
    
    
    // COMPUTE PARTICLE POSITIONS
    
    // vector <ofPoint> & currentParticlePosition = kinectToPoints.getPointCloud();

    vector <ofPoint> currentParticlePosition;
    
   for(unsigned int i = 0; i < p.size(); i++){
       p[i].setMode(currentMode);
       p[i].update(boxSize);
       currentParticlePosition.push_back(p[i].pos);
   }
    
    
    
    // DERIVE LIGHT BULB INTENSITES
    
    vector <float> intensities;
    
    for(unsigned int i = 0; i < bulbCluster.size(); i++){
        if(currentMode == NEGATIVE) {
            bulbCluster[i].isAngry = true;
        } else {
            bulbCluster[i].isAngry = false;
        }
        
        intensities.push_back(bulbCluster[i].update(currentParticlePosition));
    }

    synth.setVoiceLevels(intensities);    
    
    // SEND INTENSITY VALUES TO ARDUINO
    // sendToArduino(intensities);   // uncomment this line if an Arduino is connected via Serial USB
    
}





//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    
    ofSetColor(255);
    ofDrawBitmapStringHighlight(    "PRESS\n\n"
                                    "\'X\' to show bounding box\n"
                                    "\'B\' to show light bulbs\n"
                                    "\'K\' to show Kinect point cloud\n"
                                    "\'S\' to show synth controls\n\n"
                                    "Framerate: " + ofToString(ofGetFrameRate()), 50, ofGetHeight() * 0.5 - 100);
    
    
    
    
    
    ofPushMatrix();
    
    
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5, 0); // move everything into the center of the canvas
    
    ofRotateYDeg(ofMap(ofGetMouseX(), 0, ofGetWidth(), -90.0, 90.0)); // rotate by mouseX-position
    //    ofRotateXDeg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 0.0, 80.0)); // rotate by mouseY-position
    
    
    
    // DRAW BOUNDING BOX
    
    if(drawParticles){
        ofNoFill();
        ofSetColor(255,0,0);
        ofDrawBox(boxSize);
        ofFill();
    }
    
    
    // translate everything half a box
    // in oder to calculate from 0 to 100 not from -50 to 50
    
    ofTranslate(-boxSize*0.5, -boxSize*0.5, -boxSize*0.5);
    
    
    
    // DRAW PARTICLES
    
   if(drawParticles){
       for(unsigned int i = 0; i < p.size(); i++){
           p[i].draw();
       }
   }
    
    
    
    // DRAW BULB CLUSTER
    
    if(drawBulbs){
        for(unsigned int i = 0; i < bulbCluster.size(); i++){
            bulbCluster[i].draw();
        }
    }
    
    
    
    // DRAW POINT CLOUD FROM KINECT
    if(drawCam) {
        kinectToPoints.draw();
    }
    
    
    
    // DRAW ATTRACTOR

    if(drawParticles){
        ofSetColor(0, 255, 0);
        ofDrawSphere(singleAttractor, 10.0);
    }
    
    
    
    
    ofPopMatrix();
    // ofTranslate(ofGetWidth() - 300, 0);


    if(drawSynthControls) {
        synth.gui.setPosition(ofGetWidth() - 300, 20);
        synth.gui.draw();
    }
    
}





//--------------------------------------------------------------
// UPDATE ATTRACTOR POSITION

void ofApp::updateAttractor(){
    
    // get result from the cameras object detection
//    detectionPoint = ofPoint(detector.getPosition());
//
//    // if the detected position is in a reasonable range (hence a person has been detected)
//    // map the position to the attractor point to make the particles follow its position
//
//    if(detectionPoint.y > 0.0 && detectionPoint.x > 0.0 && detectionPoint.x < 9) {
//
//        attractorFound = true;
//
//        // map the X + Y position on a virtual hemisphere facing the front side of the bounding box
//        // hence moving in front of the camera results in the particles follow your movement
//        float r = boxSize/2 * 0.8;
//        float theta = ofMap(detectionPoint.x, 1.0, 0, (0), (M_PI));
//        float arpha = ofMap(detectionPoint.y, 1.0, 0, (M_PI * 0.25), (M_PI * 0.75));
//
//        float x = boxSize * 0.5 + r * cos(theta) * sin(arpha);
//        float z = boxSize * 0.5 + r * sin(theta) * sin(arpha);
//        float y = boxSize * 0.5 + r * cos(arpha);
//
//
//        // if the mood is negative add some noise to the position to make the particles move more unsteady
//        if(currentMode == NEGATIVE) {
//            z = z - ofSignedNoise(x * 10, ofGetElapsedTimef() * 0.7) * 12.0;
//            x = x + (ofSignedNoise(y * 10, ofGetElapsedTimef() * 0.7) - 0.5) * 12.0;
//        }
//
//
//        singleAttractor.set(x, y, z);
//
//        //        ofLog(OF_LOG_NOTICE, "detected: " + ofToString(detectionPoint));
//
//    } else {
        
        // attractorFound = false;
        
        
        // if no person has been detected
        // create a new random attraction point if in POSITIVE mood
        // at a random time interval
        
        // if(currentMode == POSITIVE){
        if(attractorActive){
            
            if(nextJump < ofGetElapsedTimeMillis()) {
                float r = boxSize/2 * 0.75;
                float theta = ofRandom(2 * M_PI);
                float arpha = ofRandom(M_PI);
                
                float x = boxSize * 0.5 + r * cos(theta) * sin(arpha);
                float y = boxSize * 0.5 + r * sin(theta) * sin(arpha);
                float z = boxSize * 0.5 + r * cos(arpha);
                
                singleAttractor.set(x, y, z);
                
                nextJump = ofGetElapsedTimeMillis() + (int)ofRandom(2000, 6000);
            }

        } else {
            singleAttractor.set(1000, 1000, 1000);
        }
            
            
            // if in NEUTRAL or NEGATIVE mood "deactivate" the attractor by setting its position far away outside the canvas
            
        // } else if(currentMode == NEUTRAL) {
            
        //     singleAttractor.set(1000, 1000, 1000);
            
        // } else if(currentMode == NEGATIVE) {
            
        //     singleAttractor.set(1000, 1000, 1000);
            
        // }
//    }
    
    
    // hand over the new attractor position to every particle
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setAttractor(&singleAttractor);
    }
}





//--------------------------------------------------------------
// SENDS LIGHT BULB INTENSITIES TO ARDUINO VIA SERIAL

void ofApp::sendToArduino(vector <float> &intensities) {
    
    
    // only send new command at a certain time interval (refreshRate)
    
    if (timeCheck + refreshRate < ofGetElapsedTimeMillis()){
        
        string commandMsg = "";
        
        
        // set a byte with value 127
        // this serves as "starting flag" - means the starting of a new command string
        
        commandMsg += (char)127;
        
        
        for(int i = 0; i < 52; i++) {
            
            // map the intensity so that it fits into one byte
            // (minus the two values serving as starting and ending flags (127 + 126)
            
            int mapped = ofMap(intensities[i], 0., 1., 0, 125);
            mapped = ofClamp(mapped, 0, 125);  // for safety
            
            
            // convert the integer into a byte (char) and add it to the string
            
            char byte = (char)mapped;
            commandMsg += byte;
            
        }
        
        
        // set a byte with value 126 -> means ending of a command string
        
        commandMsg += (char)126;
        //        ofLog(OF_LOG_NOTICE, ofToString(commandMsg));
        
        
        // write the command string into the serial buffer
        
        const char *buffer = commandMsg.c_str();
        serial.writeBytes(buffer, commandMsg.size());
        
        
        timeCheck = ofGetElapsedTimeMillis();
    }
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'x'){
        drawParticles = !drawParticles;
    }
    if( key == 'b'){
        drawBulbs = !drawBulbs;
    }
    if( key == 'k'){
        drawCam = !drawCam;
    }
    if( key == 's'){
        drawSynthControls = !drawSynthControls;        
    }
    if( key == 'a'){
        attractorActive = !attractorActive;        
    }

}





//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
