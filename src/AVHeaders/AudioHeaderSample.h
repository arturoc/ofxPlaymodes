/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOHEADERSAMPLE_H_INCLUDED
#define AUDIOHEADERSAMPLE_H_INCLUDED

#include "AudioSink.h"
#include "AudioSource.h"
#include "AudioBuffer.h"
#include "VideoHeader.h"
#include "AudioSample.h"
#include "AudioBufferSamples.h"
#include "AudioBufferSamplesMarker.h"


namespace ofxPm{
class AudioHeaderSample:public AudioSink, public AudioSource{
public:
    AudioHeaderSample(AudioBufferSamples &buffer);
    AudioHeaderSample();
    ~AudioHeaderSample();

	void			setup(AudioBufferSamples & buffer);
    void			draw();
    AudioSample		getNextAudioSample();
//    AudioSample*	getAudioSample(int _index);
    float			getNextPosition();
	void			updateTick();
	void			resetTick();

	// delay
    void			setDelaySamples(unsigned int samples);
    unsigned int	getDelaySamples();
	// in 
    void			setInSamples(unsigned int in);
    unsigned int	getInSamples() ;
	// out 
    void			setOutSamples(unsigned int out);
    unsigned int	getOutSamples() ;	
	// length
	void			setLengthSamples(unsigned int l);
	unsigned int	getLengthSamples();
	// pitch
    void			setPitch(float speed);
    float			getPitch() ;
	// loop mode
	void			setLoopMode(int loop);
	void			setPlaying(bool loopMode);
	void			setPlaying(bool loopMode, float speed);
	void			setLoopToStart();
	int				getLoopMode();
	bool			isPlaying() ;
	
	
	// others
    float			getFps();
    void			setFps(float fps);
	float			getVolume(); 
	void			setVolume(float opacity);
	int				getIndex();
	unsigned int	getDelay();
	void			setDeClickLength(int t);
	int				getCrossfadingWidth();
	int				getAudioBufferUnusedSamples();
	
	// event related
	//void			linkToVideoHeader(VideoHeader &vH);
	ofEvent<int>	loopInEvent;
	
	
	
protected:
	
    AudioBufferSamples * aBuffer;
	TimeDiff		oneSample;
	//TimeDiff delay;	
	float			index;
	unsigned int	delay;
	//unsigned int	in;
	//unsigned int	out;
	unsigned int	length;
    float			fps;
	float			pitch;
	float			volume;
	
	
	bool			playing;	
	int				loopMode;
	bool			loopStart;
	int				tickCount;
	
	AudioBufferSamplesMarker	markIn;
	AudioBufferSamplesMarker	markOut;
	AudioSample	crossfade(const AudioSample & sampleA,int mixB,float pct);
	
	// this update will be called when testApp trows update event !
	void			update(ofEventArgs &arg);

	// bools and next values for control to avoid moving markers inside crossfade area
	// indicates if we're crossfading for decliking process
	bool			isCrossfading;
	// we kept the last value of the params to use it when crossfading ends.

	bool			lengthChanged;
	unsigned int	nextLength;
	
	bool			inChanged;
	unsigned int	nextIn;
	
	
	
	};
}
#endif // AUDIOHEADERSAMPLE_H_INCLUDED
