/*
 * AudioSource.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOSOURCE_H_
#define AUDIOSOURCE_H_

#include "AudioFrame.h"

namespace ofxPm{
class AudioSource{
public:
	AudioSource();
	virtual ~AudioSource();

    virtual float getFps()=0;

    ofEvent<AudioFrame> newFrameEvent;
};
}
#endif /* AUDIOSOURCE_H_ */
