/*
 * BasicVideoRenderer.h
 *
 *  Created on: 11/02/2012
 *      Author: arturo
 */

#ifndef BASICVIDEORENDERER_H_
#define BASICVIDEORENDERER_H_

#include "VideoSink.h"
#include "VideoSource.h"

class BasicVideoRenderer: public VideoSink {
public:
	BasicVideoRenderer();
	BasicVideoRenderer(VideoSource & source);
	virtual ~BasicVideoRenderer();

	void setup(VideoSource & source);

	void draw();

private:
	VideoSource * source;
};

#endif /* BASICVIDEORENDERER_H_ */
