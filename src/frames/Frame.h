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

namespace ofxPm{
class Frame {
public:
	Frame();
	virtual ~Frame();

	Timestamp getTimestamp();
	virtual operator void*()=0;

protected:
	void refreshTimestamp();

private:
    Timestamp timestamp;
};
}
#endif /* FRAME_H_ */
