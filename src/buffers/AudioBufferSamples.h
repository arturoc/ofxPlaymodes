
#ifndef AUDIOBUFFERSAMPLES_H_
#define AUDIOBUFFERSAMPLES_H_

#include "ofMain.h"

#include "Buffer.h"
#include "AudioFrame.h"
#include "pmUtils.h"
#include "AudioSink.h"
#include "AudioSource.h"
#include "AudioSample.h"
#include <map>
#include <deque>

namespace ofxPm{
class AudioBufferSamples:public Buffer, public AudioSink, public AudioSource {
public:
	AudioBufferSamples(AudioSource & source,float sizeInSec, int sampleR, int bufferS, int numCh);
	AudioBufferSamples();
	virtual ~AudioBufferSamples();

	void setup(AudioSource & source, float sizeInSec, int sampleR, int bufferS, int numCh);

	AudioSample getAudioSample(int index);
	
	unsigned int		size();                            // total size of the buffer
	unsigned int		getMaxSize();                         // max size of the buffer
	unsigned int		sizeInSamples();                            // total size of the buffer
	unsigned int		getMaxSizeInSamples();                         // max size of the buffer
	unsigned int		getUsedSizeInSamples();                         // real size of the buffer 
	int					getSoundStreamBufferSize();						
	int					getSampleRate();
	int					getNumChannels();
	int					getUnusedSamples();
    virtual void newAudioFrame(AudioFrame &frame);  // for notification of new frame event
    float getFps();                                 // fps of the audio source

	float getRealFPS();                             // aprox calculated fps from source

	void draw();                                    // draws the stats of the buffer

    void stop();                                    // stop receiving new frames
    void resume();                                  // continue receiving new frames
	bool getIsStopped();
protected:
	deque<AudioFrame>		frames;
	
    float					fps;
    AudioSource*			source;

	Timestamp				initTime;

	unsigned int			maxSize;
	unsigned int			maxSizeSamples;
	unsigned int			usedSizeSamples;
	int						unusedSamples;

	bool					stopped;

	int						aSampleRate;
	int						aSoundStreamBufferSize;
	int						aNumCh;
};
}
#endif /* AUDIOBUFFERSAMPLES */
