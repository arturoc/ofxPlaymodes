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
		aSampleRate				=sampleR;
		aSoundStreamBufferSize	=bufferS;
		aNumChannels					=numCh;		
		this->maxSizeSamples	= sizeInSecs * aSampleRate;
		stopped					=false;
		resume();	
		
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
		printf("!!!!!!!!!!!!!\n");
		// Samples managing, store AudioSamples on the samples cue.
		float* audioFrameData = new float[frame.getBufferSize()*aNumChannels];
		audioFrameData = frame.getAudioData();
		AudioSample* aS;
		float* sampleData = new float[aNumChannels];
		
		// for every position on the audioFrame buffer ...
		for(int i=0;i<frame.getBufferSize();i++)
		{
			printf(".............\n");

			// copy data from AudioFrame to AudioSample
			for(int j=0;j<aNumChannels;j++)
			{
				sampleData[j] = audioFrameData[i*aNumChannels+j];
			}
			aS = new AudioSample(sampleData,aNumChannels);
			samples.push_back(aS);
			
			if(sizeInSamples()>maxSizeSamples){
				samples.erase(samples.begin());
			}
			
		}
		
		newFrameEvent.notify(this,frame);
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

	void AudioBufferSamples::draw()
	{
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
//	
		
//		computeWaveforms();
//		
//		for(int i = 0; i < waveforms.size(); i++){
////			ofPushMatrix();
////			ofTranslate( normalizedXtoScreenX(computedZoomBounds.min, zoomBounds)  - normalizedXtoScreenX(zoomBounds.min, zoomBounds), 0, 0);
////			ofScale(computedZoomBounds.span()/zoomBounds.span(), 1, 1);
//			waveforms[i].draw();
//		//	ofPopMatrix();
//		}
/*
		if (samples.size()>0)
		{
			int	y = 500;
			float factor = samples.size() / maxSizeSamples;
			int totalPixelsToDraw = int(ofGetWidth()-(2*PMDRAWSPACING)*factor);
			for(int i=0;i<totalPixelsToDraw;i++)
			{
				int count = (float(i)/float(totalPixelsToDraw))*samples.size();
				//ofRect(i,y,1,100);
				ofRect(i+PMDRAWSPACING,y,1,samples[count]->getAudioData()[0]*100.0);
				
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
		return getSampleRate();
	}

	//------------------------------------------------------
	void AudioBufferSamples::computeWaveforms()
	{
		int height=100;
		int originX=PMDRAWSPACING;
		int originY=500;
		int width =ofGetWidth()-(2*PMDRAWSPACING);

		AudioSample* aS;
		
		waveforms.clear();
		//float normalizationRatio = timeline->getDurationInSeconds() / player.getDuration(); //need to figure this out for framebased...but for now we are doing time based
		float trackHeight = height/(1+aNumChannels);
		int numSamples =  this->sizeInSamples(); //? / player.getNumChannels();
		int pixelsPerSample = numSamples / width;
		for(int c = 0; c < aNumChannels; c++){
			ofPolyline preview;
			int lastFrameIndex = 0;
			for(float i = originX; i < originX+width; i++){
				//float pointInTrack = screenXtoNormalizedX( i, zoomBounds ) * normalizationRatio; //will scale the screenX into wave's 0-1.0
				float pointInTrack = float(i)/float(width);
				float trackCenter = originY + trackHeight * (c+1);
				if(pointInTrack <= 1.0){
					//draw sample at pointInTrack * waveDuration;
					int frameIndex = pointInTrack * numSamples;					
					float losample = 0;
					float hisample = 0;
					for(int f = lastFrameIndex; f < frameIndex; f++){
						//int sampleIndex = f * aNumChannels + c;
						int sampleIndex = f ;
						//float subpixelSample = samples[sampleIndex][c]/32565.0;
						aS = this->getAudioSample(sampleIndex);

						float subpixelSample = aS->getAudioData()[c]/32565.0;
						if(subpixelSample < losample) {
							losample = subpixelSample;
						}
						if(subpixelSample > hisample) {
							hisample = subpixelSample;
						}
					}
					if(losample == 0 && hisample == 0){
						preview.addVertex(i, trackCenter);
					}
					else {
						if(losample != 0){
							preview.addVertex(i, trackCenter - losample * trackHeight);
						}
						if(hisample != 0){
							//ofVertex(i, trackCenter - hisample * trackHeight);
							preview.addVertex(i, trackCenter - hisample * trackHeight);
						}
					}
					lastFrameIndex = frameIndex;
				}
				else{
					preview.addVertex(i,trackCenter);
				}
			}
			waveforms.push_back(preview);
		}
		//shouldRecomputePreview = false;
	}
	
	
}
