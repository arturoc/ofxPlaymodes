/*
 * AudioBuffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "AudioBufferSamples.h"

namespace ofxPm{
	
	//-----------------------------------------------------------------------------------------
	AudioBufferSamples::AudioBufferSamples(AudioSource & source, float size, int sampleR, int bufferS,int numCh) 
	{
		setup(source,size,sampleR,bufferS,numCh);
	}
	//-----------------------------------------------------------------------------------------
	AudioBufferSamples::AudioBufferSamples() {
		source = NULL;
		fps=0;
		stopped=false;
	}
	//-----------------------------------------------------------------------------------------		
	AudioBufferSamples::~AudioBufferSamples() 
	{

	}
	//-----------------------------------------------------------------------------------------
	void AudioBufferSamples::setup(AudioSource & source,float sizeInSecs,int sampleR, int bufferS, int numCh)
	{		
		this->source=&source;
		fps=source.getFps();
		aSampleRate=sampleR;
		aSoundStreamBufferSize=bufferS;
		aNumCh=numCh;
		
		// this is the max size related to seconds ... but ... keep reading :
		this->usedSizeSamples	= sizeInSecs * aSampleRate;
		this->maxSize			= this->usedSizeSamples/aSoundStreamBufferSize +1 ;
		// ? because aex. to fill 7s we need 656,25 AudioFrames ... we need 657 AF to be declared.
		this->maxSizeSamples	= this->maxSize * aSoundStreamBufferSize;
		this->unusedSamples		= maxSizeSamples - usedSizeSamples;
		
		resume();	
		stopped					= false;
		
	}
	
	//----------------------------------------------------------------------------------------
	unsigned int AudioBufferSamples::size(){
		return frames.size();
	}
	//----------------------------------------------------------------------------------------
	unsigned int AudioBufferSamples::getMaxSize(){
		return maxSize;
	}
	//----------------------------------------------------------------------------------------
	unsigned int AudioBufferSamples::sizeInSamples()
	{
		//return this->samples.size();
		return (frames.size())*(aSoundStreamBufferSize);
	}
	//----------------------------------------------------------------------------------------
	unsigned int AudioBufferSamples::getMaxSizeInSamples()
	{
		return maxSizeSamples;
	}
	//----------------------------------------------------------------------------------------
	unsigned int AudioBufferSamples::getUsedSizeInSamples()
	{
		return usedSizeSamples;
	}
	//----------------------------------------------------------------------------------------	
	void AudioBufferSamples::newAudioFrame(AudioFrame &frame)
	{		
		if(size()==0)initTime=frame.getTimestamp();
		
		// AudioFrames managing, store AudioFrame on the cue.
		frames.push_back(frame);

		if(size()>maxSize)
		{
			frames.erase(frames.begin());
		}
		// what for ??
		newFrameEvent.notify(this,frame);		 
	}
	//----------------------------------------------------------------------------------------	
	AudioSample  AudioBufferSamples::getAudioSample(int index)
	{
		
		int samplesPerFrame = aSoundStreamBufferSize;
		int whichAudioFrame = index / samplesPerFrame;
		int whichSampleInAudioFrame = index % samplesPerFrame;

		//printf("ABS :: num Chan :: %d || index :: %d || samplesPerFrame %d || whichAudioFrame %d || whichSample %d \n",aNumCh,index,samplesPerFrame,whichAudioFrame,whichSampleInAudioFrame);
		/*float* sampleData = new float[aNumCh];

		for(int i=0;i<aNumCh;i++)
		{
			sampleData[i] = frames[whichAudioFrame]->getAudioData()[whichSampleInAudioFrame*aNumCh+i];			
		}
		
		AudioSample * aS = new AudioSample(sampleData,aNumCh);
		delete[] sampleData;*/
		
		return AudioSample(&frames[whichAudioFrame].getAudioData()[whichSampleInAudioFrame*aNumCh],aNumCh);
	}
	
	//----------------------------------------------------------------------------------------	
	float AudioBufferSamples::getFps()
	{
		return this->fps;
	}
	//----------------------------------------------------------------------------------------
	float AudioBufferSamples::getRealFPS()
	{
		return 0.0;
	}
	//----------------------------------------------------------------------------------------
	void AudioBufferSamples::draw()
	{
		float length=float(size())/((float)maxSize)*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(maxSize);
		float oneFrame =((float)ofGetWidth()-float(2*PMDRAWSPACING))/(float)maxSize;

		if(stopped==true) ofSetColor(255,0,0);
		else ofSetColor(0,120,255);
		ofLine(PMDRAWSPACING,650,length,650);
		for(int i=0;i<size();i++){
			ofSetColor(0,120,255);
			// draw wave
			if(i%2==0) ofRect((oneLength*i)+PMDRAWSPACING,650-frames[i].getAverageValue()*150,oneLength*2,(frames[i].getAverageValue()*450+1));
			// draw grid
			float X = fmod(i,source->getFps());
			if(X<1.0)
			{
				ofSetColor(0,255,255);
				ofLine((oneLength*i)+PMDRAWSPACING,650,(oneLength*i)+PMDRAWSPACING,640);
				ofDrawBitmapString(ofToString(float(size()-i)/source->getFps()),(oneLength*i)+PMDRAWSPACING,635);
				// + " s"
			}
		}
		
	}
	//---------------------------------------------------------------------------------------
	void AudioBufferSamples::stop()
	{
		stopped=true;
		ofRemoveListener(source->newFrameEvent,this,&AudioBufferSamples::newAudioFrame);
	}
	//---------------------------------------------------------------------------------------
	void AudioBufferSamples::resume()
	{
		stopped=false;
		ofAddListener(source->newFrameEvent,this,&AudioBufferSamples::newAudioFrame);
	}
	//--------------------------------------------------------------------------------------
	int AudioBufferSamples::getSoundStreamBufferSize()
	{
		return aSoundStreamBufferSize;
	}
	//--------------------------------------------------------------------------------------
	int AudioBufferSamples::getSampleRate()
	{
		return aSampleRate;
	}
	//--------------------------------------------------------------------------------------
	int AudioBufferSamples::getNumChannels()
	{
		return aNumCh;
	}
	//--------------------------------------------------------------------------------------
	int AudioBufferSamples::getUnusedSamples()
	{
		return unusedSamples;
	}
	bool AudioBufferSamples::getIsStopped()
	{
		return stopped;
	}

}
