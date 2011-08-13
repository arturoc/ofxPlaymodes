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
    VideoHeader(VideoBuffer *buffer);
    ~VideoHeader();

    // draws the state of the header not the frame
    void draw();


    int getFps();
    void setFps(int fps);

    VideoFrame * getNextVideoFrame();
    int getNextPosition();

    float speed;
    int delay;

    void setPct(float & pct){
        this->pct=pct;
        pctHasChanged=true;
    }
    void incrPct(float & incr){
        this->pct+=incr;
        pctHasChanged=true;
    }
    void setIn(float & in){
        this->in=in;
    }
    void setOut(float & out){
        this->out=out;
    }
    void setLoopMode(int & loopMode){
        this->loopMode=loopMode;
    }
    float in, out;
    bool loopMode;

protected:
    VideoBuffer * buffer;
    int fps;

    //------------------------
    // speed control
    float position;
    int prevBufferPos;
    pmTimestamp positionTS;
    pmTimeDiff oneFrame;

    //-----------------------
    // header draw
    char msgPos[5];
    int currentPos;

    //------------------------
    // external control
    bool pctHasChanged;
    float pct;
};

#endif // VIDEOHEADER_H_INCLUDED
