#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	grabber.initGrabber(640,480);
	vBuffer.setup(grabber,400);
	vHeader.resize(5);
	vRenderer.resize(5);
	for(int i=0;i<(int)vHeader.size();i++){
		vHeader[i].setup(vBuffer);
		vHeader[i].setDelay(500000*i);
		vRenderer[i].setup(vHeader[i]);
	}
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	vBuffer.draw();
	for(int i=0;i<5;i++){
		vHeader[i].draw();

	    ofSetColor(255,255,255,255/5);
		vRenderer[i].draw();
	}
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
