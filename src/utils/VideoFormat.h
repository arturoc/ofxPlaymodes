/*
 * VideoFormat.h
 *
 *  Created on: 13/02/2012
 *      Author: arturo
 */

#pragma once

namespace ofxPm{
class VideoFormat {
public:
	VideoFormat(int width, int height, int numChannels)
	:width(width)
	,height(height)
	,numChannels(numChannels){}

	VideoFormat(const ofPixels & pixels)
	:width(pixels.getWidth())
	,height(pixels.getHeight())
	,numChannels(pixels.getNumChannels()){}

	int width, height, numChannels;

	bool operator<(const VideoFormat & f) const{
		return f.width * f.height * f.numChannels > width * height * numChannels;
	}
};
}
