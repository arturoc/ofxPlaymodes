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

int AudioFrame::total_num_frames;
map<AudioFormat,vector<ofPtr<AudioFrame::Obj> > > AudioFrame::pool;
ofMutex AudioFrame::poolMutex;

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

	void AudioFrame::poolDeleter(AudioFrame::Obj * obj){
		pool[AudioFormat(obj->bufferSize,obj->channels)].push_back(ofPtr<Obj>(obj,&AudioFrame::poolDeleter));
	}

	//-------------------------------------------------------------------------------
	AudioFrame::AudioFrame()
	{
		data = ofPtr<Obj>(new Obj, &AudioFrame::poolDeleter);
	}
	
	//-------------------------------------------------------------------------------
	AudioFrame AudioFrame::newAudioFrame(const float * audioFrame,int bufferSize,const int channels)
	{		

		AudioFormat format(bufferSize,channels);
		ScopedLock<ofMutex> lock(poolMutex);
		if(!pool[format].empty()){
			//cout << "returning frame from pool" << endl;
			AudioFrame aFrame;
			aFrame.data = pool[format].back();
			aFrame.refreshTimestamp();
			aFrame.getAudioData().assign(audioFrame,audioFrame+(bufferSize*channels));
			pool[format].pop_back();
			for(int i=0;i<bufferSize;i++)
			{
				aFrame.data->averageValue+=audioFrame[i*channels];
			}
			aFrame.data->averageValue=aFrame.data->averageValue/bufferSize;
			return aFrame;
		}else{
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
			return aFrame;
		}
		
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


	AudioFrame::operator void*(){
		return data.get();
	}
}
