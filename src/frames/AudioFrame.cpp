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


	class AudioFrame::Obj{
	public:
		Obj(){
			bufferSize=-1;
			channels=-1;
			averageValue=-1;
		}
		vector<float> data;
		int bufferSize;
		int channels;
		float averageValue;
	};

	//-------------------------------------------------------------------------------
	AudioFrame::AudioFrame()
	{
		data = ofPtr<Obj>(new Obj);
	}
	
	//-------------------------------------------------------------------------------
	AudioFrame AudioFrame::newAudioFrame(const float * audioFrame,int bufferSize,const int channels)
	{		
		AudioFrame aFrame;
		aFrame.data->bufferSize = bufferSize;
		aFrame.data->channels = channels;
		aFrame.data->averageValue=0;
		aFrame.data->data.assign(audioFrame,audioFrame+(bufferSize*channels));
		for(int i=0;i<bufferSize;i++)
		{
			/*aFrame->data.push_back(audioFrame[i*channels ]);
			aFrame->data.push_back(audioFrame[i*channels+1]);*/
			aFrame.data->averageValue+=audioFrame[i*channels];
		}
		aFrame.data->averageValue=aFrame.data->averageValue/bufferSize;
		//numInstances++;
		//aFrame->retain(); ??
		//aFrame->release(); // ??
		
		return aFrame;
	}


	//-------------------------------------------------------------------------------
	AudioFrame::~AudioFrame() 
	{
		//printf("deleting AudioFrame!!\n");

		//numInstances--;
	}


	//-------------------------------------------------------------------------------
	vector<float> & AudioFrame::getAudioData()
	{
		return data->data;
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
		return data->bufferSize;
	}
	
	//-------------------------------------------------------------------------------
	int AudioFrame::getChannels()
	{
		return data->channels;
	}
	
	//-------------------------------------------------------------------------------
	float   AudioFrame::getAverageValue()
	{
		return data->averageValue;
	}
}
