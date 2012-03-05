/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOHEADER_H_INCLUDED
#define VIDEOHEADER_H_INCLUDED

#include "VideoSink.h"
#include "VideoSource.h"
#include "VideoBuffer.h"

// acts as a video source and sink
// controls the position in a buffer
// based on speed, delay, fps and loop in/out

namespace ofxPm{
class VideoHeader:public VideoSink, public VideoSource{
public:
	
    VideoHeader(VideoBuffer & buffer);
    VideoHeader();
    ~VideoHeader();

    void setup(VideoBuffer & buffer);
    void draw();
    VideoFrame *getNextVideoFrame();
	VideoFrame *getVideoFrame(int index);
    int getNextPosition();
    VideoBuffer *getBuffer() const;
	
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
	
	// 
	int		getOpacity() const;
	void	setOpacity(int opacity);
    float	getFps();
    void	setFps(float fps);
	
	// events
	void 	receivedLoopEvent(int &i);

protected:
    VideoBuffer		*buffer;			// pointer to the buffer of video frames

    Timestamp		positionTS;
    TimeDiff		oneFrame;

    float				fps;				// framerate
    float			position;			// position expresses number of frames since start
    int				currentPos;
    float speed;
    int delay;
    float in, out;
	int	opacity;

	//	OF_LOOP_NONE=0x01,
	//	OF_LOOP_PALINDROME=0x02,
	//	OF_LOOP_NORMAL=0x03

    bool	playing;
	int		loopMode;
	bool	loopStart;
};
}
#endif // VIDEOHEADER_H_INCLUDED
