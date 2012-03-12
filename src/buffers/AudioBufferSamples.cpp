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
		
		this->maxSizeSamples	= sizeInSecs * aSampleRate;
		resume();	
		stopped					=false;
		
	}
	//------------------------------------------------------	
	unsigned int AudioBufferSamples::sizeInSamples()
	{
		return this->samples.size();
	}
	//------------------------------------------------------
	unsigned int AudioBufferSamples::getMaxSizeInSamples()
	{
		return maxSizeSamples;
	}
	//------------------------------------------------------
	
	void AudioBufferSamples::newAudioFrame(AudioFrame &frame)
	{
		// Samples managing, store AudioSamples on the samples cue.
		AudioSample* aS;
		float* sampleData = new float[aNumCh];
		
		this->lock();

		// for every position on the audioFrame buffer ...
		for(int i=0;i<frame.getBufferSize();i++)
		{
			// copy data from AudioFrame to AudioSample
			for(int j=0;j<aNumCh;j++)
			{
				//sampleData[j] = audioFrameData[i*aNumCh+j];
				sampleData[j] = frame.getAudioData()[i*aNumCh+j];
			}
			aS = new AudioSample(sampleData,aNumCh); 
			samples.push_back(aS);
			
			if(sizeInSamples()>maxSizeSamples){
				samples.erase(samples.begin());
			}
			
		}
		this->unlock();
		
		//newFrameEvent.notify(this,frame);
		delete[] sampleData;
	}

	
	
	//------------------------------------------------------
	
	AudioSample* AudioBufferSamples::getAudioSample(int index)
	{
		return samples[index];
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
//		float length=float(size())/((float)maxSize)*(float)(ofGetWidth()-PMDRAWSPACING*2);
//		float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(maxSize);
//		//		float length = (float)size()/((float)maxSize*(float)ofGetWidth()-PMDRAWSPACING);
//		float oneFrame =((float)ofGetWidth()-float(2*PMDRAWSPACING))/(float)maxSize;
//
//		if(stopped==true) ofSetColor(255,0,0);
//		else ofSetColor(0,120,255);
//		ofLine(PMDRAWSPACING,650,length,650);
//		for(int i=0;i<size();i++){
//			ofSetColor(0,120,255);
//			// draw wave
//			if(i%2==0) ofRect((oneLength*i)+PMDRAWSPACING,650-frames[i]->getAverageValue()*150,oneLength*2,(frames[i]->getAverageValue()*450+1));
//			// draw grid
//			float X = fmod(i,source->getFps());
//			if(X<1.0)
//			{
//				ofSetColor(0,255,255);
//				ofLine((oneLength*i)+PMDRAWSPACING,650,(oneLength*i)+PMDRAWSPACING,640);
//				ofDrawBitmapString(ofToString(float(size()-i)/source->getFps()),(oneLength*i)+PMDRAWSPACING,635);
//				// + " s"
//			}
//		}
		
		int audioBuffDrawPos = 90;
		ofSetColor(0,255,255);
		if (samples.size()>0)
		 {
			 int	y = 500;
			 float factor = float(samples.size()) / float(maxSizeSamples);
			 int totalPixelsToDraw = int(float(int(ofGetWidth())-2*PMDRAWSPACING)*factor);
			 int count;
			 for(int i=0;i<int(float(totalPixelsToDraw));i++)
			 {
				 count = (float(i)/float(totalPixelsToDraw))*samples.size();
				 //ofRect(i,y,1,100);
				 this->lock();
				 ofRect(i+PMDRAWSPACING,PMDRAWELEMENTSY-audioBuffDrawPos+40,1,samples[count]->getAudioData()[0]*100.0);
				 this->unlock();
			 }
		 }
	
		
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
