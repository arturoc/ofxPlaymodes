/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "AudioHeader.h"

//------------------------------------------------------

namespace ofxPm
{
	
	AudioHeader::AudioHeader(AudioBuffer &buffer)
	{
		this->aBuffer=&buffer;
		fps=buffer.getFps();
		position=0;
		oneFrame=0;
		speed=1;
		currentPos=0;
		in = 1.0;
		out = 0.0;
		playing    = false;
		loopStart	= false;
		loopMode	= OF_LOOP_NORMAL;
		delay       = 0;
		this->vHeaderLink = NULL;

	}

	
	//------------------------------------------------------

	AudioHeader::AudioHeader()
	{
		position=0;
		oneFrame=0;
		speed=1;
		currentPos=0;
		in = 1.0;
		out = 0.0;
		playing    = false;
		loopStart	= false;
		loopMode	= OF_LOOP_NORMAL;
		this->vHeaderLink = NULL;
	}
		
	//------------------------------------------------------
		
	 AudioHeader::~AudioHeader()
	{
		

	}

	//------------------------------------------------------
		
	void AudioHeader::setup(AudioBuffer & buffer)
	{
		
		this->aBuffer= &buffer;
		fps=buffer.getFps();
		position=0;
		oneFrame=0;
		speed=1;
		currentPos=0;
		in = 1.0;
		out = 0.0;
		playing    = false;
		loopStart	= false;
		loopMode	= OF_LOOP_NORMAL;
		this->vHeaderLink = NULL;
		
	}

	//------------------------------------------------------
		
	void AudioHeader::draw()
	{
		float currentLength=float(currentPos)/((float)this->aBuffer->getMaxSize())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(aBuffer->getMaxSize());

		ofSetColor(0,255,255);

		ofPushStyle();
		ofSetLineWidth(3.0);
		ofLine(currentLength+PMDRAWSPACING,610,currentLength+PMDRAWSPACING,670);
		ofPopStyle();

		ofDrawBitmapString(ofToString(currentPos),currentLength,615);
		
		int	inFrame  = int(float(aBuffer->size()-1)*(in));
		int outFrame = int(float(aBuffer->size()-1)*(out));
		int inPos = PMDRAWSPACING + ((aBuffer->size()-1-inFrame) * oneLength) + oneLength/2;
		int outPos = PMDRAWSPACING + ((aBuffer->size()-1-outFrame) * oneLength) + oneLength/2;
		
		// draw in & out lines
		int audioBuffDrawPos = 90;
		ofSetLineWidth(1.0);
		ofLine(inPos,PMDRAWELEMENTSY+10-audioBuffDrawPos,inPos,PMDRAWELEMENTSY+60-audioBuffDrawPos);
		ofLine(outPos,PMDRAWELEMENTSY+10-audioBuffDrawPos,outPos,PMDRAWELEMENTSY+60-audioBuffDrawPos);
		ofLine(inPos,PMDRAWELEMENTSY+60-audioBuffDrawPos,outPos,PMDRAWELEMENTSY+60-audioBuffDrawPos);
		// draw inPos triangle
		ofBeginShape();
		ofVertex(inPos,PMDRAWELEMENTSY+10-audioBuffDrawPos);
		ofVertex(inPos+5,PMDRAWELEMENTSY+5-audioBuffDrawPos);
		ofVertex(inPos,PMDRAWELEMENTSY-audioBuffDrawPos);
		ofEndShape();
		// draw outPos triangle
		ofBeginShape();
		ofVertex(outPos,PMDRAWELEMENTSY+10-audioBuffDrawPos);
		ofVertex(outPos-5,PMDRAWELEMENTSY+5-audioBuffDrawPos);
		ofVertex(outPos,PMDRAWELEMENTSY-audioBuffDrawPos);
		ofEndShape();
		
		ofDrawBitmapString("[ " + ofToString(inFrame) ,ofPoint(inPos+0,PMDRAWELEMENTSY+75-audioBuffDrawPos));
		ofDrawBitmapString(ofToString(outFrame) + " ]" ,ofPoint(outPos-30,PMDRAWELEMENTSY+75-audioBuffDrawPos));
//		
//	
//		ofCircle(inPos,650,10);
//		ofCircle(outPos,650,10);
	}

	//------------------------------------------------------

		
	float AudioHeader::getFps()
	{
		return fps;
	}

	//------------------------------------------------------


	void AudioHeader::setFps(float fps)
	{
		this->fps=fps;
	}

	//------------------------------------------------------

	AudioFrame * AudioHeader::getNextAudioFrame()
	{
		aBuffer->lock();
			currentPos=getNextPosition();
			AudioFrame * frame = aBuffer->getAudioFrame(currentPos);
		aBuffer->unlock();
		
		return frame;
	}

	//------------------------------------------------------

	AudioFrame * AudioHeader::getAudioFrame(int position)
	{
		position = CLAMP(position,0,aBuffer->size());
		AudioFrame * currentFrame = aBuffer->getAudioFrame(position);

		int currentFrameSize=currentFrame->getBufferSize()*currentFrame->getChannels();
		float resultBuffer[currentFrameSize];
		memcpy(resultBuffer,currentFrame->getAudioData(),sizeof(float)*currentFrameSize);
				
		AudioFrame * resultFrame=
        new AudioFrame( resultBuffer,
					   currentFrame->getBufferSize(),
					   currentFrame->getChannels());
		currentFrame->release();
		return resultFrame;
	}

	//------------------------------------------------------
	// returns the real position in the buffer

	int AudioHeader::getNextPosition()
	{
		if(playing) oneFrame=(TimeDiff)(1000000.0/fps/speed);
		else oneFrame=(TimeDiff)(1000000.0/fps/1.0);
		
		unsigned int buffer_size=aBuffer->size();
		unsigned int totalNumFr = aBuffer->getTotalFrames();
		unsigned int lastAbsFrame = totalNumFr - buffer_size; 
		int	inFrame  = int(float(buffer_size-1)*(in));
		int outFrame = int(float(buffer_size-1)*(out));
		int	inAbsFrame  = totalNumFr -  inFrame;
		int	outAbsFrame = totalNumFr - outFrame;
		
		
//		 printf("-------------------------------\nTOTAL : %d\nSIZE : %d \nLAST : %d\n IN : %d / %d \n OUT : %d / %d \nPOSITION : %f %d\n",
//		 totalNumFr,
//		 buffer_size,
//		 lastAbsFrame,
//		 inAbsFrame,totalNumFr-inAbsFrame,
//		 outAbsFrame,totalNumFr-outAbsFrame,
//		 position, int(position));
		 	
		
		
		if((float)positionTS.elapsed()+(position-floor(position))*(float)abs(oneFrame)>=abs(oneFrame))
		{
			if(oneFrame!=0)
			{
				position=position + (float)positionTS.elapsed()/(float)oneFrame;

			}
			// updates the time-stamp with the current time
			positionTS.update();
		}
//		printf("new Position %f !!!!!!! %f > %f\n ",position,position,float(outAbsFrame));
		// if header is playing and loopStart is requested, set position to inPoint or outPoint depending on speed's sign !
		if(playing && loopStart)
		{
			if(speed>0.0) position=float(inAbsFrame);
			else position=float(outAbsFrame);
			loopStart=false;
		}
		
		// if we're playing in loop and we're reaching the outpoint
		if(playing && (int(position) > (outAbsFrame)))
		{
			if(loopMode==OF_LOOP_NORMAL) 
			{
				position = float(inAbsFrame);
				// throw event to the videoHeader to say : "hi ! go back to loop start !"
				if(vHeaderLink!=NULL) 
				{
					int i = 1;
					ofNotifyEvent(loopInEvent,i,vHeaderLink);
				}
				
			}
			else if (loopMode==OF_LOOP_NONE)
			{
				setPlaying(false);
			}
			else if (loopMode==OF_LOOP_PALINDROME) 
			{
				speed=-speed;
			}
		}
		// if we're in playing in loop and we're reaching the inpoint (while speed is negative probably)
		else if(playing && (int(position) < (inAbsFrame)))
		{
			if(loopMode==OF_LOOP_NORMAL)
			{ 
				position = float(outAbsFrame);
				// throw event to the videoHeader to say : "hi ! go back to loop start !"
				if(vHeaderLink!=NULL) 
				{
					int i = 1;
					ofNotifyEvent(loopInEvent,i,vHeaderLink);
				}				
			}
			else if (loopMode==OF_LOOP_NONE) setPlaying(false);
			else if (loopMode==OF_LOOP_PALINDROME) 
			{
				speed=-speed;
			}
		}
		
		// clamp position to it's limits ...
		if(playing) position=CLAMP(position,float(inAbsFrame),float(outAbsFrame));
		else position=CLAMP(position,float(lastAbsFrame),float(totalNumFr));
		
		// backpos
		int backpos=0;	
		if (!playing) backpos=0;
		else {
			backpos=int(aBuffer->getTotalFrames()-int(position));
			backpos=CLAMP(backpos,0,buffer_size-1);
		}
		
		int nextPos;
		if (playing) nextPos= (buffer_size-1) - backpos;
		else		 nextPos= (buffer_size-1) - (delay/oneFrame);
		
		nextPos = CLAMP(nextPos,0,buffer_size-1);
		if(nextPos<0) nextPos=0;
		
		return nextPos;

	}
	
	//------------------------------------------------------
	void AudioHeader::setSpeed(float speed)
	{
		this->speed = speed;
	}
	//------------------------------------------------------
	float AudioHeader::getSpeed() 
	{
		return speed;
	}	
	
	//------------------------------------------------------
	// get & set delay 
	//------------------------------------------------------
	int AudioHeader::getDelayMs() 
	{
		return delay/1000;
	}
	//------------------------------------------------------
	int AudioHeader::getDelayFrames() 
	{
		return this->getDelayMs()/(TimeDiff)(1000.0/fps/1.0);
	}
	//------------------------------------------------------
	float AudioHeader::getDelayPct() 
	{
		float res = this->getDelayFrames()/(aBuffer->size()-1); 
		return res;
	}
	//------------------------------------------------------
	void AudioHeader::setDelayMs(float delayMs)
	{
		TimeDiff oneFrame=(TimeDiff)(1000000.0/fps/1.0);
		float fAux = delayMs*1000.0f;
		this->delay = CLAMP(int(fAux),0,int((aBuffer->getMaxSize()-1)*oneFrame));
		// ? eloi hack
		//this->position = 0;
	}
	//------------------------------------------------------
	void AudioHeader::setDelayFrames(int delayFrames)
	{
		this->setDelayMs(int((float(delayFrames) / float(this->getFps()))*1000.0));
	}
	//------------------------------------------------------
	void AudioHeader::setDelayPct(float pct)
	{
		this->setDelayFrames(int(float(pct) * float(aBuffer->size())));
	}
	
	
	//------------------------------------------------------
	// get & set in & out
	//------------------------------------------------------
	float AudioHeader::getIn() 
	{
		return in;
	}
	//------------------------------------------------------
	void AudioHeader::setInMs(float inMs)
	{
		TimeDiff oneFrameMs=(TimeDiff)(1000000.0/fps/1.0);
		float fAux = float(inMs*1000.0f) / (oneFrameMs*float(aBuffer->size()));
		this->setInPct(CLAMP(fAux,0.0,1.0));    
		
	}
	//------------------------------------------------------
	void AudioHeader::setInPct(float in)
	{
		this->in=CLAMP(in,0.0,1.0);
	}
	//------------------------------------------------------
	void AudioHeader::setInFrames(int in)
	{
		float pct = float(in)/float(aBuffer->size());
		this->setInPct(pct);
	}
	
	//------------------------------------------------------
	float AudioHeader::getOut() 
	{
		return out;
	}
	//------------------------------------------------------
	void AudioHeader::setOutMs(float outMs)
	{
		TimeDiff oneFrameMs=(TimeDiff)(1000000.0/fps/1.0);
		float fAux = float(outMs*1000.0f) / (oneFrameMs*float(aBuffer->size()));
		this->setOutPct(CLAMP(fAux,0.0,1.0));    
	}
	//------------------------------------------------------
	void AudioHeader::setOutPct(float out)
	{
		this->out=CLAMP(out,0.0f,this->in);
	}
	//------------------------------------------------------
	void AudioHeader::setOutFrames(int out)
	{
		float pct = float(out)/float(aBuffer->size());
		this->setOutPct(pct);
	}
	
	//------------------------------------------------------
	// get & set loop & playing
	//------------------------------------------------------
	int AudioHeader::getLoopMode()
	{
		return loopMode;
	}
	//------------------------------------------------------
	void AudioHeader::setLoopMode(int loop)
	{
		if((loopMode!=OF_LOOP_NORMAL) || (loopMode!=OF_LOOP_NONE) || (loopMode!=OF_LOOP_PALINDROME))
		{
			printf("ofxPlaymodes::VideoHeader:: Incorrect loop mode!\n");
		}
		else loopMode = loop;
	}
	
	//------------------------------------------------------
	void AudioHeader::setLoopToStart()
	{
		loopStart=true;
	}
	//------------------------------------------------------
	bool AudioHeader::isPlaying() 
	{
		return playing;
	}
	//------------------------------------------------------
	void AudioHeader::setPlaying(bool isPlaying, float speed)
	{
		this->speed = speed;
		if(isPlaying)
		{
			// if we're entering loop mode move position to in point
			// this behaviour is to sync entering loop mode with starting at inPoint or outPoint depending on speed
			this->playing = isPlaying;
			int	loopFrame;
			if(speed>0.0f) loopFrame = int(float(aBuffer->size()-1)*(in));
			else loopFrame = int(float(aBuffer->size()-1)*(out));
			int	inAbsFrame  = aBuffer->getTotalFrames() -  loopFrame;
			position = inAbsFrame; 
		}
		else
		{
			// if we're gettint out of loop mode move delay to actual position
			// this behaviour is to let the header (set by delay on no loop) where the loop was when deactivated
			// other beahaviour could be to let the header on delay / inPoint / outPoint position when loop is turned off
			this->playing = isPlaying;
			float	actualFrame  = float(aBuffer->getTotalFrames()-1) - (position);
			TimeDiff oneFrame=(TimeDiff)(1000000.0/fps/1.0);		
			delay = (actualFrame-1)*oneFrame;
		}
	}		
	//------------------------------------------------------
	void AudioHeader::setPlaying(bool isPlaying)
	{
		this->setPlaying(isPlaying,1.0);	
	}
	//------------------------------------------------------
	void AudioHeader::linkToVideoHeader(VideoHeader &vH)
	{
		this->vHeaderLink=&vH;
		ofAddListener(this->loopInEvent,this->vHeaderLink,&VideoHeader::receivedLoopEvent);
	}
	
	//------------------------------------------------------
	void AudioHeader::setVolume(float v) 
	{
		this->volume = CLAMP (v,0.0f,1.0f);
	}
	//------------------------------------------------------
	float AudioHeader::getVolume() 
	{
		return volume;
	}
	
}
