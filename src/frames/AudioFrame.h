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
#include "ofEvents.h"
#include "ofTypes.h"

namespace ofxPm{
class AudioFrame:public ofxPm::Frame, public ofEventArgs {

public:
	static AudioFrame newAudioFrame(const float * audioFrame,const int bufferSize,const int channels);
	AudioFrame();
	virtual ~AudioFrame();
	vector<float> & getAudioData();
	
	int         getBufferSize();
	int         getChannels();
	float       getAverageValue();

	
private:
	class Obj;
	ofPtr<Obj> data;
};
}
#endif /* AUDIOFRAME_H_ */
