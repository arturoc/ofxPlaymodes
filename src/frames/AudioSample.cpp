
#include "AudioSample.h"


using Poco::ScopedLock;

namespace ofxPm{
int AudioSample::numInstances=0;

AudioSample::AudioSample(float * audioSample, int channels) {
    data = NULL;
	data = new float[channels];
	for(int i=0;i<channels;i++)
	{
	    data[i]=audioSample[i];
	}
	this->channels=channels;
    numInstances++;
	this->refreshTimestamp();
}
	
	//-------------------------------------------------------------------------------
	void AudioSample::release() 
	{
	    ScopedLock<ofMutex> lock(*mutex);
		_useCountOfThisObject--;
		if(_useCountOfThisObject == 0) {
			printf("AUDIOFRAME:: deleting object !!!!!!!!!!!!!!!!!!!!!!!!!\n");
			delete this;
		}
	}
	
	//-------------------------------------------------------------------------------
	void AudioSample::retain() 
	{
		ScopedLock<ofMutex> lock(*mutex);
		_useCountOfThisObject++;
	}
	

AudioSample::~AudioSample() {
	delete[] data;
	numInstances--;
}


float * AudioSample::getAudioData(){
    return data;
}	
	
int AudioSample::getChannels(){
    return channels;
}

}
