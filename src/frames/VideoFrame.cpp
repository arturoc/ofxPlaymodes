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

	}
	Obj(const ofPixels & videoFrame)
	:pixels(videoFrame)
	,pixelsChanged(true)
	{}

    ofPixels pixels;
    ofTexture texture;
    bool pixelsChanged;
};

	VideoFrame::VideoFrame(const ofPixels & videoFrame) 
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
		total_num_frames++;
	}


	VideoFrame::VideoFrame()
	{
		total_num_frames++;
	}

	VideoFrame::~VideoFrame() {
		total_num_frames--;
	}

	VideoFrame VideoFrame::newVideoFrame(const ofPixels & videoFrame){
		VideoFormat format(videoFrame);
		ScopedLock<ofMutex> lock(poolMutex);
		if(!pool[format].empty()){
			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			frame.refreshTimestamp();
			frame.data->pixels = videoFrame;
			frame.data->pixelsChanged = true;
			pool[format].pop_back();
			return frame;
		}else{
			return VideoFrame(videoFrame);
		}
	}



	void VideoFrame::poolDeleter(VideoFrame::Obj * obj){
		pool[VideoFormat(obj->pixels)].push_back(ofPtr<Obj>(obj,&VideoFrame::poolDeleter));
	}

	ofPixels & VideoFrame::getPixelsRef(){
		return  data->pixels;
	}

	ofTexture & VideoFrame::getTextureRef(){
		if(!data->texture.isAllocated()){
			data->texture.allocate(data->pixels.getWidth(),data->pixels.getHeight(),ofGetGlInternalFormat(data->pixels));
		}
		if(data->pixelsChanged){
			data->texture.loadData(data->pixels);
			data->pixelsChanged = false;
		}
		return data->texture;
	}

	int VideoFrame::getWidth(){
		return data->pixels.getWidth();
	}

	int VideoFrame::getHeight(){
		return data->pixels.getHeight();
	}

	int VideoFrame::getPoolSize(const VideoFormat & format){
		return pool[format].size();
	}

	VideoFrame::operator void*(){
		return data.get();
	}
	}
