/*
 * VideoBuffer.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

#include "VideoFrame.h"
#include "pmUtils.h"
#include "Buffer.h"
#include "VideoSource.h"
#include "VideoSink.h"
#include "map"
#include "deque"


// a buffer can be connected to any source and stores
// VIDEO_BUFFER_NUM_FRAMES. once the buffer is full
// it begins to delete the first stored frames
// provides different access methods to the frames
// but doesn't do any time control, that is responsability
// of the headers

class VideoBuffer: public Buffer, public VideoSink, public VideoSource {
public:
	VideoBuffer(VideoSource & source, int size);
	VideoBuffer();
	virtual ~VideoBuffer();

	void setup(VideoSource & source, int size);

    // of working in threaded mode,
    // call buffer->lock() buffer->unlock()
    // to call this methods thread sage
	VideoFrame * getVideoFrame(int position);       // frame number in the buffer
	VideoFrame * getVideoFrame(pmTimeDiff time);    // frame at n microseconds from the end of the buffer
	VideoFrame * getVideoFrame(float pct);          // % of the buffer

	VideoFrame * getNextVideoFrame();               // the last video frame in the buffer





	virtual void newVideoFrame(VideoFrame &frame);  // for notification of new frame event

	unsigned int size();                            // total size of the buffer
	unsigned int getMaxSize();                         // max size of the buffer

	int getFps();                                   // fps of the video source

    pmTimestamp getLastTimestamp();                 // timestamp for the last frame
	pmTimeDiff getTotalTime();                      // total time of the buffer in microseconds
	pmTimestamp getInitTime();                      // time of the first frame in the recording

    long getTotalFrames();                          // total frames since the beginning of the recording
	float getRealFPS();                             // aprox fps from source
	//int currentPos;                                 // currentPos in the buffer

	void draw();                                    // draws the stats of the buffer

    void stop();                                    // stop receiving new frames
    void resume();                                  // continue receiving new frames
    bool isStopped();

protected:
    deque<VideoFrame*> frames;
    int     fps;

    long    totalFrames;
    pmTimestamp initTime;

    VideoSource* source;

    bool stopped;
    unsigned int maxSize;
};

#endif /* VIDEOBUFFER_H_ */
