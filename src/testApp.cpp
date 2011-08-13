#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	printf(":: Playmodes2 0.0 - 2011\n");

	ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);

    #ifdef PLAYMODES_LIVE_VIDEO
		videoInput.listDevices();
		videoInput.setDeviceID(0);
		videoInput.initGrabber(CAPTURE_WIDTH,CAPTURE_HEIGHT);
		videoInput.setUseTexture(false);
	#else
		videoInput.loadMovie("fingers.mov");
		videoInput.play();
		videoInput.setLoopState(OF_LOOP_NORMAL);
	#endif

    /// to record sound too
	#ifdef PLAYMODES_SOUND
    ofSoundStreamSetup(2,2,44100,1024,4);
	#endif

    // to use a FF filter
	// can't get FF to compile
    //FFFilter=new FreeFrameFilter(&videoInput, CAPTURE_WIDTH, CAPTURE_HEIGHT);
    //FFFilter->loadPlugin("./data/FishEyeVFX.so");
    //FFFilter->setFFParameter(0,0.001);

    // create the buffers that receive data
    // from audioInput and videoInput
    audioBuffer=new myAudioBuffer(&audioInput);
    videoBuffer=new VideoBuffer(&videoInput);

    // create an audio header that pulls data from the
    // audio buffer
    audioHeader = new AudioHeader(audioBuffer);


    // create a multix renderer over the videoBuffer with 10 headers
    multix = new MultixRenderer(videoBuffer,10);

    ///example of rec loop initialization
    /*
    videoHeader = new VideoHeader(videoBuffer);
    recloop = new RecLoopRender(videoHeader,&videoInput);
    recloop->alpha=255;
    recloop->tintR=255;
    recloop->tintG=255;
    recloop->tintB=255;
    recloop->minmaxBlend=true;
    */

	multix->delayOffset = 100;
	multix->numHeaders = 10;
	multix->minmaxBlend = 1;
	multix->alpha = 128;
/*
    //create gui controls
    gui.addSlider("multix delay offset",&(multix->delayOffset),0,10000);
    gui.addSlider("multix speed offset",&(multix->speedOffset),-2,2);
    //gui.addSlider("audio density",&(audioHeader->density),0,50);
    //gui.addSlider("audio attack",&(envelope.attackPct),0,1);
    //gui.addSlider("audio decay",&(envelope.decayPct),0,1);
    gui.addSlider("multix video in",&(multix->in),0,1);
    gui.addSlider("multix video out",&(multix->out),0,1);
    gui.addToggle("multix loop",&(multix->loopMode));
    gui.addSlider("multix scale",&(multix->scale),0,10);

    gui.addSlider("multix rotZ offset",&(multix->rotZOffset),0,360);
    gui.addSlider("multix rotY offset",&(multix->rotYOffset),0,360);
    gui.addSlider("multix num headers",&(multix->numHeaders),1,300);
    gui.addSlider("background",&(background),0,255);

    gui.addSlider("multix alpha",&(multix->alpha),0,255);

    gui.addSlider("multix z",&(multix->z),-5000,100);
    gui.addSlider("multix anchor z",&(multix->anchorZ),-5000,+5000);

    gui.addSlider("multix xOffset",&(multix->xOffset),0,1000);
    gui.addSlider("multix yOffset",&(multix->yOffset),0,1000);
    gui.addSlider("multix xPos",&(multix->x),-10000,10280);
    gui.addSlider("multix yPos",&(multix->y),-10280,10280);


    gui.addToggle("multix blend 0-min 1-max",&(multix->minmaxBlend));
    gui.addToggle("multix depth test",&(multix->depthTest));

    gui.addSlider("multix yRotSpeed",&(multix->yRotSpeed),-100,100);
*/
    // init variables
    aFPS        = 0;
    vFPS        = 0;
    appFPS      = 0;
    updateStats = 0;
    background  = 0;
    sprintf(msg,"");
    ofBackground(255,255,255);
    drawGUI=true;
    drawStats=true;

    // the osc interface allows for mapping of osc messages to variables or setters
    oscInterface = new OscInterface(multix->getVideoHeaders(),multix->getVideoRenderers(),videoBuffer);
    //oscInterface.mapMessage("vheaderpos",new Poco::Delegate<VideoHeader,float>(videoHeader[0],&VideoHeader::setPct));
    //oscInterface.mapMessage("aheaderpos",new Poco::Delegate<AudioHeader,float>(audioHeader,&AudioHeader::setPct));

    //oscInterface.mapMessage("headerspeed",&(audioHeader->speed));
    //oscInterface->mapMessage("x",&(videoRenderer[0]->x));
    //oscInterface->mapMessage("y",&(videoRenderer[0]->z));

	
	
    oscInterface->mapMessage("delay",&(multix->delayOffset));
    oscInterface->mapMessage("numHeaders",&(multix->numHeaders));


	 
}
//--------------------------------------------------------------

void testApp::exit(){

	videoInput.close();

	 }

//--------------------------------------------------------------
void testApp::update(){

	videoInput.update();

    // update the multix state
    multix->update();

    // update the msg every 100 frames
    if(updateStats%100==0){
        aFPS=audioBuffer->getRealFPS();
        vFPS=videoBuffer->getRealFPS();
        appFPS=ofGetFrameRate();
    }
    updateStats++;
    sprintf(msg,"audio real fps:%f, video real fps: %f, app fps:%f, num frames audio: %d",aFPS,vFPS,appFPS,AudioFrame::numInstances);

    ofBackground(background,background,background);
}

//--------------------------------------------------------------
void testApp::draw(){

    // draw the multix or rec loop renderers
    //recloop->draw();
    multix->draw();


    // draw the gui
    ofSetColor(255,255,255);
    ofFill();
    ofRect(LIVE_SCREEN_W,0,ofGetWidth()-LIVE_SCREEN_W,ofGetHeight());
    ofRect(0,LIVE_SCREEN_H,ofGetWidth(),ofGetHeight()-LIVE_SCREEN_H);



    if(drawStats){
        // draw the representation of the headers and buffers
        audioHeader->draw();
        audioBuffer->draw();
        videoBuffer->draw();

        ofDrawBitmapString(msg,20,620);

        // draw the multix headers state
        ofSetColor(0,0,0);
        ofNoFill();
        ofRect(0,0,LIVE_SCREEN_W,LIVE_SCREEN_H);
        for(int i=0; i < multix->numHeaders; i++){
            VideoHeader * header = multix->getHeader(i);
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

