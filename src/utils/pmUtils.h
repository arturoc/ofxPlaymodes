/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef OFARTUTILS_H_INCLUDED
#define PMUTILS_H_INCLUDED

#include "Poco/Timestamp.h"
#include "Poco/Mutex.h"
#include "Poco/AbstractDelegate.h"
#include "ofMain.h"


namespace ofxPm{
typedef Poco::Timestamp Timestamp;
typedef Poco::Timestamp::TimeDiff TimeDiff;

typedef Poco::AbstractDelegate<int> IntDelegate;
typedef Poco::AbstractDelegate<float> FloatDelegate;
}

#endif // PMUTILS_H_INCLUDED
