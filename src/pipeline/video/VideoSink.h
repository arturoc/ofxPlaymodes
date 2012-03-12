/*
 * VideoSink.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOSINK_H_
#define VIDEOSINK_H_

#include "VideoFrame.h"
#include "VideoSource.h"

namespace ofxPm{
class VideoSink{
public:
	VideoSink();
	virtual ~VideoSink();

	virtual void newVideoFrame(VideoFrame & frame){};
};
}

#endif /* VIDEOSINK_H_ */
