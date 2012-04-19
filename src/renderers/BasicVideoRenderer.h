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
#include "VideoHeader.h"

namespace ofxPm
{
	
class BasicVideoRenderer: public VideoSink {
public:
	BasicVideoRenderer();
	BasicVideoRenderer(VideoSource & source);
	virtual ~BasicVideoRenderer();

	void setup(VideoSource & source);

	void draw();
	void draw(int x,int y,int w,int h);

private:
	VideoSource * source;
};
}
#endif /* BASICVIDEORENDERER_H_ */
