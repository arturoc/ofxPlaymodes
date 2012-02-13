/*
 * VideoFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoFrame.h"
using Poco::ScopedLock;

namespace ofxPm{
int VideoFrame::total_num_frames=0;
map<VideoFormat,vector<VideoFrame *> > VideoFrame::pool;
ofMutex VideoFrame::poolMutex;

VideoFrame::VideoFrame(const ofPixels & videoFrame) {
	pixels = videoFrame;
	total_num_frames++;
	pixelsChanged = true;
	//cout << "total num frames: " << total_num_frames <<"\n";

}

VideoFrame::~VideoFrame() {
    total_num_frames--;
}

VideoFrame * VideoFrame::newVideoFrame(const ofPixels & videoFrame){
	VideoFrame * frame = NULL;
	VideoFormat format(videoFrame);
	ScopedLock<ofMutex> lock(poolMutex);
	if(!pool[format].empty()){
		//cout << "returning frame from pool" << endl;
		frame = pool[format].back();
		frame->refreshTimestamp();
		frame->pixels = videoFrame;
		frame->pixelsChanged = true;
		frame->retain();
		pool[format].pop_back();
	}else{
		//cout << "new frame" << endl;
		frame = new VideoFrame(videoFrame);
	}
	return frame;
}

void VideoFrame::release() {
    ScopedLock<ofMutex> lock(*mutex);
	_useCountOfThisObject--;
	if(_useCountOfThisObject == 0) {
		VideoFormat format(pixels);
		poolMutex.lock();
		pool[format].push_back(this);
		poolMutex.unlock();
	}
}

ofPixels & VideoFrame::getPixelsRef(){
    return  pixels;
}

ofTexture & VideoFrame::getTextureRef(){
	if(!texture.isAllocated()){
        texture.allocate(pixels.getWidth(),pixels.getHeight(),ofGetGlInternalFormat(pixels));
    }
	if(pixelsChanged){
        texture.loadData(pixels);
        pixelsChanged = false;
	}
    return texture;
}

int VideoFrame::getWidth(){
	return pixels.getWidth();
}

int VideoFrame::getHeight(){
	return pixels.getHeight();
}

int VideoFrame::getPoolSize(const VideoFormat & format){
	return pool[format].size();
}
}
