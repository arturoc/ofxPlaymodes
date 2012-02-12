/*
 * VideoFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoFrame.h"
using Poco::ScopedLock;

int VideoFrame::total_num_frames=0;

VideoFrame::VideoFrame(const ofPixels & videoFrame) {
	pixels = videoFrame;
	total_num_frames++;
	doRelease=false;
	//cout << "total num frames: " << total_num_frames <<"\n";

}

VideoFrame::~VideoFrame() {
    ofRemoveListener(ofEvents.update,this,&VideoFrame::update);
    total_num_frames--;
}

void VideoFrame::update(ofEventArgs & args){
    if(doRelease){
        delete this;
    }
}

void VideoFrame::release() {
    ScopedLock<ofMutex> lock(*mutex);
	_useCountOfThisObject--;
	if(_useCountOfThisObject == 0) {
		doRelease=true;
        ofAddListener(ofEvents.update,this,&VideoFrame::update);
	}
}

ofPixels & VideoFrame::getPixelsRef(){
    return  pixels;
}

ofTexture & VideoFrame::getTextureRef(){
	if(!texture.isAllocated()){
        texture.allocate(pixels.getWidth(),pixels.getHeight(),GL_RGB);
        texture.loadData(pixels);
    }
    return texture;
}

int VideoFrame::getWidth(){
	return pixels.getWidth();
}

int VideoFrame::getHeight(){
	return pixels.getHeight();
}
