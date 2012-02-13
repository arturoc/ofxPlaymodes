#include "testApp.h"
using namespace ofxPm;

//--------------------------------------------------------------
void testApp::setup(){


	ofSetVerticalSync(true);
    //ofSetLogLevel(OF_LOG_VERBOSE);
	videoInput.listDevices();
	videoInput.setDeviceID(0);
	videoInput.initGrabber(640,480);
	videoInput.setUseTexture(false);

    // create the buffer that receive data
    // from videoInput
    videoBuffer.setup(videoInput,400);


    // create a multix renderer over the videoBuffer with 10 headers
    multix.setup(videoBuffer,10);


	multix.setDelayOffset (100);
	multix.setNumHeaders(10);
	multix.setMinmaxBlend(1);
	multix.setTint(ofColor(255,255,255,128));


    // init variables
    vFPS        = 0;
    appFPS      = 0;
    updateStats = 0;
    background  = 0;

    ofBackground(255,255,255);
    drawStats=true;
}
//--------------------------------------------------------------

void testApp::exit(){

	videoInput.close();
}

//--------------------------------------------------------------
void testApp::update(){

	videoInput.update();

    // update the multix state
    multix.update();

    // update the msg every 100 frames
    if(updateStats%100==0){
        vFPS=videoBuffer.getRealFPS();
        appFPS=ofGetFrameRate();
    }
    updateStats++;
    msg = "video real fps: " + ofToString(vFPS) + ", app fps:" + ofToString(appFPS);

    ofBackground(background,background,background);
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255,255,255);
    // draw the multix or rec loop renderers
    multix.draw();


    if(drawStats){
        // draw the representation of the headers and buffers
        videoBuffer.draw();

        ofDrawBitmapString(msg,20,620);

        // draw the multix headers state
        for(int i=0; i < multix.getNumHeaders(); i++){
            VideoHeader * header = multix.getHeader(i);
            if(header!=NULL)
                header->draw();
        }
    }

 
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
/*    switch (key){
        // stop grabbing audio and video to buffers
        case ' ':
        videoBuffer->stop();
        audioBuffer->stop();
        cout << "audio: "<< audioBuffer->size() << "\n";
        cout << "video: "<< videoBuffer->size() << "\n";
        break;

        // resume grabbing audio and video to buffers
        case 'r':
        videoBuffer->resume();
        audioBuffer->resume();
        cout << "audio: "<< audioBuffer->size() << "\n";
        cout << "video: "<< videoBuffer->size() << "\n";
        break;

        case 'g':
            drawGUI=!drawGUI;
            if(!drawGUI){
            	gui.setVisible(false);
            	gui.disable();
            }else{
            	gui.setVisible(true);
            	gui.enable();
            }
        break;

        case 's':
            drawStats=!drawStats;
        break;
    }
*/
 }

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	multix.setNumHeaders(ofMap(y,0,ofGetHeight(),1,100));
	multix.setDelayOffset(ofMap(x,0,ofGetWidth(),0,1000));
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
/*

void testApp::audioRequested 	(float * output, int bufferSize, int nChannels)
{
 // get the next audio frame, apply a cosine envelope
    // and copy to the sound card buffer
    AudioFrame * frame= audioHeader->getNextFrame();
    float speed=audioHeader->speed;
    AudioFrame * envFrame=envelope.applyEnvelope(frame);
    frame->release();
    if(envFrame){
        memcpy(output,envFrame->getAudioFrame(),sizeof(float)*bufferSize*nChannels);
        envFrame->release();

    }
	 
}
 
 */
//--------------------------------------------------------------

