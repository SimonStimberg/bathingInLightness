#include "ofApp.h"

// INITIALIZATION

//--------------------------------------------------------------
void ofApp::setup(){

    // ofLogToFile("logFile.txt", true);
    ofToggleFullscreen();
    
    ofSetFrameRate(60);
    ofSetVerticalSync(false);   // headless mode: do not sync to monitor refresh rate 
    ofBackground(10, 10, 10);
    
    
    
    // the dimensions of the bounding box the installation lives in
    worldSize.x = 600;      // width in mm
    worldSize.z = 600;
    worldSize.y = 1100;      // height in mm     / 960 ~ golden ratio 


    worldSize /= 3;     // to convert the real world measurements into simulation space



    int num = 100; // amount of Particles
    refreshRate = 20; // interval sending to Arduino in Milliseconds (40ms = "25fps" -> 1000 / 25)
    int numberOfBulbs = 52;     // has only effect on the creation of the random cluster

    intensityWeightKinect = 0.8;    // sets the maximum a kinect point is being weighted for computing the intensity
    intensityWeightFlock  = 0.4;

    float maxBrightness = 1.00;     // scales the overall brightness (factor for all intensity values)
    

    
    
    // GLOBAL FLAGS
    
    drawBox = true;
    drawParticles = true;
    drawBulbs = true;
    drawCam = true;
    drawKinect = true;
    drawSynthControls = true;
    drawInstructions = true;
    kinectLEDon = false;
    testSwitch = true;
    dimSound = false;

    bAttraction = false;
    playDemo = true;



    
    bulbCluster.setup(worldSize);  // use this to initialize a physical cluster with hard-coded positions
    bulbCluster.setMaxBrightness(maxBrightness);
    // bulbCluster.setup(worldSize, numberOfBulbs);  // use this to create a random cluster

    kinectToPoints.setup(worldSize);
    kinectToPoints.switchLEDon(kinectLEDon);

    initSynth(true);    // true for quad mode, false for stereo mode




    // INITIALIZE THE SWARM
	// flock.setup(num, ofGetWidth()/2, ofGetHeight()/2,-ofGetWidth()/2, 100);
    flock.setup(num, 0, 0, 0, 100);

    flock.scale = 3.0; // scale the system: relation worldsize to boidsize
	flock.setBounds(-worldSize.x*0.5, -worldSize.y*0.5, -worldSize.z*0.5,   worldSize.x*0.5, worldSize.y*0.5, worldSize.z*0.5);
	flock.setBoundmode(0)->setDt(2.0);    // some multiplicator affecting speed and forces -> works good if it stays close to the scale value

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

    mouseTrigger();     // handle mouse trigger for shutdown/restart

    // setBrightnessToDaytime();
    
    
    // COMPUTE KINECT POINT CLOUD

    kinectToPoints.update();    
    // vector <ofPoint> & kinectPointCloud = kinectToPoints.getPointCloud();
    vector <ofPoint> & kinectPointCloud = kinectToPoints.getDemoPoints(playDemo);   // DEMO-MODE FOR MAPPING


    // HAND POINT CLOUD TO FLOCK AS ATTRACTION POINTS AND UPDATE

    flock.clearAttrPts();
    if (kinectPointCloud.size() > 10) {
        bAttraction = true;
        for (unsigned int i = 0; i < kinectPointCloud.size(); i++) {
            // x, y, z, force, acttraction distance
            flock.addAttractionPoint(kinectPointCloud[i].x, kinectPointCloud[i].y, kinectPointCloud[i].z, 250 , 600);
        }
    } else {
        bAttraction = false;
        // add a soft attraction Point in the center to reward the swarm for staying away from the corners
        if (testSwitch) { flock.addAttractionPoint(0, 0, 0, 0.40, 600); }    
    }

    flock.update();
    vector <ofPoint> & flockPositions = flock.getPositions();
    
    
    // SET THE BRIGHTNESS OF THE KINECT POINTS ACCORDING TO MOVEMENT FLOW TAKING PLACE IN FRONT OF THE CAMERA
    // intensityWeightKinect = ofMap(kinectToPoints.getFlow(), 0.0, 1.0, 0.5, 1.0);
    // intensityWeightKinect = kinectToPoints.getFlow();


    
    // DERIVE LIGHT BULB INTENSITES

    // bulbCluster.setCheckRadius(15);     // DEMO-MODE FOR MAPPING
    // bulbCluster.updateIntensities(kinectPointCloud, 1.0, flockPositions, 0.0);  // DEMO-MODE FOR MAPPING
    bulbCluster.updateIntensities(kinectPointCloud, intensityWeightKinect, flockPositions, intensityWeightFlock);
    vector <float> & intensities = bulbCluster.getIntensities();
    
    

    // SEND INTENSITIES TO SYNTH

    synth.setVoiceLevels(intensities);    


    // SEND INTENSITY VALUES TO ARDUINO
    // sendToArduino(intensities);   // uncomment this line if an Arduino is connected via Serial USB
    
}





//--------------------------------------------------------------
void ofApp::draw(){

    if(drawInstructions) {
    
        ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
        
        ofSetColor(255);



        ofDrawBitmapStringHighlight(    "PRESS\n\n"
                                        "\'X\' to show bounding box\n"
                                        "\'P\' to show particles\n"
                                        "\'B\' to show light bulbs\n"
                                        "\'K\' to show Kinect point cloud\n"
                                        "\'C\' to show depth camera image\n"
                                        "\'S\' to show synth controls\n\n"
                                        "\'R\' to toggle serial refreshrate 20/50fps\n"
                                        "\'M\' to set Kinect tilt angle to 0 deg\n"
                                        "\'N\' to set Kinect tilt angle to 30 deg\n"
                                        "\'L\' to switch Kinect LED on/off\n\n"
                                        "\'F\' toggle fullscreen\n"
                                        "\' \' turn display on/off\n\n"
                                        "Framerate: " + ofToString(ofGetFrameRate()), 
                                        
                                        50, ofGetHeight() * 0.5 + 150);
            

        
        
        
        ofPushMatrix();
        
        
        ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5, 0); // move everything into the center of the canvas
        
        ofRotateYDeg(ofMap(ofGetMouseX(), 0, ofGetWidth(), -90.0, 90.0)); // rotate by mouseX-position
        //    ofRotateXDeg(ofMap(ofGetMouseY(), 0, ofGetHeight(), 10.0, -10.0)); // rotate by mouseY-position
        
        
        
        
        
        
        // DRAW PARTICLES
        
        if(drawParticles){
            if(bAttraction) {
                ofSetColor(0, 255, 0);
            }
            flock.drawScaled();
            ofSetColor(255);
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
            kinectToPoints.drawCam(50, 150);
        }


        if(drawSynthControls) {
            synth.gui.setPosition(ofGetWidth() - 370, 70);
            synth.gui.draw();
        }

    }
    
}




void ofApp::setBrightnessToDaytime() {
    
    // dim the brightness while in the twilight 18:30 - 21:45
    int now = ofGetHours() * 100 + ofGetMinutes();
    
    if (now >= 1830 && now <= 2145) {

        int momentInTime = (ofGetHours() - 18) * 60  +  ofGetMinutes() - 30;
        int totalTime = (21 - 18) * 60  +  45 - 30;

        float newMaxBrightness = ofMap(momentInTime, 0, totalTime, 1.0, 0.75);
        bulbCluster.setMaxBrightness(newMaxBrightness);
        // ofLogNotice("brghtnss: " + ofToString(newMaxBrightness));

    } else if (now < 1830 && now > 900) {        
        bulbCluster.setMaxBrightness(1.0);
    } else {
        bulbCluster.setMaxBrightness(0.75);
    }

}



void ofApp::initSynth(bool quadMode) {

    vector <int> pitches;
    vector <glm::vec2> panPositions;
    vector <ofPoint> bulbPositions = bulbCluster.getPositions();
    
    
    for(int i = 0; i < bulbPositions.size(); i++){
                

        ofPoint & b = bulbPositions[i];


        // set panPosition of the note according to the x and z position of the bulb
        float panX = ofMap(b.x, -worldSize.x*0.5, worldSize.x*0.5, -4.f, 4.f);      // using a greater range than -1.0 to 1.0 will overemphasize the panning and make the spacial effect more extreme -> more perceivable
        // float panY = ofMap(b.z, -worldSize.z*0.5, worldSize.z*0.5, -4.f, 4.f);
        // float panY = -1.0;      // for Stereo fall back
        float panY = (!quadMode) ? ofMap(b.z, -worldSize.z*0.5, worldSize.z*0.5, -4.f, 4.f) : -1.0;      // stereo vs 4ch setup
        ofLogNotice("4channel mode active: " + ofToString(quadMode));

        panPositions.push_back( glm::vec2(panX, panY) );    // to use a 2-speaker setup only, replace panY with -1.0 to ignore the 2nd spacial dimension


        // add a note - choosing the pitch according to bulbs position in the cluster
        if(b.y < worldSize.y * 0.33 - worldSize.y*0.5) {    // upper part of bulbs

            int pick = (int)ofRandom(0,3);
            if( pick == 0) {
                pitches.push_back(70);  // base minor 7th  70
            } else if (pick == 1) {
                pitches.push_back(74);  // base 9th  74
            } else {
                pitches.push_back(79);  // 5th +1 octave  79
            }

        } else if(b.y < worldSize.y * 0.66 - worldSize.y*0.5) {    // middle

            if(b.x < 0) {   // left
                
                int pick = (int)ofRandom(0,3);
                if( pick == 0) {
                    pitches.push_back(63);  // minor 3rd  63
                } else if(pick == 1) {
                    pitches.push_back(72);  // base tone +1 octave      
                } else {
                    pitches.push_back(68);  // minor 6th  68
                }

            } else {        // right

                if( ( (int)ofRandom(0,2) ) == 0) {
                    pitches.push_back(67);  // 5th  67
                } else {
                    pitches.push_back(75);  // minor 3rd +1 octave  75
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



//--------------------------------------------------------------
// SHUTDOWN THE SYSTEM IF MOUSE IS PRESSED FOR 5 SECONDS
void ofApp::mouseTrigger() {

    if (mouseDown && !drawInstructions) {        

        // ofLogNotice("still pressed");
        if (ofGetElapsedTimeMillis() > mouseDownTime + 5000 ) {

            string pw = ofBufferFromFile("masterpw.txt").getText();

            if (mouseDownButton == 0) { // left mouse button
                ofLogNotice("restart please");
                // ofSystem("echo \"xxx\" | sudo -S shutdown -r now");
                ofSystem("echo \"" + pw + "\" | sudo -S shutdown -r now");
                ofExit();
            } else if (mouseDownButton == 2) {  // right mouse button
                ofLogNotice("shutdown please");
                ofSystem("echo \"" + pw + "\" | sudo -S shutdown -h now");                
                ofExit();
            }
            // sudo -S -> read password from stdin
            // shutdown -h -> halt (aka power off)
            // shutdown -r -> reboot
        }
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
        // kinectToPoints.setTiltAngle(30);
    }
    if( key == 'm'){
        // kinectToPoints.setTiltAngle(0);
    }
    if( key == 'l'){
        kinectLEDon = !kinectLEDon;
        kinectToPoints.switchLEDon(kinectLEDon);
    }
    if( key == 't'){
        testSwitch = !testSwitch;
    }
    if( key == 'f'){
        ofToggleFullscreen();
    }
    if( key == 'd'){
        playDemo = !playDemo;
    }
    if( key == OF_KEY_UP){
        kinectToPoints.offsetDemoPoints(-5);
    }
    if( key == OF_KEY_DOWN){
        kinectToPoints.offsetDemoPoints(5);
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

    if (!mouseDown) {
        mouseDown = true;
        mouseDownButton = button;
        mouseDownTime = ofGetElapsedTimeMillis();
        // ofLogNotice("first press");
    } 
    
    // if(ofGetElapsedTimeMillis()-prevClickTime < 300){  
        // switch sound
        // if(!dimSound) {
        //     synth.gain.set(-64.0f);
        //     dimSound = true;
        // } else {
        //     synth.gain.set(0.0f);
        //     dimSound = false;;
        // }
    // }  
    // prevClickTime = ofGetElapsedTimeMillis();  
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    mouseDown = false;
    
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {

    if (!drawInstructions) {    // only when display is disabled

        // use the scroll wheel to adjust the overall sound level
        float soundLevel = synth.gain.get();
        soundLevel -= scrollY * 0.2;
        soundLevel = ofClamp(soundLevel, -32.0, 12.0);  // clamp between -32dB and 12dB

        synth.gain.set( soundLevel );

    }
    
    // ofLogNotice("scroll Y: " + ofToString(scrollY));    
    // float lin = 0.5;
    // ofLogNotice("lin 0.5 as dB: " + ofToString( pdsp::LinToDB::eval(lin) ));
    // float db = -6.0;
    // ofLogNotice("dB -6 as lin: " + ofToString(dB(db)));

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
