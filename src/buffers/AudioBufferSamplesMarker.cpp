
#include "AudioBufferSamplesMarker.h"

//------------------------------------------------------

namespace ofxPm
{
	
	AudioBufferSamplesMarker::AudioBufferSamplesMarker(unsigned int index, AudioBufferSamples &buffer)
	{
		this->setup(index,buffer);
	}
	
	
	//------------------------------------------------------
	
	AudioBufferSamplesMarker::AudioBufferSamplesMarker()
	{
		this->aBuffer = NULL;
		this->length = 0;
		this->index= 0;
		this->indexMax = 0;
		this->indexMin = 0;
		
		
	}
	
	//------------------------------------------------------
	
	AudioBufferSamplesMarker::~AudioBufferSamplesMarker()
	{
		
		
	}
	
	//------------------------------------------------------
	
	void AudioBufferSamplesMarker::setup(unsigned int _index, AudioBufferSamples & buffer)
	{
		this->aBuffer=&buffer;
		this->length = DEFAULT_LENGTH;
		// because of matching of samples coming from video frames or from audio frames,
		// we need to correct the indexes with the unused samples.
		
		this->setIndex(_index);

		//this->index =_index + aBuffer->getUnusedSamples(); 
		//this->indexMin = this->getIndex() - this->getLength();
		//this->indexMax = this->getIndex() + this->getLength();
				
		printf("ABSM::setupMarker:: index %d   length %d || MIN %d || MAX %d \n",int(this->getIndex()),int(this->getLength()),int(this->getMin()),int(this->getMax()));
	}

	//------------------------------------------------------
	
	void AudioBufferSamplesMarker::setIndex(unsigned int _index)
	{
		//int bufferSize = aBuffer->sizeInSamples();
		int bufferSize = aBuffer->getUsedSizeInSamples();
		
		// check for minimum & maximum position not out of buffer bounds
		//		if (((_index-length)>=0) && (_index+length<bufferSize) )
		if (((_index-length)>=0) && (_index<bufferSize) )
		{
			index = _index;
			indexMin = index-length;
			indexMax = index+length;
		}
		// if the case was out of bounds by minimum < 0
		else if ((_index-length)<0)
		{
			index = length;
			indexMin = index-length;
			indexMax = index+length;
		}
		// if the case was out of bounds by maximum > bufferSize
		else if ((_index)>=bufferSize)
		{
			index = (bufferSize-1);
			indexMin = index-length;
			indexMax = index+length;
		}
		
//		// update audio data on both ranges min & max
//		for(int i=0;i<length;i++)
//		{
//			// min sample i
//			AudioSample* aS = new AudioSample(aBuffer->getAudioSample((index-length)+i)->getAudioData(),aBuffer->getNumChannels());
//			
//			//AudioSample(float * audioSample, int channels)
//			//minSamples.push_back(
//		}
	}
	
	//------------------------------------------------------
	
	void AudioBufferSamplesMarker::draw()
	{
		
	}
	
	//------------------------------------------------------
	
	unsigned int AudioBufferSamplesMarker::getIndex()
	{
		return index;
	}
	
	
	//------------------------------------------------------
	
	unsigned int AudioBufferSamplesMarker::getMin()
	{
		return indexMin;
	}

	//------------------------------------------------------
	
	unsigned int AudioBufferSamplesMarker::getMax()
	{
		return indexMax;
	}

	//------------------------------------------------------
	
	unsigned int AudioBufferSamplesMarker::getLength()
	{
		return length;
	}
	
	//------------------------------------------------------

	void AudioBufferSamplesMarker::setLength(int nSamples)
	{
		length = nSamples;
		indexMin = index-length;
		indexMax = index+length;
		
	}
}
