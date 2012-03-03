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

namespace ofxPm{
class AudioGrabber: public AudioSource, public ofBaseSoundInput{
public:
    AudioGrabber();
    virtual ~AudioGrabber();
	void audioReceived(float * input, int bufferSize, int nChannels);

	float	getFps();
	void	setFps(float f);
	
	float fps;
};
}
#endif // AUDIOGRABBER_H_INCLUDED
