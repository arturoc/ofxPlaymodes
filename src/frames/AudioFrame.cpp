/*
 * AudioFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */
#include "AudioFrame.h"



namespace ofxPm{
int AudioFrame::numInstances=0;

AudioFrame::AudioFrame(float * audioFrame, int bufferSize, int channels) {
    data = NULL;
	data = new float[bufferSize*channels];
	//memcpy(data,audioFrame,sizeof(float)*bufferSize*channels);
	//data=audioFrame;
	averageValue=0;
	for(int i=0;i<bufferSize;i++){
	    data[i*2]=audioFrame[i*2];//*cos((float)i/(float)bufferSize-PI/2);
	    data[i*2+1]=audioFrame[i*2+1];//*cos((float)i/(float)bufferSize-PI/2);
	    averageValue+=audioFrame[i*2];
	}
	averageValue=averageValue/bufferSize;
	this->bufferSize=bufferSize;
	this->channels=channels;
    numInstances++;
}

AudioFrame::~AudioFrame() {
	delete[] data;
	numInstances--;
}


float * AudioFrame::getAudioFrame(){
    return data;
}


int AudioFrame::getBufferSize(){
    return bufferSize;
}
int AudioFrame::getChannels(){
    return channels;
}

float   AudioFrame::getAverageValue(){
    return averageValue;
}
}
