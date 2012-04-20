#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSimpleGuiToo.h"



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
	
	//osc eloi
	ofxOscSender			sender;
	void					updateOsc();

	// needed for gui ?À
	float					guiDelay;
	float					guiLastDelay;

	float					guiIn;
	float					guiLastIn;

	float					guiLength;
	float					guiLastLength;

	float					guiSpeed;
	float					guiLastSpeed;

	bool					guiFreeze;
	bool					guiLastFreeze;

	bool					guiPlay;
	bool					guiLastPlay;
	
	bool					guiResync;
	bool					guiLastResync;
	
};
