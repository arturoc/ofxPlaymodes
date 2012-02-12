/*
 * VideoSource.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOSOURCE_H_
#define VIDEOSOURCE_H_

#include "VideoFrame.h"
#include "VideoEvents.h"

class VideoSource{
public:
	VideoSource();
	virtual ~VideoSource();

	virtual VideoFrame * getNextVideoFrame()=0;

	void addListener(VideoFrameListener * listener){
	    ofAddListener(newFrameEvent, listener, &VideoFrameListener::newVideoFrame);
	}
    void removeListener(VideoFrameListener * listener){
	    ofRemoveListener(newFrameEvent, listener, &VideoFrameListener::newVideoFrame);
	}
    virtual int getFps()=0;
protected:
    ofEvent<VideoFrame> newFrameEvent;
};

#endif /* VIDEOSOURCE_H_ */
