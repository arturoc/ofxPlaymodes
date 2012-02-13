/*
 * Buffer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "Buffer.h"

namespace ofxPm{
Buffer::Buffer() {

}

Buffer::~Buffer() {
}

void Buffer::lock(){
    timeMutex.lock();
}

void Buffer::unlock(){
    timeMutex.unlock();
}

}
