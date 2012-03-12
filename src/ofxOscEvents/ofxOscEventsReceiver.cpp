/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */


#include "ofxOscEventsReceiver.h"


namespace ofxPm{
ofxOscEventsReceiver::ofxOscEventsReceiver()
{

}

ofxOscEventsReceiver::~ofxOscEventsReceiver(){

}

void ofxOscEventsReceiver::ProcessMessage( const osc::ReceivedMessage &m, const IpEndpointName& remoteEndpoint )
{
	ofxOscReceiver::ProcessMessage(m,remoteEndpoint);
	
	ofxOscMessage message;
	getNextMessage(&message);
	ofNotifyEvent(newOscMessage,message);
}
}
