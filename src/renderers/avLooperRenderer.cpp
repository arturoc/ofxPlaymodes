

#include "avLooperRenderer.h"
#include "VideoFrame.h"

int inc = 1;
//------------------------------------------------------

namespace ofxPm
{
	avLooperRenderer::avLooperRenderer(VideoBuffer & bufferVideo, AudioBufferSamples & bufferAudio) 
	{
		this->setup(bufferVideo,bufferAudio);
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
		vHeader.setDriveMode(1); // audio driven mode
	
		this->aBuffer = &bufferAudio;		
		aHeader2.setup(*aBuffer);

		sampleIndex			= 0;
		videoOffsetInMs		= 0;
		audioSampleRate		= aBuffer->getSampleRate();
		maximumSizeInMs		= int(float(aBuffer->getMaxSizeInSamples())/float(audioSampleRate))*1000;
		
		lastIn = 0.0;
		lastOut = 0.0;
	}
	//------------------------------------------------------

	avLooperRenderer::~avLooperRenderer() {
		// TODO Auto-generated destructor stub
	}

	//------------------------------------------------------

	void avLooperRenderer::draw()
	{
		VideoFrame frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			// draw the frame texture to screen
			frame.getTextureRef().draw(0,0);
		}
		// draw av header interfaces
		vHeader.draw();
		aHeader2.draw();
	}
	//------------------------------------------------------

	void avLooperRenderer::draw(int x,int y,int w,int h)
	{
		// audio -> video Sync !!
		//////////////////////////
		// 1
		//VideoFrame * frame = vHeader.getVideoFrame(int(float(aHeader2.getIndex())/float(aBuffer->sizeInSamples()))*vBuffer->getMaxSize());
		//printf("index %d of size %d = %d\n",aHeader2.getIndex(),aBuffer->sizeInSamples(),int(float(aHeader2.getIndex())/float(aBuffer->sizeInSamples()))*vBuffer->getMaxSize());
		
		// 2
		float delayToVideo = (float(aHeader2.getIndex()) / float(audioSampleRate)) * 1000.0; 
		vHeader.setDelayMs(float(maximumSizeInMs)-delayToVideo-float(videoOffsetInMs));
		//printf("avR ::DELAY is = %f || maxSize %d delayToVideo in ms = %f / index %d\n",float(maximumSizeInMs)-delayToVideo-float(videoOffsetInMs),maximumSizeInMs,delayToVideo,aHeader2.getIndex());
		//printf("AVLR:: videoDelayMs :: %f \n",float(maximumSizeInMs)-delayToVideo-float(videoOffsetInMs));
		
		VideoFrame frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			// draw the frame texture to screen
			ofSetColor(vHeader.getOpacity(),vHeader.getOpacity(),vHeader.getOpacity());
			frame.getTextureRef().draw(x,y,w,h);
		}
		
		// draw av header interfaces
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
//		AudioFrame * frame= aHeader.getNextAudioFrame();
		aHeader2.resetTick();
		for(int i=0;i<bufferSize;i++)
		{
			AudioSample aSample = aHeader2.getNextAudioSample();
			//if (aSample=NULL) printf("NULL!!!!!!\n");
			//printf("AVLR::sample channels %d\n",aSample.getChannels());
			//output[i*nChannels  ] = aSample->getAudioData()[0] * aHeader2.getVolume(); 
			output[i*nChannels  ] = aSample.getAudioData()[0] * aHeader2.getVolume(); 
			//output[i*nChannels+1] = aSample->getAudioData()[0] * aHeader2.getVolume(); 
			
			aHeader2.updateTick();
				
		}
		// we need
//		frame->release();
 
 
	}
	
	//------------------------------------------------------
	void avLooperRenderer::setDelayMs(float ms)
	{
		// sync!!		vHeader.setDelayMs(ms);
		aHeader2.setDelaySamples(ms*(audioSampleRate/1000));
	}
	//------------------------------------------------------
	void avLooperRenderer::setSpeed(float sp)
	{
		// sync!!		vHeader.setSpeed(sp);
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
		// SYNC		vHeader.setInMs(ms);
		aHeader2.setInSamples(ms*(audioSampleRate/1000));
		//lastIn = ms*(audioSampleRate/1000);
	}
	//------------------------------------------------------
	void avLooperRenderer::setOutMs(float ms)
	{
		//SYNC		vHeader.setOutMs(ms);
		aHeader2.setOutSamples(ms*(audioSampleRate/1000));
		//lastOut = ms*(audioSampleRate/1000);
	}
	//------------------------------------------------------
	void avLooperRenderer::setLengthMs(float ms)
	{
		aHeader2.setLengthSamples(ms*(audioSampleRate/1000));
		
	}
	//------------------------------------------------------
	void avLooperRenderer::setPlaying(bool b)
	{
		//SYNC		vHeader.setPlaying(b);
		aHeader2.setPlaying(b);
	}
	//------------------------------------------------------
	void avLooperRenderer::setLoopToStart()
	{
		//SYNC		vHeader.setLoopToStart();
		aHeader2.setLoopToStart();
	}
	//------------------------------------------------------
    void avLooperRenderer::setVideoOffsetInMs(int ms)
    {
		videoOffsetInMs = ms;
	}
	//------------------------------------------------------
    int avLooperRenderer::getVideoOffsetInMs()
    {
		return (videoOffsetInMs);
	}
	//------------------------------------------------------
    void avLooperRenderer::executeInOut()
    {
		aHeader2.setInSamples(lastIn);
		aHeader2.setOutSamples(lastOut);
	}
	//------------------------------------------------------
    void avLooperRenderer::setCrossfadeWidth(int nSamples)
    {
		aHeader2.setDeClickLength(nSamples);
	}
	//------------------------------------------------------
    void avLooperRenderer::setInSamples(unsigned int s)
    {
		aHeader2.setInSamples(s);
		aHeader2.setDelaySamples(s);
	}
	//------------------------------------------------------
	void avLooperRenderer::setLengthSamples(unsigned int s)
	{
		aHeader2.setLengthSamples(s);
	}
	
	
} //endNameSpace
