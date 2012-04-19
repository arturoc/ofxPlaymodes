/*
 * VideoFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOFRAME_H_
#define VIDEOFRAME_H_

#include "Frame.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofEvents.h"
#include "VideoFormat.h"
#include <map>

namespace ofxPm{
class VideoFrame: public ofxObjCPointer, public ofxPm::Frame, public ofEventArgs {
    // create a video frame from an ofPixels
	VideoFrame(const ofPixels & videoFrame);
	VideoFrame(){}
public:
	static VideoFrame * newVideoFrame(const ofPixels & videoFrame);
	virtual ~VideoFrame();

	void release();

    // returns pixels array
	ofPixels & getPixelsRef();
	ofTexture & getTextureRef();

	int getWidth();
	int getHeight();

	static int getPoolSize(const VideoFormat & format);

private:
    static int total_num_frames;
    static map<VideoFormat,vector<VideoFrame *> > pool;
    static ofMutex poolMutex;

    ofPixels pixels;
    ofTexture texture;
    bool pixelsChanged;
};
}
#endif /* VIDEOFRAME_H_ */
