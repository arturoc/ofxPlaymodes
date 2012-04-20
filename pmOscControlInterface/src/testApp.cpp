#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{	
	// general stuff
	/////////////////
	ofBackground(0);
	ofSetVerticalSync(true);
	
	// osc setup 
	/////////////
	int oscPort = 12345;
	sender.setup("localhost",oscPort);
	
	// gui
	guiDelay=0.0;
	guiLastDelay=0.0;

	guiIn=0.0;
	guiLastIn=0.0;

	guiLength=0.0;
	guiLastLength=0.0;
	
	guiFreeze = false;
	guiLastFreeze = false;

	guiPlay = false;
	guiLastPlay = false;

	guiResync = false;
	guiLastResync = false;

	guiSpeed = 0.0;
	guiLastSpeed = 0.0;

	gui.addTitle("PM2 OSC TEST INTERFACE");
	gui.addSlider("delayMs",guiDelay,0.0,7000.0);
	gui.addSlider("inMs",guiIn,0.0,7000.0);
	gui.addSlider("lengthMs",guiLength,0.0,7000.0);
	gui.addToggle("freeze",guiFreeze);
	gui.addToggle("play",guiPlay);
	gui.addButton("resync",guiResync);
	gui.addSlider("speed",guiSpeed,-4.0,4.0);
	
	gui.show();
}

//--------------------------------------------------------------
void testApp::update()
{
	if(guiLastDelay!=guiDelay)
	{
		guiLastDelay=guiDelay;
		ofxOscMessage m;
		m.setAddress("/delay");
		m.addFloatArg(guiDelay);
		sender.sendMessage(m);
	}

	if(guiLastIn!=guiIn)
	{
		guiLastIn=guiIn;
		ofxOscMessage m;
		m.setAddress("/inPoint");
		m.addFloatArg(guiIn);
		sender.sendMessage(m);
	}

	if(guiLastLength!=guiLength)
	{
		guiLastLength=guiLength;
		ofxOscMessage m;
		m.setAddress("/length");
		m.addFloatArg(guiLength);
		sender.sendMessage(m);
	}

	if(guiLastFreeze!=guiFreeze)
	{
		guiLastFreeze=guiFreeze;
		ofxOscMessage m;
		m.setAddress("/freeze");
		if(guiFreeze) m.addFloatArg(1.0);
		else m.addFloatArg(0.0);
		sender.sendMessage(m);
	}

	if(guiLastPlay!=guiPlay)
	{
		guiLastPlay=guiPlay;
		ofxOscMessage m;
		m.setAddress("/play");
		if(guiPlay) m.addFloatArg(1.0);
		else m.addFloatArg(0.0);
		sender.sendMessage(m);
	}

	if(guiLastResync!=guiResync)
	{
		guiLastResync=guiResync;
		ofxOscMessage m;
		m.setAddress("/resync");
		if(guiResync) m.addFloatArg(1.0);
		else m.addFloatArg(0.0);
		sender.sendMessage(m);
	}
	
	if(guiLastSpeed!=guiSpeed)
	{
		guiLastSpeed=guiSpeed;
		ofxOscMessage m;
		m.setAddress("/speed");
		m.addFloatArg(guiSpeed);
		sender.sendMessage(m);
	}
	
	if(guiLastResync!=guiResync)
	{
		guiLastResync=guiResync;
		ofxOscMessage m;
		m.setAddress("/resync");
		m.addFloatArg(guiSpeed);
		sender.sendMessage(m);
	}
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::updateOsc()
{
	/*
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		printf(".....................\n");
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		
		printf("OSC in > %s :: %f || time :: %d ms \n",m.getAddress().c_str(),m.getArgAsFloat(0),ofGetElapsedTimeMillis());
		float value = m.getArgAsFloat(0);
		
		if ( m.getAddress() == "/delay" )
		{
			avRenderer.setDelayMs(value);
		}
		if ( m.getAddress() == "/speed" )
		{
			avRenderer.setSpeed(value);
		}
		if ( m.getAddress() == "/opacity" )
		{
			avRenderer.setOpacity(value);			
		}
		if ( m.getAddress() == "/inPoint" )
		{
			avRenderer.setInMs(value);
		}
		if ( m.getAddress() == "/outPoint" )
		{
			avRenderer.setOutMs(value);
		}
		if ( m.getAddress() == "/length" )
		{
			avRenderer.setLengthMs(value);
		}
		if ( m.getAddress() == "/freeze" )
		{
			if(value==1.0f)
			{
				freeze=true;
				vBuffer.stop();
				aBuffer.stop();
			}
			else if (value==0.0f)
			{
				freeze=false;
				vBuffer.resume();
				aBuffer.resume();
			}
		}
		if ( m.getAddress() == "/play" )
		{
			if(value==1.0f)
			{
				avRenderer.setPlaying(true);
			}
			else if (value==0.0f)
			{
				avRenderer.setPlaying(false);
			}
		}
		if ( m.getAddress() == "/loopStart" )
		{
			if(value==1.0f)
			{
				avRenderer.setLoopToStart();
			}
		}
		if ( m.getAddress() == "/offsetVideoMs" )
		{
				avRenderer.setVideoOffsetInMs(int(value));
		}
		if ( m.getAddress() == "/grainTrigger" )
		{
			avRenderer.executeInOut();
		}
	}
*/
}

	
	

