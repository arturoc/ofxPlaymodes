/*
 * Frame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "Frame.h"

namespace ofxPm{
Frame::Frame() {


}

Frame::~Frame() {
}


Timestamp Frame::getTimestamp(){
    return timestamp;
}

void Frame::refreshTimestamp(){
	timestamp.update();
}
}
