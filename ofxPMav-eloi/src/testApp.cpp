#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup()
{	
	float duration = 7.0;
	freeze=false;
	
	// video pipeline
	//////////////////
	vGrabber.initGrabber(1280,720);
	vGrabber.setDeviceID(36);
	vGrabber.setFps(25);
	vBuffer.setup(vGrabber, 175);	

	aBufferSize=1024;
	aSampleRate=96000;
	soundStream.listDevices();
	soundStream.setDeviceID(7);
	soundStream.setup(2,2,aSampleRate,aBufferSize,2);
	soundStream.setInput(this);
	soundStream.setOutput(this);
	
	// audio pipeline 
	///////////////////
	audioSetupFinished=false;	
	aGrabber.setFps(float(aSampleRate)/float(aBufferSize));
	aBuffer.setup(aGrabber,7.0,aSampleRate,aBufferSize,2);
	
	// avRenderer
	//////////////
	avRenderer.setup(vBuffer,aBuffer);
	audioSetupFinished=true;
	
	
	
	// general stuff
	/////////////////
	ofBackground(0);
	//ofSetVerticalSync(true);
	
	// osc setup 
	/////////////
	int oscPort = 12345;
	receiver.setup(oscPort);
	sender.setup("localhost",oscPort);
	

	// gui
	guiPlay=false;
	guiFreeze=false;
	gui.addSlider("delayMs",guiDelay,0.0,7000.0);
	gui.addSlider("inMs",guiIn,0.0,7000.0);
	gui.addSlider("outMs",guiOut,0.0,7000.0);
	gui.addSlider("lengthMs",guiLength,60,7000.0);
	gui.addToggle("freeze",guiFreeze);
	gui.addToggle("play",guiPlay);
	gui.show();
}

//--------------------------------------------------------------
void testApp::update()
{
	vGrabber.update();
	updateOsc();
	
	//update for gui to work ?
	avRenderer.setDelayMs(float(guiDelay));
	avRenderer.setInMs(float(guiIn));
	avRenderer.setOutMs(float(guiOut));
	avRenderer.setOutMs(float(guiOut));
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255,255,255);	
	avRenderer.draw(0,0,1280,720);

	// draw buffer draw limits
	ofSetColor(100);
	ofSetLineWidth(1.0);
	ofLine(PMDRAWSPACING,600,PMDRAWSPACING,800);
	ofLine(ofGetWidth()-PMDRAWSPACING,600,ofGetWidth()-PMDRAWSPACING,800);
	
	float factorFR = (ofGetFrameRate()/60.0);
	ofSetColor(255*(1.0-factorFR),255*(factorFR*factorFR*factorFR),0);
	ofDrawBitmapString("FPS: " + ofToString(int(ofGetFrameRate())) 
					   + " || cameraBuffer FPS " + ofToString(vBuffer.getRealFPS()) 
					   + " || audio Grabber FPS " + ofToString(float(aGrabber.getFps()))
					   + " || Num AudioFrames " + ofToString(aBuffer.size())
					   + " || Total Samples " +ofToString(aBuffer.getMaxSizeInSamples())
					   + " || In " +ofToString(avRenderer.getAudioHeader()->getInSamples())
					   + " || Out " +ofToString(avRenderer.getAudioHeader()->getOutSamples())
					   + " || Length " +ofToString(avRenderer.getAudioHeader()->getLengthSamples())
					   + " || VideoOffsetMs " +ofToString(avRenderer.getVideoOffsetInMs())
					   + "\n|| Video Delays Ms " +ofToString(avRenderer.getVideoHeader()->getDelayMs())
					   ,20,ofGetHeight()-20);
	//ofDrawBitmapString("VideoFrame pool size: " + ofToString(VideoFrame::getPoolSize(VideoFormat(1280,720,3))),520,ofGetHeight()-20);
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key) {
		case 's':
			vGrabber.videoSettings();
			break;
		case ' ':
			if(guiFreeze)
			{
				freeze=true;
				vBuffer.stop();
				aBuffer.stop();
				
			}
			else if (freeze)
			{
				freeze=false;
				vBuffer.resume();
				aBuffer.resume();
			}
			
			if(guiPlay)
			{
				avRenderer.setPlaying(true);
			}
			else
			{
				avRenderer.setPlaying(false);
			}

			avRenderer.setLengthMs(float(guiLength));

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
}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels)
{ 
	if (audioSetupFinished==true)
	{
		aGrabber.audioReceived(input,bufferSize,nChannels);
	}
}
//--------------------------------------------------------------

void testApp::audioRequested (float * output, int bufferSize, int nChannels)
{
    if(audioSetupFinished==true)
	{
//		// get the next audio frame, apply a cosine envelope
//		// and copy to the sound card buffer
//		AudioFrame * frame= avRenderer.getAudioHeader()->getNextAudioFrame();
//		memcpy(output,frame->getAudioFrame(),sizeof(float)*bufferSize*nChannels);
//		frame->release();
		
		avRenderer.audioRequested(output,bufferSize,nChannels);
	}
	
}
	
	

