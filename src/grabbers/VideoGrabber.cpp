/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoGrabber.h"

namespace ofxPm{
VideoGrabber::VideoGrabber(){
}

VideoGrabber::~VideoGrabber(){
#ifdef TARGET_LINUX
    ofGstVideoGrabber * gstVideoGrabber = (ofGstVideoGrabber*)getGrabber().get();
    ofRemoveListener(gstVideoGrabber->getGstVideoUtils()->bufferEvent,this,&VideoGrabber::newFrame);
#endif
}

bool VideoGrabber::initGrabber(int w, int h){
	bool ret = ofVideoGrabber::initGrabber(w,h,false);
#ifdef TARGET_LINUX
	if(ret){
		ofGstVideoGrabber * gstVideoGrabber = (ofGstVideoGrabber*)getGrabber().get();
		ofAddListener(gstVideoGrabber->getGstVideoUtils()->bufferEvent,this,&VideoGrabber::newFrame);
	}
#endif
	return ret;
}

VideoFrame VideoGrabber::getNextVideoFrame(){
    return frame;
}

void VideoGrabber::update(){
#ifndef TARGET_LINUX
	ofVideoGrabber::update();
	if(isFrameNew()){
		frame = VideoFrame::newVideoFrame(getPixelsRef());
		newFrameEvent.notify(this,frame);
	}

#endif
}

void VideoGrabber::newFrame(ofPixels & pixels){
	frame = VideoFrame::newVideoFrame(pixels);
	newFrameEvent.notify(this,frame);
}

//------------------------------------------------------
float VideoGrabber::getFps(){
	return fps;
}
	
//------------------------------------------------------
void VideoGrabber::setFps(float fps){
	this->fps = fps;
}
	
	;
	
}
