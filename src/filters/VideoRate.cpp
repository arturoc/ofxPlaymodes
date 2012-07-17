/*
 * VideoRate.cpp
 *
 *  Created on: 16/07/2012
 *      Author: arturo
 */

#include "filters/VideoRate.h"

namespace ofxPm{

VideoRate::VideoRate()
:source(0)
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

VideoFrame VideoRate::getNextVideoFrame(){
	Poco::ScopedLock<ofMutex> lock(mutexFront);
	return front;
}

void VideoRate::newVideoFrame(VideoFrame & frame){
	mutex.lock();
	back = frame;
	mutex.unlock();
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
		if(back!=NULL){
			mutex.lock();
			VideoFrame currFrame = back;
			mutex.unlock();

			mutexFront.lock();
			front = VideoFrame::newVideoFrame(currFrame.getPixelsRef());
			mutexFront.unlock();
			ofNotifyEvent(newFrameEvent,front);
		}
		time = ofGetElapsedTimeMicros()-time;
		long sleeptime =  1000000./fps-time;
		if(sleeptime>0){
			usleep(sleeptime);
		}
	}
}

}
