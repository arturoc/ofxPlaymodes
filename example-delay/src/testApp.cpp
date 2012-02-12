#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	grabber.initGrabber(640,480);
	buffer.setup(grabber, 400);
	header.setup(buffer);
	renderer.setup(header);

	ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	renderer.draw();

	header.draw();
	buffer.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if(key==' '){
		if(buffer.isStopped()){
			buffer.resume();
		}else{
			buffer.stop();
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float pct = float(x)/float(ofGetWidth());
	header.setPct(pct);
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
