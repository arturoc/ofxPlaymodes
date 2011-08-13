/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "AudioHeader.h"

 AudioHeader::AudioHeader(myAudioBuffer *buffer)
{
    this->buffer=buffer;
    fps=buffer->getFps();
    position=0;
    oneFrame=(pmTimeDiff)round(1000000.0/(float)fps);
    speed=1;
    prevBufferPos=0;
    density=0;
    currentPos=0;
    pctHasChanged=false;
}


 AudioHeader::~AudioHeader()
{

}


void AudioHeader::draw()
{
    ofSetColor(0,0,0);
    float currentLength=(float)currentPos/(float)AUDIO_BUFFER_NUM_FRAMES*(float)ofGetWidth();
    sprintf(msgPos,"%i",currentPos);
    ofLine(currentLength,620,currentLength,660);
    ofSetColor(50,50,50);
    ofLine(currentLength-density/(float)AUDIO_BUFFER_NUM_FRAMES*(float)ofGetWidth(),630,currentLength-density/(float)AUDIO_BUFFER_NUM_FRAMES*(float)ofGetWidth(),650);
    ofLine(currentLength+density/(float)AUDIO_BUFFER_NUM_FRAMES*(float)ofGetWidth(),630,currentLength+density/(float)AUDIO_BUFFER_NUM_FRAMES*(float)ofGetWidth(),650);


    ofDrawBitmapString(msgPos,currentLength,615);
}

float AudioHeader::getFps()
{
    return fps;
}



void AudioHeader::setFps(float fps)
{
    this->fps=fps;
}



AudioFrame * AudioHeader::getNextFrame(){
    buffer->lock();
    if(pctHasChanged && buffer->size()){
        pct=CLAMP(pct,0,1);
        //cout << "vheader pct: "<< pct<< "\n";
        currentPos=pct*(buffer->size()-1);
        positionTS.update();
        position=buffer->getTotalFrames()-(buffer->size()-currentPos);
        pctHasChanged=false;
    }else{
        currentPos=getNextPosition();
        pct=(float)currentPos/(float)buffer->size();
    }
    AudioFrame * frame = buffer->getAudioFrame(currentPos);
    buffer->unlock();
    return frame;
}


AudioFrame * AudioHeader::getAudioFrame(int position,float density){
    position = CLAMP(position,0,buffer->size());
    AudioFrame * currentFrame = buffer->getAudioFrame(position);
    vector<AudioFrame*> prevFrames;
    vector<AudioFrame*> nextFrames;

    for(int i=0;i<density+1;i++){
        if(buffer->size()>position+i+1){
            AudioFrame * frame = buffer->getAudioFrame(position+i);
            if(frame){
                nextFrames.push_back(frame);
            }
        }
    }
    for(int i=0;i>-density-1;i--){
        if(position+i>0){
            AudioFrame * frame = buffer->getAudioFrame(position+i);
            if(frame){
                nextFrames.push_back(frame);
            }
        }
    }

    int currentFrameSize=currentFrame->getBufferSize()*currentFrame->getChannels();
    float resultBuffer[currentFrameSize];
    memcpy(resultBuffer,currentFrame->getAudioFrame(),sizeof(float)*currentFrameSize);

    for(int i=0;i<prevFrames.size();i++){
        int frameSize=prevFrames[i]->getBufferSize()*prevFrames[i]->getChannels();
        int offset=CLAMP((int)((float)frameSize*(density-(float)i)),0,frameSize);
        for(int j=frameSize-offset;j<frameSize;j++){
            resultBuffer[frameSize-j]=resultBuffer[frameSize-j]+prevFrames[i]->getAudioFrame()[j];
        }
        prevFrames[i]->release();
    }

    for(int i=0;i<nextFrames.size();i++){
        int frameSize=nextFrames[i]->getBufferSize()*nextFrames[i]->getChannels();
        int offset=CLAMP((int)((float)frameSize*(density-(float)i)),0,frameSize);
        for(int j=0;j<offset;j++){
            resultBuffer[frameSize-j]=resultBuffer[frameSize-j]+nextFrames[i]->getAudioFrame()[j];
        }
        nextFrames[i]->release();
    }

    AudioFrame * resultFrame=
        new AudioFrame( resultBuffer,
                        currentFrame->getBufferSize(),
                        currentFrame->getChannels());
    currentFrame->release();
    return resultFrame;
}

//------------------------------------------
// returns the real position in the buffer
int AudioHeader::getNextPosition(){
    oneFrame=(pmTimeDiff)(1000000.0/fps/speed);
    if((float)positionTS.elapsed()+(position-floor(position))*(float)abs(oneFrame)>=abs(oneFrame)){
        if(oneFrame!=0){
            position+=(float)positionTS.elapsed()/(float)oneFrame;

        }
        positionTS.update();
    }
    unsigned int buffer_size=buffer->size();
    //-------------------------------------------
    // calculates the absolute position in frames
    // for all the duration
    position=CLAMP(position,
                    buffer->getTotalFrames()-buffer->size(),
                    buffer->getTotalFrames());
    int backpos=buffer->getTotalFrames()-position;

    backpos=CLAMP(backpos,1,buffer_size);
    return buffer_size-backpos;
}

