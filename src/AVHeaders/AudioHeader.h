/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOHEADER_H_INCLUDED
#define AUDIOHEADER_H_INCLUDED

#include "AudioSink.h"
#include "AudioSource.h"
#include "AudioBuffer.h"


namespace ofxPm{
class AudioHeader:public AudioSink, public AudioSource{
public:
    AudioHeader(AudioBuffer &buffer);
    AudioHeader();
    ~AudioHeader();

	void setup(AudioBuffer & buffer);
    void draw();
    AudioFrame * getNextAudioFrame();
    AudioFrame * getAudioFrame(int position);
    int getNextPosition();
	

	// delay
    int		getDelayMs() const;
    int		getDelayFrames() const;
    float	getDelayPct() const;
    void	setDelayMs(int delay);
    void	setDelayFrames(int delay);
    void	setDelayPct(float pct);
	// in 
    float	getIn() const;
    void	setInMs(int in);
    void	setInFrames(int in);
    void	setInPct(float in);
	// out 
    float	getOut() const;	
    void	setOutMs(int out);
    void	setOutFrames(int out);
    void	setOutPct(float out);
	// speed
    float	getSpeed() const;
    void	setSpeed(float speed);
	// loop mode
	int		getLoopMode();
	void	setLoopMode(int loop);
	bool	isPlaying() const;
	void	setPlaying(bool loopMode);
	void	setPlaying(bool loopMode, float speed);
	void	setLoopToStart();
	
	// others
    float	getFps();
    void	setFps(float fps);
	
protected:
	
    AudioBuffer * buffer;

    float position;
    int currentPos;

    Timestamp positionTS;
	TimeDiff oneFrame;

	//TimeDiff delay;	
	int	delay;
	float in;
	float out;
    float fps;
	float speed;
	
	bool	playing;	
	int		loopMode;
	bool	loopStart;

	
//    //------------------------
//    // external control
//    bool pctHasChanged;
//    float pct;
};
}
#endif // AUDIOHEADER_H_INCLUDED
