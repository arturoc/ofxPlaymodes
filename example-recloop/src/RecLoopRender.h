#ifndef RECLOOPRENDER_H
#define RECLOOPRENDER_H

#include <VideoSink.h>
#include <VideoSource.h>


class RecLoopRender : public ofxPm::VideoSink
{
public:
	RecLoopRender();
	RecLoopRender(ofxPm::VideoSource & bufferSource, ofxPm::VideoSource & liveSource);
	virtual ~RecLoopRender();

	void setup(ofxPm::VideoSource & bufferSource, ofxPm::VideoSource & liveSource);

	void draw();

	void stop();
	void resume();

	int alpha;
	int tintR, tintG, tintB;
	bool minmaxBlend;
protected:
private:
	ofxPm::VideoSource * buffer;
	ofxPm::VideoSource * live;
	ofImage       image;
	bool          imageAllocated;
	bool		  stopped;
};

#endif // RECLOOPRENDER_H
