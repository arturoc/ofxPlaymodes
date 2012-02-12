#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	grabber.initGrabber(640,480);
	vBuffer = new VideoBuffer(&grabber);
	for(int i=0;i<5;i++){
		vHeader.push_back(new VideoHeader(vBuffer));
		vHeader[i]->delay = 500000*i;
	}
	for(int i=0;i<5;i++){
		vRenderer.push_back(new BasicVideoRenderer(vHeader[i]));
	}
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	vBuffer->draw();
	for(int i=0;i<5;i++){
		vHeader[i]->draw();

	    ofSetColor(255,255,255,255/5);
		vRenderer[i]->draw();
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
