/*
 * FileGrabber.h
 *
 *  Created on: 01/11/2009
 *      Author: arturo castro
 */

#ifndef FILEGRABBER_H_
#define FILEGRABBER_H_

#include "VideoSource.h"
#include "ofMain.h"


class FileGrabber: public VideoSource, public ofVideoPlayer {
public:
	FileGrabber();
	virtual ~FileGrabber();

	VideoFrame * getNextVideoFrame();
    int getFps();

    void update();

};

#endif /* FILEGRABBER_H_ */
