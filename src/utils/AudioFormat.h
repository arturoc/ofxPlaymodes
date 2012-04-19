/*
 * AudioFormat.h
 *
 *  Created on: 19/04/2012
 *      Author: arturo
 */


#pragma once

namespace ofxPm{
class AudioFormat {
public:
	AudioFormat(int bufferSize, int numChannels)
	:bufferSize(bufferSize)
	,numChannels(numChannels){}


	int bufferSize, numChannels;

	bool operator<(const AudioFormat & f) const{
		return f.bufferSize * f.numChannels > bufferSize * numChannels;
	}
};
}
