
#include "synth.h"

void PolySynth::setup( vector <int> pitches ){

    int numVoices = pitches.size();
    
    voices.resize( numVoices );
    voiceLevels.resize( numVoices );
    
    for( size_t i=0; i<voices.size(); ++i ){
        voices[i].setup( *this, i, pitches[i]);
    }   
    
    patch();
    setUI();
    
}


void PolySynth::patch() {

    //-------------- PATCHING ----------------------
    for(int i=0; i<voices.size(); ++i){

         // connect each voice to the master gain (master bus)
         voices[i] >> gain.ch(0);
         voices[i] >> gain.ch(1);
     }

     gain.ch(0) >> engine.audio_out(0);
     gain.ch(1) >> engine.audio_out(1);


     //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(1); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}


void PolySynth::setUI() {

    gui.setup("panel");
    gui.add( gain.set("gain", -12, -48, 12) ); 
    gain.enableSmoothing(50.f);
    
    uiOsc.setName("oscillator control");
    uiOsc.add( drift_amt.set("oscillator drift", 0.13f, 0.0f, 0.5f) ); 
    uiOsc.add( oscMix.set("oscillator mix", 0.5f, 0.0f, 1.0f) );

    ui.setName("synth parameters");
    ui.add(pw.set("pulse width", 0.5f, 0.0f, 1.0f) );
    ui.add(pwmAmt.set("pwm amount", 0.3f, 0.0f, 0.5f) );
    ui.add(pwmSpeed.set("pwm speed (hz)", 0.5f, 0.005f, 1.5f));
    cutoff.enableSmoothing(200.0f);
    ui.add(cutoff.set("cutoff pitch", 82, 20, 136));
    ui.add(reso.set("resonance", 0.0f, 0.0f, 1.0f) );
    

    uiVoices.setName("voice levels");
    for( size_t i=0; i<voices.size(); ++i ){
        uiVoices.add(voiceLevels[i].set("voice "+ofToString(i), 1.0f, 0.0f, 1.0f) );
        voiceLevels[i].enableSmoothing(50.0f);
    }

    gui.add( uiOsc );
    gui.add( ui );
    gui.add( uiVoices );
    gui.setPosition(20, 20);

}





void PolySynth::Voice::setup( PolySynth & ui, int v, int pitch){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", voicePitch);
    addModuleInput("cutoff", filter.in_cutoff() );
    addModuleOutput("signal", voiceAmp);


    // SIGNAL PATH

    // OSC 1 - Pulse Wave
                                        osc1.out_pulse() >> mix.in_A();
                                                   pitch >> osc1.in_pitch();
    1.5f >> driftLFO1.out_random() >> ui.drift_amt.ch(v) >> osc1.in_pitch();     

    // OSC 2 - Sine Wave
                                         osc2.out_sine() >> mix.in_B();
                                                   pitch >> osc2.in_pitch();
    1.5f >> driftLFO2.out_random() >> ui.drift_amt.ch(v) >> osc2.in_pitch();     


    mix >> filter >> voiceAmp;

    
    // MODULATIONS AND CONTROL
    lfo.out_sine() >> ui.pwmAmt.ch(v) >> osc1.in_pw();
                                ui.pw >> osc1.in_pw();
                          ui.pwmSpeed >> lfo.in_freq(); 
                                                             
                            ui.cutoff >> filter.in_cutoff(); 
                              ui.reso >> filter.in_reso();
    //  voiceTrigger >> modEnv.set(400.0f, 600.0f, 1.0f, 800.0f) >> ui.modAmt.ch(v) >> filter.in_cutoff(); 

    
    // (voiceTrigger >> (ampEnv.set(200.0f, 400.0f, 1.0f, 600.0f ) * 0.1f)) >> voiceAmp.in_mod();
                    ui.voiceLevels[v] >> voiceAmp.in_mod();


    
    

        ui.oscMix >> mix.in_fade();  
}


// float PolySynth::Voice::meter_mod_env() const{
//     return modEnv.meter_output();
// }

// float PolySynth::Voice::meter_pitch() const{
//     return oscillator.meter_pitch();
// }
