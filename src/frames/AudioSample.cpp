
#include "AudioSample.h"



namespace ofxPm{

	AudioSample::AudioSample(float * audioSample, int _channels) 
	{
		channels=_channels;
		data = audioSample;/*new float[channels];
		for(int i=0;i<channels;i++)
		{
			data[i]=audioSample[i];
		}
		numInstances++;*/
	//	printf("AS:: chann %d\n",this->channels);
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
