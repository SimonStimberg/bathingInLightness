
#include "synth.h"

void PolySynth::setup( vector <int> pitches ){

    int numVoices = pitches.size();
    
    voices.resize( numVoices );
    voiceLevels.resize( numVoices );
    voiceFilterCutoffs.resize( numVoices );
    
    for( size_t i=0; i<voices.size(); ++i ){
        voices[i].setup( *this, i, pitches[i]);
    }   

    masterBus.channels(4);
    gain.channels(4);
    
    patch();
    setUI();
    
}
 

void PolySynth::patch() {

    //-------------- PATCHING ----------------------
    for(int i=0; i<voices.size(); ++i){

         // connect each voice to the master gain (master bus)
        voices[i].out("front_L") >> masterBus.ch(0);
        voices[i].out("front_R") >> masterBus.ch(1);
        voices[i].out("back_L")  >> masterBus.ch(2);
        voices[i].out("back_R")  >> masterBus.ch(3);

    }

     500 >> loCutF.in_freq();

    //  chorus.ch(0) >> delay.ch(0) >> comp.ch(0) >> loCut.ch(0) >> gain.ch(0) >> engine.audio_out(0);
    //  chorus.ch(1) >> delay.ch(1) >> comp.ch(1) >> loCut.ch(1) >> gain.ch(1) >> engine.audio_out(1);

    // masterBus.ch(0) >> chorusF.ch(0) >> delayF.ch(0) >> gain.ch(0) >> engine.audio_out(0);
    // masterBus.ch(1) >> chorusF.ch(1) >> delayF.ch(1) >> gain.ch(1) >> engine.audio_out(1);
    // masterBus.ch(2) >> chorusB.ch(0) >> delayB.ch(0) >> gain.ch(2) >> engine.audio_out(2);
    // masterBus.ch(3) >> chorusB.ch(1) >> delayB.ch(1) >> gain.ch(3) >> engine.audio_out(3);

    // with low cut and compressor
    masterBus.ch(0) >> chorusF.ch(0) >> delayF.ch(0) >> compF.ch(0) >> loCutF.ch(0) >> overdriveFL >> gain.ch(0) >> engine.audio_out(0);
    masterBus.ch(1) >> chorusF.ch(1) >> delayF.ch(1) >> compF.ch(1) >> loCutF.ch(1) >> overdriveFR >> gain.ch(1) >> engine.audio_out(1);
    masterBus.ch(2) >> chorusB.ch(0) >> delayB.ch(0) >> compB.ch(0) >> loCutB.ch(0) >> overdriveBL >> gain.ch(2) >> engine.audio_out(2);
    masterBus.ch(3) >> chorusB.ch(1) >> delayB.ch(1) >> compB.ch(1) >> loCutB.ch(1) >> overdriveBR >> gain.ch(3) >> engine.audio_out(3);

        

     //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(5); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setChannels(0, 4);
    engine.setup( 44100, 512, 3); 

}


void PolySynth::setUI() {

    gui.setup("SYNTH");
    gui.add( gain.set("master gain", 0, -64, 12) ); 
    gain.enableSmoothing(50.f);
    
    uiOsc.setName("oscillator control");
    uiOsc.add( oscMix.set("oscillator mix", 0.5f, 0.0f, 1.0f) );
    uiOsc.add( drift_amt.set("oscillator drift", 0.13f, 0.0f, 0.5f) ); 
    uiOsc.add( vibratoAmt.set("vibrato amount", 0.15f, 0.0f, 1.0f) );
    uiOsc.add( vibratoSpeed.set("vibrato speed (hz)", 7.0f, 4.0f, 10.0f));

        osc1.setName("osc 1 (pulse)");
        osc1.add(pw.set("pulse width", 0.5f, 0.0f, 1.0f) );
        osc1.add(pwmAmt.set("pwm amount", 0.2f, 0.0f, 0.5f) );
        osc1.add(pwmSpeed.set("pwm speed (hz)", 0.3f, 0.005f, 1.5f));

        osc2.setName("osc 2 (sine)");
        osc2.add(octaveShift.set("octave", -1, -1, 2) );
        osc2.add(fineTune.set("fine tune", 0.0f, -0.5f, 0.5f) );
        
        uiOsc.add( osc1 );
        uiOsc.add( osc2 );

    uiFilter.setName("filter");
    uiFilter.add(cutoff.set("cutoff pitch", 80, 20, 136));
    uiFilter.add(reso.set("resonance", 0.15f, 0.0f, 1.0f) );
    uiFilter.add( filterModAmt.set("LFO mod amount", 10, 0, 80) );
    uiFilter.add( filterModSpeed.set("LFO speed (hz)", 0.1f, 0.0f, 1.0f));
    cutoff.enableSmoothing(100.0f);   

    gui.add( uiOsc );
    gui.add( uiFilter );
    gui.add( chorusF.parameters );
    gui.add( delayF.parameters );
    gui.setPosition(20, 20);


    guiVoices.setup("voice levels");
    for( size_t i=0; i<voices.size(); ++i ){
        guiVoices.add(voiceLevels[i].set("voice "+ofToString(i), 1.0f, 0.0f, 1.0f) );
        voiceLevels[i].enableSmoothing(50.0f);
    }
    guiVoices.setPosition(260, 20);

}


void PolySynth::setVoiceLevels(vector <float> & levels) {
    if (voices.size() == levels.size()) {
        
        for( size_t i=0; i<voices.size(); ++i ){

            float newLevel = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., 0., 0.5);
            voiceLevels[i].set(newLevel);

            float newCutoff = ofMap(ofClamp(levels[i], 0., 1.), 0., 1., -20., 0.);  // values are being apparently not overwritten but added to the overall value
            voiceFilterCutoffs[i].set(newCutoff);
            
        }   
        
    }

}


void PolySynth::setVoicePans(vector <glm::vec2> & panPos) {
    
    if (voices.size() == panPos.size()) {   

        for( size_t i=0; i<voices.size(); ++i ){

            panPos[i].x >> voices[i].pan_front();
            panPos[i].x >> voices[i].pan_back();
            panPos[i].y >> voices[i].pan_depth();
        }   
        
    }

}





void PolySynth::Voice::setup( PolySynth & ui, int v, int pitch){

    addModuleInput("panFront", panFront.in_pan());
    addModuleInput("panBack", panBack.in_pan());
    addModuleInput("panDepth", panDepth.in_pan());

    addModuleInput("pitch", voicePitch);
    addModuleInput("cutoff", filter.in_cutoff() );
    // addModuleOutput("signal", voiceAmp);
    addModuleOutput("front_L", panFront.out_L() );
    addModuleOutput("front_R", panFront.out_R() );
    addModuleOutput("back_L", panBack.out_L() );
    addModuleOutput("back_R", panBack.out_R() );

    // SIGNAL PATH   
    mix >> filter >> voiceAmp >> panDepth;
    panDepth.out_L() >> panFront;
    panDepth.out_R() >> panBack;

    // OSC 1 - Pulse Wave
                                        osc1.out_pulse() >> mix.in_A();
                                                   pitch >> osc1.in_pitch();
    1.5f >> driftLFO1.out_random() >> ui.drift_amt.ch(v) >> osc1.in_pitch();   
        vibratoLfo.out_triangle() >> ui.vibratoAmt.ch(v) >> osc1.in_pitch();  

    // OSC 2 - Sine Wave
                                         osc2.out_sine() >> mix.in_B();
                                                   pitch >> osc2.in_pitch();
    1.5f >> driftLFO2.out_random() >> ui.drift_amt.ch(v) >> osc2.in_pitch();
                                     ui.octaveShift * 12 >> osc2.in_pitch();  
                                             ui.fineTune >> osc2.in_pitch();
        vibratoLfo.out_triangle() >> ui.vibratoAmt.ch(v) >> osc2.in_pitch();       



    

    
    // MODULATIONS AND CONTROL
    lfo.out_sine() >> ui.pwmAmt.ch(v) >> osc1.in_pw();
                                ui.pw >> osc1.in_pw();
                          ui.pwmSpeed >> lfo.in_freq(); 

                      ui.vibratoSpeed >> vibratoLfo.in_freq();


    // FILTER STUFF
                                           ui.cutoff >> filter.in_cutoff(); 
                                             ui.reso >> filter.in_reso();
                                   ui.filterModSpeed >> filterModLfo.in_freq();
    filterModLfo.out_sine() >> ui.filterModAmt.ch(v) >> filter.in_cutoff();


    //  voiceTrigger >> modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) >> ui.modAmt.ch(v) >> filter.in_cutoff(); 

    
    // (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();
                    ui.voiceLevels[v] >> voiceAmp.in_mod();
             ui.voiceFilterCutoffs[v] >> filter.in_cutoff(); 


    
    

        ui.oscMix >> mix.in_fade();  
        // -1.0f >> pan.in_pan();
}





// float PolySynth::Voice::meter_mod_env() const{
//     return modEnv.meter_output();
// }

// float PolySynth::Voice::meter_pitch() const{
//     return oscillator.meter_pitch();
// }
