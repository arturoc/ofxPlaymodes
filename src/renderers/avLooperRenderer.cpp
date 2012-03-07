

#include "avLooperRenderer.h"
#include "VideoFrame.h"

//------------------------------------------------------

namespace ofxPm
{
	avLooperRenderer::avLooperRenderer(VideoBuffer & bufferVideo, AudioBuffer & bufferAudio) 
	{
		this->vBuffer = &bufferVideo;
		vHeader.setup(*vBuffer);
		
		this->aBuffer = &bufferAudio;		
		aHeader.setup(*aBuffer);
		
		// link audio and video headers for event communication a->v
		aHeader.linkToVideoHeader(vHeader);
		
		sampleIndex=0;
	}

	//------------------------------------------------------

	avLooperRenderer::avLooperRenderer(){
		vBuffer = NULL;
	}
	//------------------------------------------------------

	void avLooperRenderer::setup(VideoBuffer & bufferVideo,AudioBuffer & bufferAudio)
	{
		this->vBuffer = &bufferVideo;
		vHeader.setup(*vBuffer);
	
		this->aBuffer = &bufferAudio;		
		aHeader.setup(*aBuffer);

		// link audio and video headers for event communication a->v
		aHeader.linkToVideoHeader(vHeader);
	}
	//------------------------------------------------------

	avLooperRenderer::~avLooperRenderer() {
		// TODO Auto-generated destructor stub
	}

	//------------------------------------------------------

	void avLooperRenderer::draw()
	{
		VideoFrame * frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			frame->getTextureRef().draw(0,0);
			frame->release();
		}
		vHeader.draw();
		aHeader.draw();
	}
	//------------------------------------------------------

	void avLooperRenderer::draw(int x,int y,int w,int h)
	{
				
		VideoFrame * frame = vHeader.getNextVideoFrame();

		if(frame!=NULL){
			ofSetColor(vHeader.getOpacity(),vHeader.getOpacity(),vHeader.getOpacity());
			frame->getTextureRef().draw(x,y,w,h);
			frame->release();
		}
		
		// draw renderer interface 
		aBuffer->draw();
		vBuffer->draw();

		vHeader.draw();
		aHeader.draw();
		
		ofSetColor(255,255,255);
		
	}

	//------------------------------------------------------

	VideoHeader* avLooperRenderer::getVideoHeader()
	{
		return &vHeader;
	}
	//------------------------------------------------------
	
	AudioHeader* avLooperRenderer::getAudioHeader()
	{
		return &aHeader;
	}
	
	//------------------------------------------------------

	void avLooperRenderer::audioRequested (float * output, int bufferSize, int nChannels)
	{
		// get the next audio frame
		// and copy to the sound card buffer
		
//		// amb memcpy
//		AudioFrame * frame= aHeader.getNextAudioFrame();
//		memcpy(output,frame->getAudioFrame(),sizeof(float)*bufferSize*nChannels);
//		frame->release();

		
		AudioFrame * frame= aHeader.getNextAudioFrame();
		for(int i=0;i<bufferSize;i++)
		{
//			output[i*nChannels  ] = frame->getAudioData()[i*nChannels  ]*1.0f;//aHeader.getVolume(); 
//			output[i*nChannels+1] = frame->getAudioData()[i*nChannels+1]*1.0f;//aHeader.getVolume(); 
			output[i*nChannels  ] = aBuffer->getAudioSample(sampleIndex); 
			output[i*nChannels+1] = aBuffer->getAudioSample(sampleIndex);
			//output[i*nChannels+1] = aBuffer->getAudioSample(sampleIndex*nChannels+1); 
			
			//sampleIndex=(sampleIndex+1)%aBuffer->getMaxSizeInSamples();
			sampleIndex=(sampleIndex+1);
			if (sampleIndex>aBuffer->getMaxSizeInSamples()) sampleIndex=0;
		}
		// we need 
		frame->release();
	}
	
	//------------------------------------------------------
	void avLooperRenderer::setDelayMs(int ms)
	{
		vHeader.setDelayMs(ms);
		aHeader.setDelayMs(ms);
	}
	//------------------------------------------------------
	void avLooperRenderer::setSpeed(float sp)
	{
		vHeader.setSpeed(sp);
		aHeader.setSpeed(sp);
	}
	//------------------------------------------------------
	void avLooperRenderer::setOpacity(float v)
	{
		vHeader.setOpacity(int(v*255.0));
		aHeader.setVolume(v);
	}
	//------------------------------------------------------
	void avLooperRenderer::setInMs(int ms)
	{
		vHeader.setInMs(ms);
		aHeader.setInMs(ms);

	}
	//------------------------------------------------------
	void avLooperRenderer::setOutMs(int ms)
	{
		vHeader.setOutMs(ms);
		aHeader.setOutMs(ms);
	}
	//------------------------------------------------------
	void avLooperRenderer::setPlaying(bool b)
	{
		vHeader.setPlaying(b);
		aHeader.setPlaying(b);
	}
	//------------------------------------------------------
	void avLooperRenderer::setLoopToStart()
	{
		vHeader.setLoopToStart();
		aHeader.setLoopToStart();
	}
	
} //endNameSpace
