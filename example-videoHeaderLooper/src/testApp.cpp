#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup()
{	
	float duration = 7.0;
	audioSetupFinished=false;

	// audio pipeline 
	aBufferSize=512;
	aSampleRate=96000;
	

//#define AUDIO_BUFFER_NUM_FRAMES 

	aGrabber.setFps(float(aSampleRate)/float(aBufferSize));
	aBuffer.setup(aGrabber,7.0*(float(aSampleRate)/float(aBufferSize)));
//	aBuffer.setup(aGrabber,656);
//	aBuffer.stop();
	aHeader.setup(aBuffer);
	
	
	soundStream.listDevices();
	soundStream.setDeviceID(8);
	soundStream.setup(2,2,aSampleRate,aBufferSize,4);
	soundStream.setInput(this);
	soundStream.setOutput(this);
	
	// video pipeline
	grabber.initGrabber(1280,720);
	grabber.setDeviceID(22);
	// need to override like this to have deisred effect
	grabber.setFps(25);
	buffer.setup(grabber, 175);	
//	buffer.stop();
	renderer.setup(buffer);
	
	
	aHeader.linkToVideoHeader(*renderer.getHeader());
	
	freeze=true;
	
	audioSetupFinished=true;
	
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
	// draw buffer draw limits
	ofSetColor(100);
	ofSetLineWidth(1.0);
	ofLine(PMDRAWSPACING,600,PMDRAWSPACING,800);
	ofLine(ofGetWidth()-PMDRAWSPACING,600,ofGetWidth()-PMDRAWSPACING,800);

	buffer.draw();
	aBuffer.draw();
	aHeader.draw();
	
	float factorFR = (ofGetFrameRate()/60.0);
	ofSetColor(255*(1.0-factorFR),255*(factorFR*factorFR*factorFR),0);
	ofDrawBitmapString("FPS: " + ofToString(int(ofGetFrameRate())) + " | aGrabber FPS " + ofToString(float(aGrabber.getFps())) ,20,ofGetHeight()-20);
	//ofDrawBitmapString("VideoFrame pool size: " + ofToString(VideoFrame::getPoolSize(VideoFormat(1280,720,3))),520,ofGetHeight()-20);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key) {
		case 's':
			grabber.videoSettings();
			break;
		case ' ':
			
			freeze=!freeze;
			if(freeze) 
			{
				buffer.stop();
				aBuffer.stop();	
			}
			else 
			{
				buffer.resume();
				aBuffer.resume();	
				
			}

		default:
			break;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float factor = float(x)/float(ofGetWidth());
	renderer.getHeader()->setDelayMs(int(factor*7000));
	aHeader.setDelayMs(int(factor*7000));

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
			aHeader.setDelayMs(int(value));
			delayHeader = value;
		}
		if ( m.getAddress() == "/speed" )
		{
			renderer.getHeader()->setSpeed(value);
			aHeader.setSpeed(value);
		}
		if ( m.getAddress() == "/opacity" )
		{
			renderer.getHeader()->setOpacity(int(value*255.0));
		}
		if ( m.getAddress() == "/inPoint" )
		{
			renderer.getHeader()->setInMs(value);
			aHeader.setInMs(value);
			printf("testApp :: inP %f\n",value);
		}
		if ( m.getAddress() == "/outPoint" )
		{
			renderer.getHeader()->setOutMs(value);
			aHeader.setOutMs(value);
		}
		if ( m.getAddress() == "/freeze" )
		{
			if(value==1.0f)
			{
				freeze=true;
				buffer.stop();
				aBuffer.stop();
			}
			else if (value==0.0f)
			{
				freeze=false;
				buffer.resume();
				aBuffer.resume();
			}
		}
		if ( m.getAddress() == "/play" )
		{
			if(value==1.0f)
			{
				renderer.getHeader()->setPlaying(true);
				aHeader.setPlaying(true);
			}
			else if (value==0.0f)
			{
				renderer.getHeader()->setPlaying(false);
				aHeader.setPlaying(false);
			}
		}
		if ( m.getAddress() == "/loopStart" )
		{
			if(value==1.0f)
			{
				renderer.getHeader()->setLoopToStart();
				aHeader.setLoopToStart();
			}
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
		// get the next audio frame, apply a cosine envelope
		// and copy to the sound card buffer
		AudioFrame * frame= aHeader.getNextAudioFrame();
		memcpy(output,frame->getAudioFrame(),sizeof(float)*bufferSize*nChannels);
		frame->release();
	}
	
}
	
	

