/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */


#ifndef VIDEOGRABBER_H_INCLUDED
#define VIDEOGRABBER_H_INCLUDED

#include "VideoSource.h"
#include "ofMain.h"


class VideoGrabber: public VideoSource, public ofVideoGrabber{
public:
    VideoGrabber();
    virtual ~VideoGrabber();

	VideoFrame * getNextVideoFrame();
    int getFps();

    void update();

};


#endif // VIDEOGRABBER_H_INCLUDED
