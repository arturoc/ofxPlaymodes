#ifndef AUDIOEVENTS_H_INCLUDED
#define AUDIOEVENTS_H_INCLUDED


#include "Poco/FIFOEvent.h"
#include "Poco/Delegate.h"
#include "ofTypes.h"

class AudioFrameListener {

  public:

	virtual void newAudioFrame(AudioFrame & frame){};


};


#endif // AUDIOEVENTS_H_INCLUDED
