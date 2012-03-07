/*
 * VideoBuffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoBuffer.h"

namespace ofxPm{
VideoBuffer::VideoBuffer(VideoSource & source, int size) {
	setup(source,size);
}

VideoBuffer::VideoBuffer(){
	source = NULL;
	fps=0;
	totalFrames=0;
	stopped = false;
	maxSize = 0;
}


void VideoBuffer::setup(VideoSource & source, int size){
	this->source=&source;
	fps=source.getFps();
	totalFrames=0;
	maxSize = size;
	resume();
}

VideoBuffer::~VideoBuffer() {

}

void VideoBuffer::newVideoFrame(VideoFrame & frame){
    totalFrames++;
    if(size()==0)initTime=frame.getTimestamp();
    timeMutex.lock();
    frames.push_back(&frame);
    frame.retain();
    while(size()>maxSize){
        //delete buffer[times.front()];
        //buffer.erase(times.front());
        //cout << "releasing frame release count:"<<frames.front()->_useCountOfThisObject<<"\n";
        frames.front()->release();
        frames.erase(frames.begin());
    }
    timeMutex.unlock();
    newFrameEvent.notify(this,frame);

}

Timestamp VideoBuffer::getLastTimestamp(){
    if(size()>0)
        return frames.back()->getTimestamp();
    else
        return Timestamp();
}

TimeDiff VideoBuffer::getTotalTime(){
    return getLastTimestamp()-getInitTime();
}

Timestamp VideoBuffer::getInitTime(){
    return initTime;
}

unsigned int VideoBuffer::size(){
    return frames.size();
}


unsigned int VideoBuffer::getMaxSize(){
	return maxSize;
}


float VideoBuffer::getFps(){
    return fps;
}

VideoFrame * VideoBuffer::getVideoFrame(TimeDiff time){
    VideoFrame *frame=NULL;
    if(size()>0){
        int frameback = CLAMP((int)((float)time/1000000.0*(float)fps),1,size());
        int currentPos=CLAMP(size()-frameback,0,size()-1);
        frame= frames[currentPos];
        /*if(((float)time/1000000.0*(float)fps)<size() && ((float)time/1000000.0*(float)fps)>=0)
            frame= frames[frames.size()-1-(int)((float)time/1000000.0*(float)fps)];
        else if(((float)time/1000000.0*(float)fps)<0)
            frame= frames[size()-1];
        else
            frame= frames[0];
*/
        frame->retain();
    }

    return frame;

}

VideoFrame * VideoBuffer::getVideoFrame(int position){
    //return buffer.find(times[times.size()-position])->second;
    if(size()){
        position = CLAMP(position,0,size()-1);
        frames[position]->retain();
        //cout << "frame " << position << " retained " << frames[position]->_useCountOfThisObject << "\n";
        return frames[position];
    }else{
        return NULL;
    }
}

VideoFrame * VideoBuffer::getVideoFrame(float pct){
    return getVideoFrame(getLastTimestamp()-(getInitTime()+getTotalTime()*pct));
}

VideoFrame * VideoBuffer::getNextVideoFrame(){
    return getVideoFrame((int)size()-1);
}

long VideoBuffer::getTotalFrames(){
    return totalFrames;
}

float VideoBuffer::getRealFPS(){
    if(size()>10)
        return 10.0/(float)(frames.back()->getTimestamp()-frames[size()-11]->getTimestamp())*1000000.0;
        //eloi : return 10.0/(float)(frames.back()->getTimestamp()-frames[size()-1]->getTimestamp())*1000000.0;
    else
        return 0;
}


void VideoBuffer::draw(){
	
    float length = (float(size())/float(maxSize))*(ofGetWidth()-(PMDRAWSPACING));
    float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(maxSize);
	int drawBufferY = PMDRAWELEMENTSY+40;
    if(stopped) ofSetColor(255,0,0);
	else ofSetColor(255);
	
	ofLine(0+PMDRAWSPACING,drawBufferY,length,drawBufferY);
	
	ofSetColor(255);
	
    char measureMessage[10];
    for(int i=0;i<(int)size()+1;i++){
		/*
       if(i%100==0){
            ofLine(oneLength*i,710,oneLength*i,700);
            sprintf(measureMessage,"%0.2f",(float)(frames[i]->getTimestamp()-initTime)/1000000.0);
            ofDrawBitmapString(measureMessage,oneLength*i,695);
        }
		 */
		if(i%fps==0) 
		{
			ofSetLineWidth(2.0);
			ofSetColor(255,128,0);
			if(i!=int(size())) ofDrawBitmapString(ofToString(int(size()-i-1)),oneLength*(i)+PMDRAWSPACING + oneLength/2,PMDRAWELEMENTSY+25);
			else 
			{
				ofSetColor(50);
				ofDrawBitmapString(ofToString(getTotalFrames()),oneLength*(i)+PMDRAWSPACING - 10,PMDRAWELEMENTSY+55); 
			}
			//if(i!=int(size())) ofDrawBitmapString(ofToString(getTotalFrames()-i),ofGetWidth()-PMDRAWSPACING-(oneLength*(i+1)) + oneLength/2,drawBufferY-15);
        }
		else 
		{
			ofSetLineWidth(1.0);
			ofSetColor(155,58,0);
		}
		ofLine(oneLength*(i)+PMDRAWSPACING,drawBufferY,oneLength*(i)+PMDRAWSPACING,drawBufferY-10);
    }
}


void VideoBuffer::stop(){
	ofRemoveListener(source->newFrameEvent,this,&VideoBuffer::newVideoFrame);
    stopped = true;
	
}

void VideoBuffer::resume(){
	ofAddListener(source->newFrameEvent,this,&VideoBuffer::newVideoFrame);
    stopped = false;
}

bool VideoBuffer::isStopped(){
	return stopped;
}
}
