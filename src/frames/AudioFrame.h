/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIOFRAME_H_
#define AUDIOFRAME_H_

#include "Frame.h"
#include "pmUtils.h"
#include "ofMain.h"

class AudioFrame:public plFrame {
public:
	AudioFrame(float * audioFrame, int bufferSize, int channels);
	AudioFrame(){};
	virtual ~AudioFrame();
	float * getAudioFrame();

	int         getBufferSize();
	int         getChannels();
	float       getAverageValue();

    static int numInstances;
private:
    float * data;
	int bufferSize;
	int channels;
	float averageValue;
};

#endif /* AUDIOFRAME_H_ */
