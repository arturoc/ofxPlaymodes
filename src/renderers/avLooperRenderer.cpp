

#include "avLooperRenderer.h"
#include "VideoFrame.h"

int inc = 1;
//------------------------------------------------------

namespace ofxPm
{
	avLooperRenderer::avLooperRenderer(VideoBuffer & bufferVideo, AudioBufferSamples & bufferAudio) 
	{
		this->vBuffer = &bufferVideo;
		vHeader.setup(*vBuffer);
		
		this->aBuffer = &bufferAudio;		
		aHeader2.setup(*aBuffer);
		
		
		
		// link audio and video headers for event communication a->v
		aHeader2.linkToVideoHeader(vHeader);
		
		sampleIndex=0;
	}

	//------------------------------------------------------

	avLooperRenderer::avLooperRenderer(){
		vBuffer = NULL;
	}
	//------------------------------------------------------

	void avLooperRenderer::setup(VideoBuffer & bufferVideo,AudioBufferSamples & bufferAudio)
	{
		this->vBuffer = &bufferVideo;
		vHeader.setup(*vBuffer);
	
		this->aBuffer = &bufferAudio;		
		aHeader2.setup(*aBuffer);

		// link audio and video headers for event communication a->v
		aHeader2.linkToVideoHeader(vHeader);

		sampleIndex=0;
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
		aHeader2.draw();
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
		aHeader2.draw();
		
		ofSetColor(255,255,255);
		
	}

	//------------------------------------------------------

	VideoHeader* avLooperRenderer::getVideoHeader()
	{
		return &vHeader;
	}
	//------------------------------------------------------
	
	AudioHeaderSample* avLooperRenderer::getAudioHeader()
	{
		return &aHeader2;
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

		int sampleIndex=0;
		AudioFrame* frame = aHeader.getNextAudioFrame();
		aHeader2.resetTick();
		for(int i=0;i<bufferSize;i++)
		{
			AudioSample* aSample = aHeader2.getNextAudioSample();
			
			output[i*nChannels  ] = aSample->getAudioData()[0]; 
			output[i*nChannels+1] = aSample->getAudioData()[0]; 
			
			aHeader2.updateTick();
			aSample->release();
				
		}
		// we need
		frame->release();
	}
	
	//------------------------------------------------------
	void avLooperRenderer::setDelayMs(float ms)
	{
		vHeader.setDelayMs(ms);
		aHeader2.setDelaySamples(ms*96);
	}
	//------------------------------------------------------
	void avLooperRenderer::setSpeed(float sp)
	{
		vHeader.setSpeed(sp);
		aHeader2.setPitch(sp);
	}
	//------------------------------------------------------
	void avLooperRenderer::setOpacity(float v)
	{
		vHeader.setOpacity(int(v*255.0));
		aHeader2.setVolume(v);
	}
	//------------------------------------------------------
	void avLooperRenderer::setInMs(float ms)
	{
		vHeader.setInMs(ms);
		aHeader2.setInSamples((ms*96));
		

	}
	//------------------------------------------------------
	void avLooperRenderer::setOutMs(float ms)
	{
		vHeader.setOutMs(ms);
		aHeader2.setOutSamples((ms*96));

	}
	//------------------------------------------------------
	void avLooperRenderer::setPlaying(bool b)
	{
		vHeader.setPlaying(b);
		aHeader2.setPlaying(b);
	}
	//------------------------------------------------------
	void avLooperRenderer::setLoopToStart()
	{
		vHeader.setLoopToStart();
		aHeader2.setLoopToStart();
	}
	
} //endNameSpace
