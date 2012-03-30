/*
 * AudioFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */
#include "AudioFrame.h"

using Poco::ScopedLock;


namespace ofxPm
{

	
	int AudioFrame::numInstances=0;

	//-------------------------------------------------------------------------------
	AudioFrame::AudioFrame()
	{
		bufferSize=-1;
		channels=-1;
		averageValue=-1;
	}
	
	//-------------------------------------------------------------------------------
	AudioFrame * AudioFrame::newAudioFrame(const float * audioFrame,int bufferSize,const int channels) 
	{		
		AudioFrame * aFrame = new AudioFrame();
		aFrame->bufferSize = bufferSize;
		aFrame->channels = channels;
		aFrame->averageValue=0;
		for(int i=0;i<bufferSize;i++)
		{
			aFrame->data.push_back(audioFrame[i*channels ]);
			aFrame->data.push_back(audioFrame[i*channels+1]);
			aFrame->averageValue+=audioFrame[i*channels];
		}
		aFrame->averageValue=aFrame->averageValue/bufferSize;
		numInstances++;
		aFrame->retain();
		printf("AudioFrame Nu Instances %d\n",numInstances);
		return aFrame;
	}
		
	//-------------------------------------------------------------------------------
	void AudioFrame::release() 
	{
	    ScopedLock<ofMutex> lock(*mutex);
		_useCountOfThisObject--;
		if(_useCountOfThisObject == 0) {
			printf("AUDIOFRAME:: deleting object !!!!!!!!!!!!!!!!!!!!!!!!!\n");
			delete this;
		}
	}
		
	//-------------------------------------------------------------------------------
	void AudioFrame::retain() 
	{
		ScopedLock<ofMutex> lock(*mutex);
		_useCountOfThisObject++;
	}
	
	//-------------------------------------------------------------------------------
	AudioFrame::~AudioFrame() 
	{
		data.~vector();
		printf("deleting AudioFrame!!\n");
		numInstances--;
	}


	//-------------------------------------------------------------------------------
	vector<float> AudioFrame::getAudioData()
	{
		return data;
	}
		
//	float * AudioFrame::getAudioData(int index)
//	{
//		float* result = new float[channels];
//		for(int i=0;i<channels;i++)
//		{
//			result[i] = data[]
//			
//		}
//		return data;
//	}

	//-------------------------------------------------------------------------------
	int AudioFrame::getBufferSize()
	{
		return bufferSize;
	}
	
	//-------------------------------------------------------------------------------
	int AudioFrame::getChannels()
	{
		return channels;
	}
	
	//-------------------------------------------------------------------------------
	float   AudioFrame::getAverageValue()
	{
		return averageValue;
	}
}
