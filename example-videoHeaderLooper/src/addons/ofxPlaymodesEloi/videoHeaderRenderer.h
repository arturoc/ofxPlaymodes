
#ifndef VIDEOHEADERRENDERER_H_
#define VIDEOHEADERRENDERER_H_

#include "VideoSink.h"
#include "VideoSource.h"
#include "VideoHeader.h"
#include "VideoBuffer.h"

using namespace ofxPm;

class videoHeaderRenderer {
public:
	videoHeaderRenderer();
	videoHeaderRenderer(VideoBuffer & _buffer);
	virtual ~videoHeaderRenderer();

	void setup(VideoBuffer & _buffer);
	VideoHeader getHeader();

	void draw();
	void draw(int x,int y,int w,int h);

private:
	VideoBuffer*	vBuffer;
	VideoHeader		vHeader;
	
};
#endif 