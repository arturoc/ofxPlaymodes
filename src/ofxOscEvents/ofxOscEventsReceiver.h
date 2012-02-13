/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */


#ifndef _ofxOscEventsRECEIVER_H
#define _ofxOscEventsRECEIVER_H

#include "ofMain.h"

#include "ofxOscReceiver.h"
#include "ofxOscMessage.h"

namespace ofxPm{
class ofxOscEventsReceiver : public ofxOscReceiver
{
public:
	ofxOscEventsReceiver();
	~ofxOscEventsReceiver();

	ofEvent<ofxOscMessage> newOscMessage;
protected:
	/// process an incoming osc message and add it to the queue
	virtual void ProcessMessage( const osc::ReceivedMessage &m, const IpEndpointName& remoteEndpoint );

};
}
#endif
