/*
 * VideoRate.cpp
 *
 *  Created on: 16/07/2012
 *      Author: arturo
 */

#include "filters/VideoRate.h"

namespace ofxPm{

VideoRate::VideoRate()
:back(0)
,front(0)
,source(0)
,fps(30){
	// TODO Auto-generated constructor stub

}

VideoRate::~VideoRate() {
	// TODO Auto-generated destructor stub
}


void VideoRate::setup(VideoSource & _source, float fps){
	source = &_source;
	ofAddListener(source->newFrameEvent,this,&VideoRate::newVideoFrame);
	setFps(fps);
	startThread(true,false);
}

VideoFrame * VideoRate::getNextVideoFrame(){
	if(front) front->retain();
	Poco::ScopedLock<ofMutex> lock(mutexFront);
	return front;
}

void VideoRate::newVideoFrame(VideoFrame & frame){
	if(back) back->release();
	mutex.lock();
	back = &frame;
	mutex.unlock();
	//videoFrame->retain();
}

float VideoRate::getFps(){
	return fps;
}

void VideoRate::setFps(float _fps){
	fps = _fps;
}


void VideoRate::threadedFunction(){
	while(isThreadRunning()){
		unsigned long time = ofGetElapsedTimeMicros();
		mutex.lock();
		if(back){
			VideoFrame * currFrame = back;
			mutex.unlock();

			if(front) front->release();
			mutexFront.lock();
			front = VideoFrame::newVideoFrame(currFrame->getPixelsRef());
			mutexFront.unlock();
			ofNotifyEvent(newFrameEvent,*front);
		}else{
			mutex.unlock();
		}
		time = ofGetElapsedTimeMicros()-time;
		long sleeptime =  1000000./fps-time;
		if(sleeptime>0){
			usleep(sleeptime);
		}
	}
}

}
