/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef BASEENVELOPE_H_INCLUDED
#define BASEENVELOPE_H_INCLUDED

namespace ofxPm{
// base envelope class used to create diferent
// envelopes to be applied to audio grains
class BaseEnvelope{

    // modifies the frame applying the envelope to the audio data
    virtual AudioFrame * applyEnvelope(AudioFrame * frame)=0;

    // % of the beggining of the frame considered as attack
    virtual void setAttack(float pct)=0;

    // % of the end of the frame considered as decay
    virtual void setDecay(float pct)=0;
};
}
#endif // BASEENVELOPE_H_INCLUDED
