/*
 * VideoBuffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoBuffer.h"

VideoBuffer::VideoBuffer(VideoSource * source) {
	source->addListener(this);
	this->source=source;
	fps=source->getFps();
	totalFrames=0;

}

VideoBuffer::~VideoBuffer() {

}

void VideoBuffer::newVideoFrame(VideoFrame & frame){
    totalFrames++;
    if(size()==0)initTime=frame.getTimestamp();
    timeMutex.lock();
    frames.push_back(&frame);
    frame.retain();
    if(size()>VIDEO_BUFFER_NUM_FRAMES){
        //delete buffer[times.front()];
        //buffer.erase(times.front());
        //cout << "releasing frame release count:"<<frames.front()->_useCountOfThisObject<<"\n";
        frames.front()->release();
        frames.erase(frames.begin());
    }
    timeMutex.unlock();
    newFrameEvent.notify(this,frame);

}

pmTimestamp VideoBuffer::getLastTimestamp(){
    if(size()>0)
        return frames.back()->getTimestamp();
    else
        return pmTimestamp();
}

pmTimeDiff VideoBuffer::getTotalTime(){
    return getLastTimestamp()-getInitTime();
}
pmTimestamp VideoBuffer::getInitTime(){
    return initTime;
}

unsigned int VideoBuffer::size(){
    return frames.size();
}

int VideoBuffer::getFps(){
    return fps;
}

VideoFrame * VideoBuffer::getVideoFrame(pmTimeDiff time){
    VideoFrame *frame=NULL;
    if(size()>0){
        int frameback = CLAMP((int)((float)time/1000000.0*(float)fps),1,size());
        int currentPos=size()-frameback;
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

VideoFrame * VideoBuffer::getNextVideoFrame(){
    return getVideoFrame((int)size()-1);
}

VideoFrame * VideoBuffer::getVideoFrame(float pct){
    return getVideoFrame(getLastTimestamp()-(getInitTime()+getTotalTime()*pct));
}

long VideoBuffer::getTotalFrames(){
    return totalFrames;
}




float VideoBuffer::getRealFPS(){
    if(size()>10)
        return 10.0/(float)(frames.back()->getTimestamp()-frames[size()-11]->getTimestamp())*1000000.0;
    else
        return 0;
}


void VideoBuffer::draw(){
    float length = (float)size()/(float)VIDEO_BUFFER_NUM_FRAMES*(float)ofGetWidth();
    float oneLength=(float)ofGetWidth()/(float)VIDEO_BUFFER_NUM_FRAMES;
    ofLine(0,710,length,710);
    char measureMessage[10];
    for(int i=0;i<size();i++){
       if(i%100==0){
            ofLine(oneLength*i,710,oneLength*i,700);
            sprintf(measureMessage,"%0.2f",(float)(frames[i]->getTimestamp()-initTime)/1000000.0);
            ofDrawBitmapString(measureMessage,oneLength*i,695);
        }
    }
}


void VideoBuffer::stop(){
    ((VideoSource*)source)->removeListener(this);
}

void VideoBuffer::resume(){
    ((VideoSource*)source)->addListener(this);
}
