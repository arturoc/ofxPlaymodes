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

class VideoFrame:public plFrame, public ofEventArgs {
public:
    // create a video frame from an ofPixels
	VideoFrame(const ofPixels & videoFrame);
	VideoFrame(){}
	virtual ~VideoFrame();

	void release();

    // returns pixels array
	ofPixels & getPixelsRef();
	ofTexture & getTextureRef();

	int getWidth();
	int getHeight();


private:
    static int total_num_frames;
    ofPixels pixels;
    ofTexture texture;
    bool doRelease;


    // called to update the texture before drawing
	void update(ofEventArgs & args);
};

#endif /* VIDEOFRAME_H_ */
