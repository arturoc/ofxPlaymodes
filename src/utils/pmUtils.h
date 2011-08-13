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

typedef Poco::Timestamp pmTimestamp;
typedef Poco::Timestamp::TimeDiff pmTimeDiff;

typedef Poco::Mutex ofMutex;

typedef Poco::AbstractDelegate<int> pmIntDelegate;
typedef Poco::AbstractDelegate<float> pmFloatDelegate;



#endif // PMUTILS_H_INCLUDED
