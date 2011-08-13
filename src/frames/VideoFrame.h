/*
 * VideoFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOFRAME_H_
#define VIDEOFRAME_H_

#include "Frame.h"
#include "ofMain.h"

class VideoFrame:public plFrame, public ofEventArgs {
public:
    // create a video frame from a pixels array and with height parameters
	VideoFrame(unsigned char * videoFrame, int w, int h);
	VideoFrame(){}
	virtual ~VideoFrame();

    // called to update the texture before drawing
	void update(ofEventArgs & args);
	void release();

    // returns pixels array
	unsigned char * getVideoFrame();
	ofTexture * getTexture();

    int w;
    int h;

    bool isTexAllocated();
    void setTexAllocated(bool isAllocated);


private:
    unsigned char * pixels;
    static int total_num_frames;
    ofTexture * texture;
    bool texAllocated;
    bool doRelease;
};

#endif /* VIDEOFRAME_H_ */
