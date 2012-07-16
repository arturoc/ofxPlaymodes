/*
 * FileGrabber.cpp
 *
 *  Created on: 01/11/2009
 *      Author: arturo castro
 */

#include "FileGrabber.h"

namespace ofxPm{
FileGrabber::FileGrabber() {


}

FileGrabber::~FileGrabber() {
	setUseTexture(false);
}

VideoFrame FileGrabber::getNextVideoFrame(){
    return VideoFrame::newVideoFrame(getPixelsRef());
}

void FileGrabber::update(){
	ofVideoPlayer::update();
	if(isFrameNew()){
		VideoFrame frame = getNextVideoFrame();
		newFrameEvent.notify(this,frame);
	}
}

float FileGrabber::getFps(){
    return 25;
}
}
