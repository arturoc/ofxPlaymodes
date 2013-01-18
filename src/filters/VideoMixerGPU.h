/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef VIDEOMIXERGPU_H_
#define VIDEOMIXERGPU_H_

#include "VideoFilter.h"

namespace ofxPm{
class VideoMixerGPU: public VideoFilter {
public:
	VideoMixerGPU();
	virtual ~VideoMixerGPU();

	void setup(VideoSource & source1, VideoSource & source2);

    VideoFrame getNextVideoFrame();
	void newVideoFrame(VideoFrame & frame);
	float getFps();

private:
	void glThreadUpdate(ofEventArgs & args);
	VideoFrame front, back, back1, back2;
	VideoSource * source1, * source2;
	ofShader shader;
	bool newFrame;
	ofMutex swapBuffersMutex;
};
}

#endif /* VIDEOMIXERGPU_H_ */
