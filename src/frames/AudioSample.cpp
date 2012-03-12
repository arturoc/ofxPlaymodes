
#include "AudioSample.h"



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
	
void AudioSample::release() {
	
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
