

#ifndef AVLOOPERRENDERER_H_
#define AVLOOPERRENDERER_H_

#include "VideoSink.h"
#include "VideoSource.h"
#include "VideoHeader.h"
#include "AudioSink.h"
#include "AudioSource.h"
#include "AudioHeader.h"
#include "AudioHeaderSample.h"


namespace ofxPm
{

//------------------------------------------------------
class avLooperRenderer: public VideoSink, public ofBaseSoundOutput{
public:
	avLooperRenderer();
	avLooperRenderer(VideoBuffer & buffer, AudioBufferSamples & bufferAudio);
	virtual ~avLooperRenderer();

	void				setup(VideoBuffer & bufferVideo, AudioBufferSamples & bufferAudio);
	VideoHeader*		getVideoHeader();
	AudioHeaderSample*		getAudioHeader();
	
	void				draw();
	void				draw(int x,int y,int w,int h);

	void				audioRequested(float * output, int bufferSize, int nChannels);
	
	void				setDelayMs(float ms);
	void				setSpeed(float sp);
	void				setOpacity(float value);
	void				setInMs(float ms);
	void				setOutMs(float ms);
	void				setLengthMs(float ms);
	void				setPlaying(bool b);
	void				setLoopToStart();
	
private:
	VideoBuffer*		vBuffer;
	VideoHeader			vHeader;
	AudioBufferSamples*		aBuffer;
	//AudioHeader			aHeader;
	AudioHeaderSample	aHeader2;
	
	
	int					sampleIndex;

	//VideoSource*		source;
};
//------------------------------------------------------

}
#endif 
