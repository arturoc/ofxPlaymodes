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
	    data[i*channels  ] = audioFrame[i*channels ];
	    data[i*channels+1] = audioFrame[i*channels+1];
	    averageValue+=audioFrame[i*channels];
	}
	averageValue=averageValue/bufferSize;
	this->bufferSize=bufferSize;
	this->channels=channels;
    numInstances++;
}
	
void AudioFrame::release() {
	
// VideoFrame release() code
/*	ScopedLock<ofMutex> lock(*mutex);
	_useCountOfThisObject--;
	if(_useCountOfThisObject == 0) {
		VideoFormat format(pixels);
		poolMutex.lock();
		pool[format].push_back(this);
		poolMutex.unlock();
	}
*/
}
	
AudioFrame::~AudioFrame() {
	delete[] data;
	numInstances--;
}


float * AudioFrame::getAudioData(){
    return data;
}
//	
//float * AudioFrame::getAudioData(int index)
//{
//	float* result = new float[channels];
//	for(int i=0;i<channels;i++)
//	{
//		result[i] = data[]
//		
//	}
//	return data;
//}
//
	
	

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
