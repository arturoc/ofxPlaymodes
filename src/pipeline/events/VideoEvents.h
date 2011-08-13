#ifndef VIDEOEVENTS_H_INCLUDED
#define VIDEOEVENTS_H_INCLUDED


#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"
#include "ofTypes.h"

class VideoFrameListener {

  public:

	virtual void newVideoFrame(VideoFrame & frame){};



};





#endif // VIDEOEVENTS_H_INCLUDED
