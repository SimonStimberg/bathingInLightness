#include "ofApp.h"

// modules channels API example
//
// whatever module whic can have more than one input or output channel
// for example stereo modules, or multichannel modules
// let you access the channels with the ch() method
//
// multichannel modules allocate resources for new channels when required
// stereo module usually have a fixed number of channels ( two ).
// check for module reference of the ch() method for knowing what is possible
//
// in this example a single pdsp::ParameterAmp with multiple channels
// controls the amount of individual LFOs going to individual oscillators
// and a single pdsp::VAFilter is used to filter the voices panned in stereo

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    
    //-------------- PATCHING ----------------------
    oscillators.resize(10);
    pitches.resize( oscillators.size() );
    drift_lfo.resize( oscillators.size() );
    

    for(size_t i=0; i<oscillators.size(); ++i){
        float pan = pdsp::spread( i, oscillators.size(), 0.5f );
        // ch() method let you access signal channels of the filter

        if(i % 2 == 0 || i == 0) {
            oscillators[i].out_pulse() * pdsp::panL(pan) >> filter.ch(0) >> chorus.ch(0); 
            oscillators[i].out_pulse() * pdsp::panR(pan) >> filter.ch(1) >> chorus.ch(1);
            PWMlfo.out_sine() >> pwmAmt.ch(0) >> oscillators[i].in_pw();
                                       pw_mod >> oscillators[i].in_pw();
            // oscillators[i].in_pw() >> pw_mod.ch(0);
            
        } else {
            oscillators[i].out_sine() * pdsp::panL(pan) >> filter.ch(0) >> chorus.ch(0); 
            oscillators[i].out_sine() * pdsp::panR(pan) >> filter.ch(1) >> chorus.ch(1); 
        }
        // ch() method let you access signal channels of the drift_amt module
        1.5f >> drift_lfo[i].out_random() >> drift_amt.ch(i) >> oscillators[i].in_pitch();     
                                                  pitches[i] >> oscillators[i].in_pitch();

        // add vibrato by periodically (LFO) altering the pitch
        vibratoLfo.out_triangle() >> vibratoAmt.ch(0) >> oscillators[i].in_pitch();
    }

    
    
    // the control input is the same for al the filter channels
    cutoff_ctrl.enableSmoothing(100.0f); // 100ms smoothing
    cutoff_ctrl >> filter.in_cutoff(); 
    reso_ctrl >> filter.in_reso(); 
    

    // dry channel
    filter.ch(0) >> mixL.in_A();
    filter.ch(1) >> mixR.in_A();

    // wet channel (with effects)
    chorus.ch(0) >> delay.ch(0) >> mixL.in_B();
    chorus.ch(1) >> delay.ch(1) >> mixR.in_B();

    // route the dry/wet mixer through the master fader and finally to Audio Out
    mixL >> gain.ch(0) >> engine.audio_out(0);
    mixR >> gain.ch(1) >> engine.audio_out(1);


    mixAmount >> mixL.in_fade();
    mixAmount >> mixR.in_fade();

    chorusSpeed >> chorus.in_speed();
    chorusDepth >> chorus.in_depth();
    

    pwmSpeed >> PWMlfo.in_freq(); 

    filterSpeed >> filterLfo.in_freq();
    filterLfo.out_sine() >> filterAmt.ch(0) >> filter.in_cutoff();

    vibratoSpeed >> vibratoLfo.in_freq();

        
        
    // --------------- GUI -------------------------- 
    gui.setup("gui", "settings.xml", 20, 20);

    int basePitch = 60;
    int chord [] = { 0, 0, 3, 3, 7, 7, 10, 10, 14, 14 };

    oscUI.setName("oscillators");


    for(size_t i=0; i<pitches.size(); ++i){

        // int nextPitch;
        // if(i % 2 == 0 || i == 0) {
        //     nextPitch = basePitch + chord[i];
        // } else {
        //     nextPitch = basePitch + chord[i-1];
        // }

        oscUI.add(pitches[i].set("pitch "+ofToString(i)+" coarse", basePitch + chord[i], 24, 96) );
        oscUI.add(pitches[i].set("pitch "+ofToString(i)+" fine", 0.0f, -0.5f, 0.5f) );
    }
    gui.add( oscUI );
    gui.getGroup("oscillators").minimize();
    
    // the control parameter for drift_amt is the same for all the channels
    gui.add( drift_amt.set("oscillator drift", 0.13f, 0.0f, 0.5f) ); 
    gui.add( cutoff_ctrl.set("filter cutoff", 80, 20, 136) );
    gui.add( reso_ctrl.set("filter reso", 0.15f, 0.0f, 1.0f ) );

    gui.add( pw_mod.set("pulse width modulation", 0.5f, 0.0f, 1.0f ) );
    gui.add( pwmAmt.set("pwm amount", 0.2f, 0.0f, 0.5f) );
    gui.add( pwmSpeed.set("pwm speed (hz)", 0.3f, 0.005f, 1.5f));

    gui.add( filterAmt.set("filter LFO amount", 10, 0, 80) );
    gui.add( filterSpeed.set("filter LFO speed (hz)", 0.1f, 0.0f, 1.0f));
    
    gui.add( vibratoAmt.set("vibrato amount", 0.15f, 0.0f, 1.0f) );
    gui.add( vibratoSpeed.set("vibrato speed (hz)", 7.0f, 4.0f, 10.0f));


    chorusUI.setName("chorus parameters");
    chorusUI.add(chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 1.0f));
    chorusUI.add(chorusDepth.set("depth (ms)", 3.5f, 1.0f, 10.0f));
    gui.add( chorusUI );
    gui.add( delay.parameters );

    gui.add( mixAmount.set("dry/wet", 0.5f, 0.0f, 1.0f) ); 
    gui.add( gain.set("gain", -12, -48, 12) ); 
    gain.enableSmoothing(50.f);
    
    //gui.loadFromFile("settings.xml");
    
    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(1); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

    ofLog(OF_LOG_NOTICE, "440Hz : " + ofToString(pdsp::f2p(440)));
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
