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


namespace ofxPm{
class AudioHeaderSample:public AudioSink, public AudioSource{
public:
    AudioHeaderSample(AudioBufferSamples &buffer);
    AudioHeaderSample();
    ~AudioHeaderSample();

	void			setup(AudioBufferSamples & buffer);
    void			draw();
    AudioSample*	getNextAudioSample();
    AudioSample*	getAudioSample(int _index);
    float			getNextPosition();
	void			updateTick();
	void			resetTick();

	// delay
    void	setDelaySamples(unsigned int samples);
    unsigned int		getDelaySamples();
	// in 
    void	setInSamples(unsigned int in);
    unsigned int		getInSamples() ;
	// out 
    void	setOutSamples(unsigned int out);
    unsigned int		getOutSamples() ;	
	// pitch
    void	setPitch(float speed);
    float	getPitch() ;
	// loop mode
	void	setLoopMode(int loop);
	void	setPlaying(bool loopMode);
	void	setPlaying(bool loopMode, float speed);
	void	setLoopToStart();
	int		getLoopMode();
	bool	isPlaying() ;
	void	setLengthSamples(unsigned int l);
	
	
	// others
    float		getFps();
    void		setFps(float fps);
	float		getVolume(); 
	void		setVolume(float opacity);
	int			getIndex();

	// event related
	void	linkToVideoHeader(VideoHeader &vH);
	ofEvent<int> loopInEvent;
	
protected:
	
    AudioBufferSamples * aBuffer;
	VideoHeader * vHeaderLink;

//    float position;
//    int currentPos;
//
//    Timestamp positionTS;
	TimeDiff oneSample;

	//TimeDiff delay;	
	float		index;
	unsigned int			delay;
	unsigned int			in;
	unsigned int			out;
	unsigned int			length;
    float		fps;
	float		pitch;
	float		volume;
	
	
	bool	playing;	
	int		loopMode;
	bool	loopStart;
	int		tickCount;
	
	// declick
	int		declickLength; 
	

	};
}
#endif // AUDIOHEADERSAMPLE_H_INCLUDED
