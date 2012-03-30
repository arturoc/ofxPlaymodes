/*
 * AudioBuffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "AudioBufferSamples.h"

namespace ofxPm{
AudioBufferSamples::AudioBufferSamples(AudioSource & source, float size, int sampleR, int bufferS,int numCh) {
	setup(source,size,sampleR,bufferS,numCh);
}
		
	//------------------------------------------------------
	AudioBufferSamples::AudioBufferSamples() {
		source = NULL;
		fps=0;
		stopped=false;
		totalFrames=0;
	}

	//------------------------------------------------------
		
	AudioBufferSamples::~AudioBufferSamples() 
	{

	}

	//------------------------------------------------------

	void AudioBufferSamples::setup(AudioSource & source,float sizeInSecs,int sampleR, int bufferS, int numCh)
	{		
		this->source=&source;
		fps=source.getFps();
		aSampleRate=sampleR;
		aSoundStreamBufferSize=bufferS;
		aNumCh=numCh;
		totalFrames=0;
		
		this->maxSizeSamples	= sizeInSecs * aSampleRate;
		this->maxSize			= this->maxSizeSamples/aSoundStreamBufferSize +1 ;
		resume();	
		stopped					= false;
		
	}
	
	//------------------------------------------------------
	unsigned int AudioBufferSamples::size(){
		return frames.size();
	}
	//------------------------------------------------------
	unsigned int AudioBufferSamples::getMaxSize(){
		return maxSize;
	}
	//------------------------------------------------------	
	unsigned int AudioBufferSamples::sizeInSamples()
	{
		//return this->samples.size();
		return (frames.size())*(aSoundStreamBufferSize);
	}
	//------------------------------------------------------
	unsigned int AudioBufferSamples::getMaxSizeInSamples()
	{
		return maxSizeSamples;
	}
	//------------------------------------------------------
	
	void AudioBufferSamples::newAudioFrame(AudioFrame &frame)
	{
		
		if(size()==0)initTime=frame.getTimestamp();
		
		// AudioFrames managing, store AudioFrame on the cue.
		totalFrames++;
		frames.push_back(&frame);
		frame.retain();		
		printf("ABS:: size %d vs maxSize %d\n",size(),maxSize);
		if(size()>maxSize)
		{
			frames.front()->release();
			frames.erase(frames.begin());
		}
		
		// what for ??
		newFrameEvent.notify(this,frame);		 
	}

	
	
	//------------------------------------------------------
	
	AudioSample* AudioBufferSamples::getAudioSample(int index)
	{
		
		int samplesPerFrame = aSoundStreamBufferSize;
		int whichAudioFrame = index / samplesPerFrame;
		int whichSampleInAudioFrame = index % samplesPerFrame;

		//printf("num Chan :: %d || index :: %d || samplesPerFrame %d || whichAudioFrame %d || whichSample %d \n",aNumCh,index,samplesPerFrame,whichAudioFrame,whichSampleInAudioFrame);
		
		
		
		float* sampleData = new float[aNumCh];

		for(int i=0;i<aNumCh;i++)
		{
			sampleData[i] = frames[whichAudioFrame]->getAudioData()[whichSampleInAudioFrame*aNumCh+i];			
		}
		AudioSample *aS = new AudioSample(sampleData,aNumCh);
		AudioSample *aSaux = aS;
		aS->retain();
		//aS->release();
		delete[] sampleData;
		return aSaux;
	}
	
	//------------------------------------------------------
	
	float AudioBufferSamples::getFps(){
		return this->fps;
	}

	//------------------------------------------------------

	float AudioBufferSamples::getRealFPS(){
//		 if(size()>10)
//			return 10.0/(float)(frames.back()->getTimestamp()-frames[size()-11]->getTimestamp())*1000000.0;
//		else
//			return 0;
	}

	//------------------------------------------------------

	void AudioBufferSamples::draw()
	{
//		
		float length=float(size())/((float)maxSize)*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(maxSize);
		//		float length = (float)size()/((float)maxSize*(float)ofGetWidth()-PMDRAWSPACING);
		float oneFrame =((float)ofGetWidth()-float(2*PMDRAWSPACING))/(float)maxSize;

		if(stopped==true) ofSetColor(255,0,0);
		else ofSetColor(0,120,255);
		ofLine(PMDRAWSPACING,650,length,650);
		for(int i=0;i<size();i++){
			ofSetColor(0,120,255);
			// draw wave
			if(i%2==0) ofRect((oneLength*i)+PMDRAWSPACING,650-frames[i]->getAverageValue()*150,oneLength*2,(frames[i]->getAverageValue()*450+1));
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
		
		/*
		int audioBuffDrawPos = 90;
		ofSetColor(0,255,255);
		//if (samples.size()>0)
		if (frames.size()>0)
			{
			 int	y = 500;
			 //float factor = float(samples.size()) / float(maxSizeSamples);
			 float factor = float(frames.size()*aSoundStreamBufferSize) / float(maxSizeSamples);
			 int totalPixelsToDraw = int(float(int(ofGetWidth())-2*PMDRAWSPACING)*factor);
			 int count;
			 for(int i=0;i<int(float(totalPixelsToDraw));i++)
			 {
				 //count = (float(i)/float(totalPixelsToDraw))*samples.size();
				 count = (float(i)/float(totalPixelsToDraw))*(frames.size()*aSoundStreamBufferSize);
				 //ofRect(i,y,1,100);
				 this->lock();
				 //ofRect(i+PMDRAWSPACING,PMDRAWELEMENTSY-audioBuffDrawPos+40,1,samples[count]->getAudioData()[0]*100.0);
				 this->unlock();
			 }
		 }
	*/
		
	}

	//------------------------------------------------------

	void AudioBufferSamples::stop(){
		stopped=true;
		ofRemoveListener(source->newFrameEvent,this,&AudioBufferSamples::newAudioFrame);
	}

	//------------------------------------------------------
	void AudioBufferSamples::resume(){
		stopped=false;
		ofAddListener(source->newFrameEvent,this,&AudioBufferSamples::newAudioFrame);
	}


	//------------------------------------------------------
	int AudioBufferSamples::getSoundStreamBufferSize()
	{
		return aSoundStreamBufferSize;
	}

	//------------------------------------------------------
	int AudioBufferSamples::getSampleRate()
	{
		return aSampleRate;
	}
	
}
