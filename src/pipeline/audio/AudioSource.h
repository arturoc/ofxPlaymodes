/*
 * AudioSource.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOSOURCE_H_
#define AUDIOSOURCE_H_

#include "AudioFrame.h"
#include "Source.h"
#include "AudioEvents.h"

class AudioSource{
public:
	AudioSource();
	virtual ~AudioSource();

	void addListener(AudioFrameListener * listener){
	    //ofAddListenerMethod(newFrameEvent,listener,&AudioFrameListener::newAudioFrame);
	}
    void removeListener(AudioFrameListener * listener){
	    //ofRemoveListenerMethod(newFrameEvent,listener,&AudioFrameListener::newAudioFrame);
    }
    virtual float getFps()=0;

    ofEvent<AudioFrame> newFrameEvent;
};

#endif /* AUDIOSOURCE_H_ */
