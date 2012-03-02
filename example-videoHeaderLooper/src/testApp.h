#pragma once

#include "ofMain.h"
#include "ofxPlaymodes.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxPm::VideoGrabber			grabber;
		ofxPm::VideoBuffer			buffer;
		ofxPm::HeaderLooperRenderer	renderer;
	
	// audio
	ofxPm::AudioGrabber	aGrabber;
	ofxPm::AudioBuffer aBuffer;
	ofxPm::AudioHeader aHeader;
	int aBufferSize;
	int aSampleRate;
	int aNumChannels;
	ofSoundStream	soundStream;
	//void audioIn(float * input, int bufferSize, int nChannels); 
	void audioIn(float * input, int bufferSize, int nChannels);
	//void audioOut (float * output, int bufferSize, int nChannels, );
	void audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
	//---------------------------------------------------
	// testApp var's
	bool					freeze;
	float					delayHeader;

	//osc eloi
	ofxOscReceiver			receiver;
	ofxOscSender			sender;
	void					updateOsc();
	
	
};
