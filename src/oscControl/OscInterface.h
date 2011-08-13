/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef OSCINTERFACE_INCLUDED
#define OSCINTERFACE_INCLUDED

#include "ofMain.h"

#include "ofxOscEventsReceiver.h"

#include <map>
#include "pmUtils.h"
#include "VideoHeader.h"
#include "VideoRenderer.h"
#include "VideoBuffer.h"

#define OSC_INTERFACE_PORT 12345

// allows to map osc messages to variables and setters
class OscInterface{
public:
    OscInterface(vector<VideoHeader*> videoHeaders, vector<VideoRenderer*> videoRenderers, VideoBuffer * videoBuffer);
    void mapMessage(string property,int *value);
    void mapMessage(string property,float *value);
    void mapMessage(string property,pmIntDelegate* setter);
    void mapMessage(string property,pmFloatDelegate* setter);

    void newOscMessage(ofxOscMessage & message);
private:
    map<string,int*> integers;
    map<string,float*> floats;
    map<string,pmIntDelegate* > intSetters;
    map<string,pmFloatDelegate* > floatSetters;
    ofxOscEventsReceiver receiver;
    vector<VideoHeader*> videoHeaders;
    vector<VideoRenderer*> videoRenderers;
    VideoBuffer * videoBuffer;
};


#endif // OSCINTERFACE_INCLUDED
