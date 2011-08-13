/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOGRABBER_H_INCLUDED
#define AUDIOGRABBER_H_INCLUDED

#include "AudioSource.h"
#include "ofMain.h"

class AudioGrabber: public AudioSource{
public:
    AudioGrabber();
    virtual ~AudioGrabber();
	void audioReceived(float * input, int bufferSize, int nChannels);
	float getFps();
};

#endif // AUDIOGRABBER_H_INCLUDED
