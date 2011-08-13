#ifndef _TEST_APP
#define _TEST_APP

//uncomment this to read from camera
#define PLAYMODES_LIVE_VIDEO

//uncomment this to get sound buffers
//#define PLAYMODES_SOUND


#include "ofMain.h"
#include "Playmodes.h"

/*
#include "ofxWidgets.h"

#include "grabbers/VideoGrabber.h"
#include "grabbers/FileGrabber.h"
#include "grabbers/AudioGrabber.h"

#include "buffers/AudioBuffer.h"
#include "buffers/VideoBuffer.h"

#include "AVHeaders/AudioHeader.h"

#include "oscControl/OscInterface.h"

#include "frames/envelopes/RaisedCosineBellEnvelope.h"

#include "renderers/RecLoopRender.h"
#include "renderers/MultixRenderer.h"
#include "filters/FreeFrameFilter.h"
*/

class testApp : public ofSimpleApp{

public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

	

#ifdef PLAYMODES_LIVE_VIDEO
    VideoGrabber videoInput;
#else
    FileGrabber videoInput;
#endif

    AudioGrabber audioInput;

    VideoBuffer * videoBuffer;
    myAudioBuffer * audioBuffer;
    void newAudioFrame(AudioFrame frame);

    AudioHeader * audioHeader;

    float aFPS;
    float vFPS;
    float appFPS;
    int updateStats;

    char msg[1024];

    //void audioRequested 	(float * output, int bufferSize, int nChannels);

    //ofxWFrame gui;

    OscInterface * oscInterface;
    //RaisedCosineBellEnvelope envelope;

    int background;

    bool drawGUI;
    bool drawStats;

	
    //RecLoopRender  * recloop;
    
	MultixRenderer * multix;
    
	//FreeFrameFilter* FFFilter;

	 

	 };

	
#endif

