

#ifndef HEADERLOOPERRENDERER_H_
#define HEADERLOOPERRENDERER_H_

#include "VideoSink.h"
#include "VideoSource.h"
#include "VideoHeader.h"

namespace ofxPm
{

//------------------------------------------------------
class HeaderLooperRenderer: public VideoSink {
public:
	HeaderLooperRenderer();
	HeaderLooperRenderer(VideoBuffer & buffer);
	virtual ~HeaderLooperRenderer();

	void setup(VideoBuffer & buffer);
	VideoHeader* getHeader();
	
	void draw();
	void draw(int x,int y,int w,int h);

private:
	VideoBuffer*		vBuffer;
	VideoHeader			vHeader;
	//VideoSource*		source;
};
//------------------------------------------------------

}
#endif /* HEADERLOOPERRENDERER_H_ */

