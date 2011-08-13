/*
 * NewFrameListener.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef NEWFRAMELISTENER_H_
#define NEWFRAMELISTENER_H_

#include "Frame.h"

class NewFrameListener {
public:
	NewFrameListener();
	virtual ~NewFrameListener();

	virtual void newFrame(plFrame * frame){};
};

#endif /* NEWFRAMELISTENER_H_ */
