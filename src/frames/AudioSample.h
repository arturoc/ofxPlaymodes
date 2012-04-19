/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOSAMPLE_H_
#define AUDIOSAMPLE_H_

#include "Frame.h"
#include "pmUtils.h"
#include "ofMain.h"
#include "ofEvents.h"

namespace ofxPm{
class AudioSample {

public:
	AudioSample(float * audioSample, int channels);
	AudioSample(){};
	
	float *			getAudioData();
	void			setAudioData(int _channel, float _value);
	int				getChannels();
	
	
private:
    float * data;
	int channels;
};
}
#endif /* AUDIOSAMPLE_H_ */
