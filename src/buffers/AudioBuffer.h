/*
 * AudioBuffer.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "ofMain.h"

#include "Buffer.h"
#include "AudioFrame.h"
#include "pmUtils.h"
#include "AudioSink.h"
#include "AudioSource.h"
#include <map>
#include <deque>

#define AUDIO_BUFFER_NUM_FRAMES 1148

namespace ofxPm{
class AudioBuffer:public Buffer, public AudioSink, public AudioSource {
public:
	AudioBuffer(AudioSource * source);
	virtual ~AudioBuffer();

	AudioFrame * getAudioFrame(int position);       // frame number in the buffer
	AudioFrame * getAudioFrame(TimeDiff microsec);// frame at n microseconds from the end of the buffer
	AudioFrame * getAudioFrame(float pct);          // % of the buffer

	unsigned int size();                            // total size of the buffer


    virtual void newAudioFrame(AudioFrame &frame);  // for notification of new frame event
    float getFps();                                 // fps of the audio source

    Timestamp getLastTimestamp();                 // timestamp for the last frame
	TimeDiff getTotalTime();                      // total time of the buffer in microseconds
	Timestamp getInitTime();                      // time of the first frame in the recording

	long getTotalFrames();                          // total frames since the beginning of the recording
	float getRealFPS();                             // aprox calculated fps from source

	//int currentPos;                                 // currentPos in the buffer

	void draw();                                    // draws the stats of the buffer

    void stop();                                    // stop receiving new frames
    void resume();                                  // continue receiving new frames

protected:
    deque<AudioFrame*> frames;
    float     data[AUDIO_BUFFER_NUM_FRAMES];
    float     fps;
    Timestamp initTime;
    long    totalFrames;
    AudioSource* source;
};
}
#endif /* AUDIOBUFFER_H_ */
