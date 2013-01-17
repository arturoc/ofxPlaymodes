#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup(){
	gui.setup();
	gui.add(fps.setup("fps",30,1,200));

	grabber.initGrabber(640,480);
	vRate.setup(grabber,fps);
	vBuffer.setup(vRate,400);
	vHeader.setup(vBuffer);
	ofPixels blackPixels;
	blackPixels.allocate(640,480,3);
	blackPixels.set(0);
	for(int i=0;i<400;i++){
		VideoFrame videoFrame = VideoFrame::newVideoFrame(blackPixels);
		videoFrame.getTextureRef();
		vBuffer.newVideoFrame(videoFrame);
	}
	vHeader.setDelayMs(1000);

	vRenderer1.setup(grabber);
	vRenderer2.setup(vHeader);
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	vRate.setFps(fps);
	vHeader.setFps(fps);
	vHeader.setDelayMs(1000);
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
			+ " || cameraBuffer FPS " + ofToString(vBuffer.getRealFPS()),20,ofGetHeight()-80);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
