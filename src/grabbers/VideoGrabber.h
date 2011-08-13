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


// the video grabber is by now implemented using ofxAdvVideo
// until there's a common base for video players/grabbers in of
class VideoGrabber: public VideoSource, public ofVideoGrabber{
public:
    VideoGrabber();
    virtual ~VideoGrabber();

	VideoFrame * getNextVideoFrame();
    int getFps();

    void update();

};


#endif // VIDEOGRABBER_H_INCLUDED
