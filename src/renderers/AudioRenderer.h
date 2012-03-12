/*
 * AudioRenderer.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef AUDIORENDERER_H_
#define AUDIORENDERER_H_

#include "AudioSink.h"

namespace ofxPm{
class AudioRenderer: public AudioSink {
public:
	AudioRenderer();
	virtual ~AudioRenderer();
};
}
#endif /* AUDIORENDERER_H_ */
