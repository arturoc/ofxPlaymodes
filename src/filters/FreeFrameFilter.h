/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

// for FreeFrame 1.0 plugin effect

#ifndef FREEFRAMEFILTER_H_
#define FREEFRAMEFILTER_H_

#include "VideoFilter.h"
#include "VideoSource.h"
#include "ofFreeframe.h"

class FreeFrameFilter : public VideoFilter {
public:
    ///
    FreeFrameFilter(){};
    FreeFrameFilter(VideoSource * source,int w,int h);
	virtual ~FreeFrameFilter();
    ///
    VideoFrame * getNextVideoFrame();
    int getFps();
    void newVideoFrame(VideoFrame &frame);
    ///
    void loadPlugin(char * filename);
    void setFFParameter(int index,float value);

    ofFreeframe* ff;

private:
    ///
    VideoSource * source;
    VideoFrame * currentFrame;
    ///
    int w,h;



};

#endif /* FREEFRAMEFILTER_H_ */
