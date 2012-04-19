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
class AudioSample:public ofxPm::Frame, public ofEventArgs {

public:
	AudioSample(float * audioSample, int channels);
	AudioSample(){};
	virtual ~AudioSample();
	
	float *			getAudioData();
	void			setAudioData(int _channel, float _value);
	void			release();
	void			retain();
	int				getChannels();
	
    static int		numInstances;
	
private:
    float * data;
	int channels;
};
}
#endif /* AUDIOSAMPLE_H_ */
