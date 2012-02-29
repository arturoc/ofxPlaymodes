#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup(){
	grabber.initGrabber(1280,720);
	grabber.setDeviceID(22);
	// need to override like this to have effect
	grabber.setFps(25);
	
	buffer.setup(grabber, 175);
	
	// basic renderer setup
	//header.setup(buffer);
	//renderer.setup(header);
	
	// headerLooperRenderer setup
	renderer2.setup(buffer);
	
	ofBackground(0);
	
	//----------------
	int oscPort = 12345;
	receiver.setup(oscPort);
	sender.setup("localhost",oscPort);
	freeze = false;
	
	ofSetVerticalSync(true);
	//ofSetBackgroundAuto(false);
	ofBackground(0,0,0);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	updateOsc();
}

//--------------------------------------------------------------
void testApp::draw(){
//	if(freeze){
//		ofSetColor(255,0,0);
//		ofRect(0,0,1250,705);
//	}
	
	ofSetColor(255,255,255);
	
	//renderer.draw(0,0,1245,700);
	//renderer2.draw();
	renderer2.draw(0,0,1245,700);
	
	buffer.draw();
	//header.draw();
	
	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),20,ofGetHeight()-20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key) {
		case 's':
			grabber.videoSettings();
			break;
			break;
		case '1':
			//header.setLoopMode(true);
			//header.setSpeed(2.0);
			break;
		case '2':
			//header.setLoopMode(false);
			//header.setSpeed(1.0);
			break;
		case '3':
			buffer.stop();
			break;
		case '4':
			buffer.resume();
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

	//float pct = float(x)/float(ofGetWidth());
	//header.setDelayFrame(int(pct*buffer.size()));

	//float pct = float(x)/float(ofGetWidth());
	//header.setInMs(int(pct*7000.0));
	

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
		
		if ( m.getAddress() == "/delay" )
		{
			float value = m.getArgAsFloat(0);
			printf("osc in .........delay :: %f\n",value);
			
			//header.setDelayMs(int(value));
			//s header.setInMs(7000-delayHeader);
			renderer2.getHeader()->setDelayMs(int(value));
			delayHeader = value;

		}
		if ( m.getAddress() == "/speed" )
		{
			float value = m.getArgAsFloat(0);
			printf("osc in speed :: %f\n",value);
			
			renderer2.getHeader()->setOpacity(int(value*255.0));
			
			//header.setSpeed(value);
			renderer2.getHeader()->setSpeed(value);
		}
		if ( m.getAddress() == "/inPoint" )
		{
			float value = m.getArgAsFloat(0);
			printf("osc in inPoint :: %f\n",value);
			//header.setInPct(value);
			renderer2.getHeader()->setInPct(value);
		}
		if ( m.getAddress() == "/outPoint" )
		{
			float value = m.getArgAsFloat(0);
			//header.setOutPct(value);
			renderer2.getHeader()->setOutPct(value);
			//s header.setOutMs(7000-value);

			printf("osc in outPoint :: %f\n",value);
		}
		if ( m.getAddress() == "/freeze" )
		{
			float value = m.getArgAsFloat(0);
			if(value==1.0f)
			{
				freeze=true;
				buffer.stop();
				//s header.setInMs(7000-delayHeader);
				//s header.setLoopMode(true);
				//header.setSpeed(0.0);
				
			}
			else if (value==0.0f)
			{
				freeze=false;
				buffer.resume();
				//s header.setDelayMs(delayHeader);
				//s header.setLoopMode(false);
				//renderer2.getHeader().setLoopMode(false);
				
			}
			printf("osc in freeze :: %f\n",value);
			
		}
		if ( m.getAddress() == "/play" )
		{
			float value = m.getArgAsFloat(0);
			if(value==1.0f)
			{
				//header.setLoopMode(true);				
				renderer2.getHeader()->setPlaying(true);
			}
			else if (value==0.0f)
			{
				//header.setLoopMode(false);
				renderer2.getHeader()->setPlaying(false);
				
			}
			printf("osc in freeze :: %f\n",value);
			
		}
		if ( m.getAddress() == "/loopStart" )
		{
			float value = m.getArgAsFloat(0);
			if(value==1.0f)
			{
				//header.setLoopToStart();				
				renderer2.getHeader()->setLoopToStart();
			}
		}
		
	}
	
	
}
