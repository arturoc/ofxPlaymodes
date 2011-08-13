/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoGrabber.h"

VideoGrabber::VideoGrabber(){
    setUseTexture(false);
}

VideoGrabber::~VideoGrabber(){
}

VideoFrame * VideoGrabber::getNextVideoFrame(){
    //newFrameEvent.init("PlayModes.VideoGrabber.newFrame");
    VideoFrame * frame = new VideoFrame(getPixels(),getWidth(),getHeight());
    return frame;
}

void VideoGrabber::update(){
	ofVideoGrabber::update();
	if(isFrameNew()){
		VideoFrame * frame = getNextVideoFrame();
		newFrameEvent.notify(this,*frame);
		frame->release();
	}
}

int VideoGrabber::getFps(){
    return 30;
}
