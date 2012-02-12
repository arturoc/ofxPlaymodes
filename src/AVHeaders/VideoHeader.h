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

class VideoHeader:public VideoSink, public VideoSource{
public:
    VideoHeader(VideoBuffer & buffer);
    VideoHeader();
    ~VideoHeader();

    void setup(VideoBuffer & buffer);
    void draw();

    int getFps();
    void setFps(int fps);

    VideoFrame *getNextVideoFrame();
    int getNextPosition();

    VideoBuffer *getBuffer() const;
    int getDelay() const;
    float getIn() const;
    float getPct() const;
    float getSpeed() const;
    bool isLoopMode() const;

    void setDelay(int delay);
    void setIn(float in);
    void setOut(float out);
    void setLoopMode(bool loopMode);
    void setPct(float pct);
    void setSpeed(float speed);
protected:
    VideoBuffer *buffer;
    int fps;
    float position;
    int prevBufferPos;
    pmTimestamp positionTS;
    pmTimeDiff oneFrame;
    char msgPos[5];
    int currentPos;
    bool pctHasChanged;
    float pct;


    float speed;
    int delay;
    float in, out;
    bool loopMode;
};

#endif // VIDEOHEADER_H_INCLUDED
