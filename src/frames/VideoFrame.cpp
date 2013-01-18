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
	,createdTexPixels(true)
	{
		total_num_frames++;

	}
	Obj(const ofPixels & videoFrame)
	:pixels(videoFrame)
	,pixelsChanged(true)
	,createdTexPixels(true)
	{
		total_num_frames++;
	}
	Obj(ofTexture & videoFrame)
	:pixelsChanged(false)
	,createdTexPixels(false)
	{
		pixels.allocate(videoFrame.getWidth(),videoFrame.getHeight(),ofGetImageTypeFromGLType(videoFrame.texData.glTypeInternal));
		updateTexture(videoFrame);
		total_num_frames++;
	}
	Obj(ofFbo & videoFrame)
	:pixelsChanged(false)
	,createdTexPixels(false)
	{
		pixels.allocate(videoFrame.getWidth(),videoFrame.getHeight(),ofGetImageTypeFromGLType(videoFrame.getTextureReference().texData.glTypeInternal));
		updateTexture(videoFrame);
		total_num_frames++;
	}

	void updateTexture(ofTexture & videoFrame){
		if(!fbo.isAllocated()){
			fbo.allocate(videoFrame.getWidth(),videoFrame.getHeight(),videoFrame.texData.glTypeInternal);
		}
		fbo.begin();
		videoFrame.bind();
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
		mesh.addTexCoord(ofVec2f(0,0));
		mesh.addTexCoord(ofVec2f(videoFrame.getWidth(),0));
		mesh.addTexCoord(ofVec2f(videoFrame.getWidth(),videoFrame.getHeight()));
		mesh.addTexCoord(ofVec2f(0,videoFrame.getHeight()));
		mesh.addVertex(ofVec3f(0,0));
		mesh.addVertex(ofVec3f(videoFrame.getWidth(),0));
		mesh.addVertex(ofVec3f(videoFrame.getWidth(),videoFrame.getHeight()));
		mesh.addVertex(ofVec3f(0,videoFrame.getHeight()));
		mesh.draw();
		videoFrame.unbind();
		fbo.end();
	}


	void updateTexture(ofFbo & videoFrame){
		if(!fbo.isAllocated()){
			fbo.allocate(videoFrame.getWidth(),videoFrame.getHeight(),videoFrame.getTextureReference().texData.glTypeInternal);
		}
		videoFrame.bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindTexture(fbo.getTextureReference().texData.textureTarget, (GLuint)fbo.getTextureReference().texData.textureID);
		glCopyTexImage2D(fbo.getTextureReference().texData.textureTarget,0,fbo.getTextureReference().texData.glTypeInternal,0,0,fbo.getWidth(),fbo.getHeight(),0);
		videoFrame.unbind();
		glReadBuffer(GL_BACK);
	}

    ofPixels pixels;
    ofFbo fbo;
    bool pixelsChanged;
    bool createdTexPixels;
};

	VideoFrame::VideoFrame(const ofPixels & videoFrame) 
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
	}

	VideoFrame::VideoFrame(ofTexture & videoFrame)
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
	}

	VideoFrame::VideoFrame(ofFbo & videoFrame)
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
			frame.data->createdTexPixels = true;
			return frame;
		}else{
			poolMutex.unlock();
			return VideoFrame(videoFrame);
		}
	}

	VideoFrame VideoFrame::newVideoFrame(ofTexture & videoFrame){
		VideoFormat format(videoFrame);
		poolMutex.lock();
		if(!pool[format].empty()){
			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			pool[format].pop_back();
			poolMutex.unlock();

			frame.refreshTimestamp();
			frame.getFboRef();
			frame.data->updateTexture(videoFrame);
			frame.data->pixelsChanged = false;
			frame.data->createdTexPixels = false;
			return frame;
		}else{
			poolMutex.unlock();
			return VideoFrame(videoFrame);
		}
	}

	VideoFrame VideoFrame::newVideoFrame(ofFbo & videoFrame){
		VideoFormat format(videoFrame);
		poolMutex.lock();
		if(!pool[format].empty()){
			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			pool[format].pop_back();
			poolMutex.unlock();

			frame.refreshTimestamp();
			frame.getFboRef();
			frame.data->updateTexture(videoFrame);
			frame.data->pixelsChanged = false;
			frame.data->createdTexPixels = false;
			return frame;
		}else{
			poolMutex.unlock();
			return VideoFrame(videoFrame);
		}
	}

	VideoFrame VideoFrame::newVideoFrame(VideoFrame videoFrame){
		if(videoFrame.data->createdTexPixels){
			return newVideoFrame(videoFrame.getPixelsRef());
		}else{
			return newVideoFrame(videoFrame.getFboRef());
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
		if(data->pixelsChanged){
			getFboRef().getTextureReference().loadData(data->pixels);
			data->pixelsChanged = false;
		}
		return getFboRef().getTextureReference();
	}

	ofFbo & VideoFrame::getFboRef(){
		if(!data->fbo.isAllocated()){
			data->fbo.allocate(data->pixels.getWidth(),data->pixels.getHeight(),ofGetGlInternalFormat(data->pixels));
		}
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

	void VideoFrame::setTextureOnly(bool texOnly){
		data->createdTexPixels=!texOnly;
		data->pixelsChanged=false;
	}
	}
