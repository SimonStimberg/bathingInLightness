
#include "synthFX.h"


// DELAY
StereoDelay::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void StereoDelay::patch() {

    channels.resize(2);
    
    addModuleInput("L", channels[0] );
    addModuleInput("R", channels[1] );
    addModuleOutput("L", channels[0] );
    addModuleOutput("R", channels[1] );

    // patching dry and wet
    channels[0].input >> inputFader.ch(0) >> mixL.in_A();;
    channels[1].input >> inputFader.ch(1) >> mixR.in_A();;

    channels[0].input >> inputFader.ch(0) >> delay0 >> mixL.in_B();;
    channels[1].input >> inputFader.ch(1) >> delay1 >> mixR.in_B();;

    // routing the d/w mix to the output
    mixL >> outputFader.ch(0) >> channels[0].output;
    mixR >> outputFader.ch(1) >> channels[1].output;

    mixControl >> mixL.in_fade();
    mixControl >> mixR.in_fade();


    time * (1.f/16.f) >> lDelayTimeControl >> delay0.in_time();
    time * (1.f/16.f) >> rDelayTimeControl >> delay1.in_time();

    lFeedbackControl >> delay0.in_feedback();
    rFeedbackControl >> delay1.in_feedback();
    dampingControl >> delay0.in_damping();
    dampingControl >> delay1.in_damping();

    parameters.setName("stereo delay");

    parameters.add( inputFader.set("input gain", 0, -48, 12) );
    
    parameters.add( lDelayTimeControl.set("L 16th time", 5, 1, 16) );
    parameters.add( lFeedbackControl.set("L feedback", 0.14f, 0.0f, 1.0f));
    parameters.add( rDelayTimeControl.set("R 16th time", 3, 1, 16) );
    parameters.add( rFeedbackControl.set("R feedback", 0.156f, 0.0f, 1.0f));   
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f));  

    parameters.add( mixControl.set("dry / wet", 0.1f, 0.0f, 1.0f));  
   
    parameters.add( outputFader.set("output gain", 0, -48, 12) );

    inputFader.enableSmoothing(50.f);
    outputFader.enableSmoothing(50.f);
    mixControl.enableSmoothing(50.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lFeedbackControl.enableSmoothing(50.f);
    rFeedbackControl.enableSmoothing(50.f);

}

pdsp::Patchable & StereoDelay::ch( int index ){
    index = index%2; // wrap to stereo 
    return channels[index];
}



// CHORUS
StereoChorus::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void StereoChorus::patch() {

    channels.resize(2);
    
    addModuleInput("L", channels[0] );
    addModuleInput("R", channels[1] );
    addModuleOutput("L", channels[0] );
    addModuleOutput("R", channels[1] );

    // patching dry and wet
    channels[0].input >> mixL.in_A();;
    channels[1].input >> mixR.in_A();;

    channels[0].input >> chorus.ch(0) >> mixL.in_B();;
    channels[1].input >> chorus.ch(1) >> mixR.in_B();;

    // routing the d/w mix to the output
    mixL >> channels[0].output;
    mixR >> channels[1].output;

    mixControl >> mixL.in_fade();
    mixControl >> mixR.in_fade();


    chorusSpeed >> chorus.in_speed();
    chorusDepth >> chorus.in_depth();
    chorusDelay >> chorus.in_delay();
    

    parameters.setName("stereo chorus");

    parameters.add( chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 1.0f) );
    parameters.add( chorusDepth.set("depth (ms)", 3.5f, 1.0f, 10.0f) );
    parameters.add( chorusDelay.set("delay (ms)", 80, 0, 400) );

    parameters.add( mixControl.set("dry / wet", 0.05f, 0.0f, 1.0f) );  

    mixControl.enableSmoothing(50.f);

}

pdsp::Patchable & StereoChorus::ch( int index ){
    index = index%2; // wrap to stereo 
    return channels[index];
}