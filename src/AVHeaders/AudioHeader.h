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
    AudioHeader(AudioBuffer *buffer);
    ~AudioHeader();

    // draws the state of the header
    void draw();

    TimeDiff delay;


    float getFps();
    void setFps(float fps);

    AudioFrame * getNextFrame();
    AudioFrame * getAudioFrame(int position,float density);


    int getNextPosition();

    float speed;
    float density;

    void setPct(float & pct){
        this->pct=pct;
        pctHasChanged=true;
    }
    void incrPct(float & incr){
        this->pct+=incr;
        pctHasChanged=true;
    }
protected:
    AudioBuffer * buffer;
    float fps;

    //------------------------
    // speed control
    float position;
    int prevBufferPos;
    Timestamp positionTS;
    TimeDiff oneFrame;

    //-----------------------
    // header draw
    int currentPos;
    char msgPos[5];

    //------------------------
    // external control
    bool pctHasChanged;
    float pct;
};
}
#endif // AUDIOHEADER_H_INCLUDED
