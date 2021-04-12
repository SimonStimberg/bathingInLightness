#include "ofApp.h"

// INITIALIZATION

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    
    // the dimensions of the bounding box the installation lives in
    worldSize.x = 600;      // width in mm
    worldSize.z = 600;
    worldSize.y = 960;      // height in mm


    worldSize /= 3;     // to convert the real world measurements into simulation space



    int num = 100; // amount of Particles
    refreshRate = 20; // interval sending to Arduino in Milliseconds (40ms = "25fps" -> 1000 / 25)
    int numberOfBulbs = 52;     // has only effect on the creation of the random cluster
    
    
    
    // GLOBAL FLAGS
    
    drawBox = true;
    drawParticles = true;
    drawBulbs = true;
    drawCam = true;
    drawKinect = false;
    drawSynthControls = false;
    drawInstructions = true;



    
    // bulbCluster.setup(worldSize);  // use this to initialize a physical cluster with hard-coded positions
    bulbCluster.setup(worldSize, numberOfBulbs);  // use this to create a random cluster

    kinectToPoints.setup(worldSize);

    initSynth();




    // INITIALIZE THE SWARM
	// flock.setup(num, ofGetWidth()/2, ofGetHeight()/2,-ofGetWidth()/2, 100);
    flock.setup(num, 0, 0, 0, 100);

    flock.scale = 3.0; // scale the system: relation worldsize to boidsize
	flock.setBounds(-worldSize.x*0.5, -worldSize.y*0.5, -worldSize.z*0.5,   worldSize.x*0.5, worldSize.y*0.5, worldSize.z*0.5);
	flock.setBoundmode(0)->setDt(3.0);    // some multiplicator affecting speed and forces -> works good if it stays close to the scale value

    // ADJUST BOID PARAMETERS
    // flock.setCohesion(25.0f);
    flock.setDistCohesion(150.0f);
    // flock.setSeparate(5.0f);


 
    
    
    // INITIALIZE SERIAL COMMUNICATION
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 38400;
    serial.setup(0, baud); //open the first device
    
    timeCheck = ofGetElapsedTimeMillis();
    
    

    
}










//--------------------------------------------------------------
void ofApp::update(){
    
    
    // COMPUTE KINECT POINT CLOUD

    kinectToPoints.update();    
    vector <ofPoint> & kinectPointCloud = kinectToPoints.getPointCloud();



    // HAND POINT CLOUD TO FLOCK AS ATTRACTION POINTS AND UPDATE

    flock.clearAttrPts();
    for (unsigned int i = 0; i < kinectPointCloud.size(); i++) {
        // x, y, z, force, acttraction distance
        flock.addAttractionPoint(kinectPointCloud[i].x, kinectPointCloud[i].y, kinectPointCloud[i].z, 250 , 600);
    }

    flock.update();
    vector <ofPoint> & flockPositions = flock.getPositions();
    
    
    
    // DERIVE LIGHT BULB INTENSITES

    bulbCluster.updateIntensities(kinectPointCloud, 0.8, flockPositions, 0.5);
    vector <float> & intensities = bulbCluster.getIntensities();
    
    

    // SEND INTENSITIES TO SYNTH

    synth.setVoiceLevels(intensities);    


    // SEND INTENSITY VALUES TO ARDUINO
    sendToArduino(intensities);   // uncomment this line if an Arduino is connected via Serial USB
    
}





//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    
    ofSetColor(255);

    if(drawInstructions) {

        ofDrawBitmapStringHighlight(    "PRESS\n\n"
                                        "\'X\' to show bounding box\n"
                                        "\'P\' to show particles\n"
                                        "\'B\' to show light bulbs\n"
                                        "\'K\' to show Kinect point cloud\n"
                                        "\'C\' to show depth camera image\n"
                                        "\'S\' to show synth controls\n\n"
                                        "Framerate: " + ofToString(ofGetFrameRate()), 50, ofGetHeight() * 0.5 - 100);
        
    }    
    
    
    
    ofPushMatrix();
    
    
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5, 0); // move everything into the center of the canvas
    
    ofRotateYDeg(ofMap(ofGetMouseX(), 0, ofGetWidth(), -90.0, 90.0)); // rotate by mouseX-position
    //    ofRotateXDeg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 10.0, -10.0)); // rotate by mouseY-position
    
    
    
    
    
    
    // DRAW PARTICLES
    
    if(drawParticles){
        flock.drawScaled();
    }
    


    // DRAW BULB CLUSTER
    
    if(drawBulbs){
        bulbCluster.draw();
    }
    
    
    
    // DRAW POINT CLOUD FROM KINECT
    if(drawKinect) {
        ofSetColor(255, 0, 0);
        kinectToPoints.draw();
    }
    
    

    // DRAW BOUNDING BOX
    
    if(drawBox){
        ofNoFill();
        ofSetColor(100);
        ofDrawBox(worldSize.x, worldSize.y, worldSize.z);
        ofFill();
    }
    
    
    
    ofPopMatrix();
    // ofTranslate(ofGetWidth() - 300, 0);

    if(drawCam) {
        ofSetColor(255);
        kinectToPoints.drawCam(ofGetWidth() - 400, ofGetHeight() * 0.5);
    }


    if(drawSynthControls) {
        synth.gui.setPosition(ofGetWidth() - 370, 70);
        synth.gui.draw();
    }
    
}




void ofApp::initSynth() {

    vector <int> pitches;
    vector <float> panPositions;
    vector <ofPoint> bulbPositions = bulbCluster.getPositions();
    
    
    for(int i = 0; i < bulbPositions.size(); i++){
                

        ofPoint & b = bulbPositions[i];


        // set panPosition of the note according to the x-position of the bulb
        float pan = ofMap(b.x, -worldSize.x*0.5, worldSize.x*0.5, -1.f, 1.f);
        panPositions.push_back( pan );


        // add a note - choosing the pitch according to bulbs position in the cluster
        if(b.y < worldSize.y * 0.33 - worldSize.y*0.5) {    // upper part of bulbs

            if( ( (int)ofRandom(0,2) ) == 0) {
                pitches.push_back(70);  // base minor 7th
            } else {
                pitches.push_back(74);  // base 9th
            }

        } else if(b.y < worldSize.y * 0.66 - worldSize.y*0.5) {    // middle

            if(b.x < 0) {   // left
                
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



void ofApp::exit() { 
    kinectToPoints.exit();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'x'){
        drawBox = !drawBox;
    }
    if( key == 'p'){
        drawParticles = !drawParticles;
    }
    if( key == 'b'){
        drawBulbs = !drawBulbs;
    }
    if( key == 'c'){
        drawCam = !drawCam;
    }
    if( key == 'k'){
        drawKinect = !drawKinect;
    }
    if( key == 's'){
        drawSynthControls = !drawSynthControls;        
    }
    if( key == ' '){
        drawInstructions = !drawInstructions;        
    }
    if( key == 'r'){
        refreshRate = (refreshRate > 20) ? 20 : 50;
        ofLogNotice("new Framerate: " + ofToString(1000 / refreshRate));
    }
    if( key == 'n'){
        kinectToPoints.setTiltAngle(30);
    }
    if( key == 'm'){
        kinectToPoints.setTiltAngle(0);
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
