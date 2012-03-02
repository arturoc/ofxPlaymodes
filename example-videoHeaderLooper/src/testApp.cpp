#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup()
{	
	// audio pipeline 
	aBufferSize=512;
	aSampleRate=44100;

//#define AUDIO_BUFFER_NUM_FRAMES 

	aBuffer.setup(aGrabber,7*44100/aBufferSize);
	aHeader.setup(aBuffer);
	
	soundStream.listDevices();
	soundStream.setup(0,2,aSampleRate,aBufferSize,2);
	soundStream.setInput(this);
	
	// video pipeline
	grabber.initGrabber(1280,720);
	//grabber.setDeviceID(22);
	// need to override like this to have deisred effect
	grabber.setFps(25);
	buffer.setup(grabber, 175);	
	renderer.setup(buffer);
	
	// general stuff
	ofBackground(0);
	ofSetVerticalSync(true);
	
	// osc setup 
	int oscPort = 12345;
	receiver.setup(oscPort);
	sender.setup("localhost",oscPort);
	freeze = false;
	
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	updateOsc();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255,255,255);	
	renderer.draw(0,0,1245,700);
	buffer.draw();
	aBuffer.draw();
	ofSetColor(255,128,0);
	ofDrawBitmapString("FPS: " + ofToString(int(ofGetFrameRate())),20,ofGetHeight()-20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key) {
		case 's':
			grabber.videoSettings();
			break;

		default:
			break;
	}

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
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		printf(".");
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		printf("osc in >> %s :: %f\n",m.getAddress().c_str(),m.getArgAsFloat(0));
		float value = m.getArgAsFloat(0);
		
		if ( m.getAddress() == "/delay" )
		{
			renderer.getHeader()->setDelayMs(int(value));
			delayHeader = value;
		}
		if ( m.getAddress() == "/speed" )
		{
			renderer.getHeader()->setSpeed(value);
		}
		if ( m.getAddress() == "/opacity" )
		{
			renderer.getHeader()->setOpacity(int(value*255.0));
		}
		if ( m.getAddress() == "/inPoint" )
		{
			renderer.getHeader()->setInMs(value);
		}
		if ( m.getAddress() == "/outPoint" )
		{
			renderer.getHeader()->setOutMs(value);
		}
		if ( m.getAddress() == "/freeze" )
		{
			if(value==1.0f)
			{
				freeze=true;
				buffer.stop();				
			}
			else if (value==0.0f)
			{
				freeze=false;
				buffer.resume();
			}
		}
		if ( m.getAddress() == "/play" )
		{
			if(value==1.0f)
			{
				renderer.getHeader()->setPlaying(true);
			}
			else if (value==0.0f)
			{
				renderer.getHeader()->setPlaying(false);
			}
		}
		if ( m.getAddress() == "/loopStart" )
		{
			if(value==1.0f)
			{
				renderer.getHeader()->setLoopToStart();
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels)
{  
	aGrabber.audioReceived(input,bufferSize,nChannels);
}