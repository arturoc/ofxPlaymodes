
#ifndef AUDIOBUFFER2_H_
#define AUDIOBUFFER2_H_

#include "ofMain.h"

#include "Buffer.h"
#include "AudioFrame.h"
#include "pmUtils.h"
#include "AudioSink.h"
#include "AudioSource.h"
#include "AudioSample.h"
#include "pmUtils.h"
#include <map>
#include <deque>

namespace ofxPm{
class AudioBuffer:public Buffer, public AudioSink, public AudioSource {
public:
	AudioBuffer(AudioSource & source,float sizeInSec, int sampleR, int bufferS, int numCh);
	AudioBuffer();
	virtual ~AudioBuffer();

	void setup(AudioSource & source, float sizeInSec, int sampleR, int bufferS, int numCh);

//	AudioFrame * getAudioFrame(int position);       // frame number in the buffer
//	AudioFrame * getAudioFrame(TimeDiff microsec);// frame at n microseconds from the end of the buffer
//	AudioFrame * getAudioFrame(float pct);          // % of the buffer
//	AudioSample* getAudioSample(int index);
	
	unsigned int	size();                            // total size of the buffer
	unsigned int	getMaxSize();                         // max size of the buffer
	unsigned int	sizeInSamples();                            // total size of the buffer
	unsigned int	getMaxSizeInSamples();                         // max size of the buffer
	int				getSoundStreamBufferSize();
	int				getAudioSampleRate();
    virtual void newAudioFrame(AudioFrame &frame);  // for notification of new frame event
    float getFps();                                 // fps of the audio source

    Timestamp getLastTimestamp();                 // timestamp for the last frame
	TimeDiff getTotalTime();                      // total time of the buffer in microseconds
	Timestamp getInitTime();                      // time of the first frame in the recording

	long getTotalFrames();                          // total frames since the beginning of the recording
	float getRealFPS();                             // aprox calculated fps from source

	//int currentPos;                                 // currentPos in the buffer

	void draw();                                    // draws the stats of the buffer

    void stop();                                    // stop receiving new frames
    void resume();                                  // continue receiving new frames

protected:
    deque<AudioFrame*>		frames;
	
    //float					data[AUDIO_BUFFER_NUM_FRAMES];
    float					fps;
    Timestamp				initTime;
    long					totalFrames;
    AudioSource*			source;
	unsigned int			maxSize;
	unsigned int			maxSizeSamples;
	bool					stopped;

	int						aSampleRate;
	int						aSoundStreamBufferSize;
	int						aNumCh;
};
}
#endif /* AUDIOBUFFER_H_ */
