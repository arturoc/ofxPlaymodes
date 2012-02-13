/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef RAISEDCOSINEBELLENVELOPE_H_INCLUDED
#define RAISEDCOSINEBELLENVELOPE_H_INCLUDED

#include "BaseEnvelope.h"

namespace ofxPm{
class RaisedCosineBellEnvelope:public BaseEnvelope{
public:
    RaisedCosineBellEnvelope(){
        attackPct   = 0.1;
        decayPct    = 0.1;
    }

    AudioFrame * applyEnvelope(AudioFrame * frame){
        frame->retain();
        int grainSize = frame->getBufferSize();
        int attackSamples = grainSize * attackPct;
        int releaseSamples  = grainSize * decayPct;

        float *source = frame->getAudioFrame();
        float *dst = new float[grainSize*frame->getChannels()];

        for(int i=0;i<attackSamples;i++){
            dst[i*2]=(1.0 + cos( PI + ( PI * ( i / attackSamples ) )))*
                                 (source[i*2] / 2.0);
            dst[i*2+1]=(1.0 + cos( PI + ( PI * ( i / attackSamples ) )))*
                                 (source[i*2+1] / 2.0);
        }
        for(int i=attackSamples;i<grainSize-releaseSamples;i++){
            dst[i*2]=source[i*2];
            dst[i*2+1]=source[i*2+1];
        }
        for(int i=grainSize-releaseSamples;i<grainSize;i++){
            dst[i*2]=(1.0 + cos( PI * ( i / releaseSamples ) ) )*
                                             (source[i*2] / 2.0);
            dst[i*2+1]=(1.0 + cos( PI * ( i / releaseSamples ) ) )*
                                             (source[i*2+1] / 2.0);
        }
        frame->release();
        AudioFrame * dstFrame = new AudioFrame(dst,grainSize,frame->getChannels());
        return dstFrame;
    }
    void setAttack(float pct){
        attackPct=pct;
    }
    void setDecay(float pct){
        decayPct=pct;
    }

    float attackPct;
    float decayPct;
};
}
#endif // RAISEDCOSINEBELLENVELOPE_H_INCLUDED
