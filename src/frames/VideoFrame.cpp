/*
 * VideoFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoFrame.h"
using Poco::ScopedLock;

namespace ofxPm
{
int VideoFrame::total_num_frames=0;
map<VideoFormat,vector<ofPtr<VideoFrame::Obj> > > VideoFrame::pool;
ofMutex VideoFrame::poolMutex;

class VideoFrame::Obj{
public:
	Obj()
	:pixelsChanged(false)
	{
		total_num_frames++;

	}
	Obj(const ofPixels & videoFrame)
	:pixels(videoFrame)
	,pixelsChanged(true)
	{
		total_num_frames++;
	}

    ofPixels pixels;
    ofFbo fbo;
    bool pixelsChanged;
};

	VideoFrame::VideoFrame(const ofPixels & videoFrame) 
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
	}


	VideoFrame::VideoFrame()
	{
	}

	VideoFrame::~VideoFrame() {
	}

	VideoFrame VideoFrame::newVideoFrame(const ofPixels & videoFrame){
		VideoFormat format(videoFrame);
		poolMutex.lock();
		if(!pool[format].empty()){
			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			pool[format].pop_back();
			poolMutex.unlock();

			frame.refreshTimestamp();
			frame.data->pixels = videoFrame;
			frame.data->pixelsChanged = true;
			return frame;
		}else{
			poolMutex.unlock();
			return VideoFrame(videoFrame);
		}
	}



	void VideoFrame::poolDeleter(VideoFrame::Obj * obj){
		poolMutex.lock();
		pool[VideoFormat(obj->pixels)].push_back(ofPtr<Obj>(obj,&VideoFrame::poolDeleter));
		poolMutex.unlock();
	}

	ofPixels & VideoFrame::getPixelsRef(){
		return  data->pixels;
	}

	ofTexture & VideoFrame::getTextureRef(){
		static int numAllocations=0;
		if(!data->fbo.isAllocated()){
			cout << "allocating texture " << numAllocations << endl;
			numAllocations++;
			data->fbo.allocate(data->pixels.getWidth(),data->pixels.getHeight(),ofGetGlInternalFormat(data->pixels));
		}
		if(data->pixelsChanged){
			data->fbo.getTextureReference().loadData(data->pixels);
			data->pixelsChanged = false;
		}
		return data->fbo.getTextureReference();
	}

	ofFbo & VideoFrame::getFboRef(){
		return data->fbo;
	}

	int VideoFrame::getWidth(){
		return data->pixels.getWidth();
	}

	int VideoFrame::getHeight(){
		return data->pixels.getHeight();
	}

	int VideoFrame::getPoolSize(const VideoFormat & format){
		ScopedLock<ofMutex> lock(poolMutex);
		return pool[format].size();
	}


	int VideoFrame::getTotalNumFrames(){
		return total_num_frames;
	}

	VideoFrame::operator void*(){
		return data.get();
	}
	}
