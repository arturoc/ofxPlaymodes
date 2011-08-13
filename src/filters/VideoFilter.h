/*
 * VideoFilter.h
 *
 *  Created on: 19-des-2008
 *      Author: eloi
 */

#ifndef VIDEOFILTER_H_
#define VIDEOFILTER_H_

#include "BaseFilter.h"
#include "VideoSource.h"
#include "VideoSink.h"

class VideoFilter : public VideoSink, public VideoSource {
public:

    VideoFilter();
	virtual ~VideoFilter();
    virtual VideoFrame * getNextVideoFrame()=0;
	virtual void newVideoFrame(VideoFrame & frame){};
    virtual int getFps()=0;

};

#endif /* VIDEOFILTER_H_ */
