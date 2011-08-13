/*
 * FileGrabber.cpp
 *
 *  Created on: 01/11/2009
 *      Author: arturo castro
 */

#include "FileGrabber.h"

FileGrabber::FileGrabber() {


}

FileGrabber::~FileGrabber() {
	setUseTexture(false);
}

VideoFrame * FileGrabber::getNextVideoFrame(){
    //newFrameEvent.init("PlayModes.VideoGrabber.newFrame");
    VideoFrame * frame = new VideoFrame(getPixels(),getWidth(),getHeight());
    return frame;
}

void FileGrabber::update(){
	ofVideoPlayer::update();
	if(isFrameNew()){
		VideoFrame * frame = getNextVideoFrame();
		newFrameEvent.notify(this,*frame);
		frame->release();
	}
}

int FileGrabber::getFps(){
    return 25;
}
