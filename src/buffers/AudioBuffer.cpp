/*
 * AudioBuffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "AudioBuffer.h"

namespace ofxPm{
AudioBuffer::AudioBuffer(AudioSource * source) {
	this->source=source;
    fps=source->getFps();
    totalFrames=0;
    resume();
}

AudioBuffer::~AudioBuffer() {

}

unsigned int AudioBuffer::size(){
    return frames.size();
}

void AudioBuffer::newAudioFrame(AudioFrame &frame){
    if(size()==0)initTime=frame.getTimestamp();
    totalFrames++;
    //buffer.insert(make_pair(frame->getTimestamp(),frame));
   // times.push(frame->getTimestamp());
    frames.push_back(&frame);
    frame.retain();

    if(size()>AUDIO_BUFFER_NUM_FRAMES){
        //delete buffer[times.front()];
        //buffer.erase(times.front());
        //times.pop();
        frames.front()->release();
        frames.erase(frames.begin());
    }
    newFrameEvent.notify(this,frame);
}


AudioFrame * AudioBuffer::getAudioFrame(int position){
    if(size()){
        position = CLAMP(position,0,size());
        frames[position]->retain();
        //cout << "frame " << position << " retained " << frames[position]->_useCountOfThisObject << "\n";
        return frames[position];
    }else{
        return NULL;
    }
}

AudioFrame * AudioBuffer::getAudioFrame(TimeDiff time){
    AudioFrame * frame = NULL;
    if(size()>0){
        int frameback = CLAMP((int)((float)time/1000000.0*(float)fps),1,size());
        int currentPos=size()-frameback;
        frame= frames[currentPos];

        /*if(((float)time/1000000.0*(float)fps)<size() && ((float)time/1000000.0*(float)fps)>=0)
            frame= frames[frames.size()-1-(int)((float)time/1000000.0*(float)fps)];
        else if(((float)time/1000000.0*(float)fps)<0)
            frame= frames[size()-1];
        else
            frame= frames[0];*/
        frame->retain();
    }
    return frame;
}
AudioFrame * AudioBuffer::getAudioFrame(float pct){
    return getAudioFrame(getLastTimestamp()-(getInitTime()+getTotalTime()*pct));
}
float AudioBuffer::getFps(){
    return fps;
}

Timestamp AudioBuffer::getLastTimestamp(){
    if(size()>0)
        return frames.back()->getTimestamp();
    else
        return Timestamp();
}

TimeDiff AudioBuffer::getTotalTime(){
    return getLastTimestamp()-getInitTime();
}
Timestamp AudioBuffer::getInitTime(){
    return initTime;
}

long AudioBuffer::getTotalFrames(){
    return totalFrames;
}

float AudioBuffer::getRealFPS(){
     if(size()>10)
        return 10.0/(float)(frames.back()->getTimestamp()-frames[size()-11]->getTimestamp())*1000000.0;
    else
        return 0;
}

void AudioBuffer::draw(){
    float length = (float)size()/(float)AUDIO_BUFFER_NUM_FRAMES*(float)ofGetWidth();
    float oneLength=(float)ofGetWidth()/(float)AUDIO_BUFFER_NUM_FRAMES;
    //ofLine(0,650,length,650);
    char measureMessage[10];
    for(int i=0;i<size();i++){
        ofRect(oneLength*i,650-frames[i]->getAverageValue()*150,oneLength,frames[i]->getAverageValue()*150+1);
        if(i%100==0){
            ofLine(oneLength*i,650,oneLength*i,640);
            sprintf(measureMessage,"%0.2f",(float)(frames[i]->getTimestamp()-initTime)/1000000.0);
            ofDrawBitmapString(measureMessage,oneLength*i,635);
        }
    }
}

void AudioBuffer::stop(){
	ofRemoveListener(source->newFrameEvent,this,&AudioBuffer::newAudioFrame);
}

void AudioBuffer::resume(){
	ofAddListener(source->newFrameEvent,this,&AudioBuffer::newAudioFrame);
}
}
