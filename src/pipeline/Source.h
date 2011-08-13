/*
 * Source.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include "Frame.h"
#include "NewFrameListener.h"

class Source {
public:
	Source();
	virtual ~Source();

	plFrame *	getNextFrame();

	//-----------------------------
	// events

	void addNewFrameListener(NewFrameListener * listener);
	void removeNewFrameListener(NewFrameListener * listener);
};

#endif /* SOURCE_H_ */
