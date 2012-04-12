
#include "AudioSample.h"


using Poco::ScopedLock;

namespace ofxPm{
int AudioSample::numInstances=0;

	AudioSample::AudioSample(float * audioSample, int _channels) 
	{
		channels=_channels;
		data = new float[channels];
		for(int i=0;i<channels;i++)
		{
			data[i]=audioSample[i];
		}
		numInstances++;
		this->refreshTimestamp();
	//	printf("AS:: chann %d\n",this->channels);
	}
	
	//-------------------------------------------------------------------------------
	void AudioSample::release() 
	{
	    ScopedLock<ofMutex> lock(*mutex);
		_useCountOfThisObject--;
		if(_useCountOfThisObject == 0) {
			printf("AS:: deleting object !!!!!!!!!!!!!!!!!!!!!!!!!\n");
			delete this;
		}
	}
	
	//-------------------------------------------------------------------------------
	void AudioSample::retain() 
	{
		ScopedLock<ofMutex> lock(*mutex);
		_useCountOfThisObject++;
	}
	

	//-------------------------------------------------------------------------------
	AudioSample::~AudioSample() 
	{
		delete[] data;
		numInstances--;
	}


	//-------------------------------------------------------------------------------
	float * AudioSample::getAudioData(){
		return data;
	}	
		
	//-------------------------------------------------------------------------------
	int AudioSample::getChannels(){
		return channels;
	}	
	//-------------------------------------------------------------------------------
	void AudioSample::setAudioData(int _channel, float _value)
	{
		this->data[_channel] = _value;
	}
}
