/*
 * Frame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "pmUtils.h"
#include "ofxObjCPointer.h"

class plFrame: public ofxObjCPointer {
public:
	plFrame();
	virtual ~plFrame();

	void * getFrame();
	pmTimestamp getTimestamp();

private:
    pmTimestamp timestamp;
};

#endif /* FRAME_H_ */
