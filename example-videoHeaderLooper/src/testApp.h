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
		//ofxPm::VideoHeader			header;
		//ofxPm::BasicVideoRenderer	renderer;
		ofxPm::HeaderLooperRenderer	renderer2;

	//---------------------------------------------------
	// testApp var's
	bool					freeze;
	float					delayHeader;

	//osc eloi
	ofxOscReceiver			receiver;
	ofxOscSender			sender;
	void					updateOsc();
	
	
};
