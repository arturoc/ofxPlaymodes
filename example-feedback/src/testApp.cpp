#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup(){
	gui.setup();
	gui.add(fps.setup("fps",30,1,60));
	gui.add(delay.setup("delay",1000,0,2000));

	//grabber.setDeviceID(1);
	grabber.initGrabber(640,480);
	//grabber.setUseTexture(false);
	vHeader.setup(vBuffer);
	vMixer.setup(grabber,vHeader);
	vRate.setup(vMixer,fps);
	vBuffer.setup(vRate,400,false);
	vHeader.setDelayMs(1000);

	vRenderer1.setup(grabber);
	vRenderer2.setup(vMixer);
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	vRate.setFps(fps);
	vHeader.setFps(fps);
	vHeader.setDelayMs(delay);
}

//--------------------------------------------------------------
void testApp::draw(){
	vBuffer.draw();
	vHeader.draw();
	vRenderer1.draw();
	ofPushMatrix();
	ofTranslate(640,0);
	vRenderer2.draw();
	ofPopMatrix();
	gui.draw();
	ofDrawBitmapString("FPS: " + ofToString(int(ofGetFrameRate()))
			+ " || cameraBuffer FPS " + ofToString(vBuffer.getRealFPS())
			+ " || videoframes pool size: " + ofToString(VideoFrame::getPoolSize(VideoFormat(640,480,3)))
			+ " || total frames: " +ofToString(VideoFrame::getTotalNumFrames()),20,ofGetHeight()-80);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		if(vBuffer.isStopped()){
			vBuffer.resume();
		}else{
			vBuffer.stop();
		}
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
