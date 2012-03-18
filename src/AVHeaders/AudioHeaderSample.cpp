/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "AudioHeaderSample.h"

//------------------------------------------------------

namespace ofxPm
{
	
	AudioHeaderSample::AudioHeaderSample(AudioBufferSamples &buffer)
	{
		this->setup(buffer);
	}
	
	
	//------------------------------------------------------
	
	AudioHeaderSample::AudioHeaderSample()
	{
		index	= 0;
		delay   = 0;
		in		= 0;
		out		= 0;
		length	= 0;
		pitch	= 1.0;
		
		playing		= false;
		loopStart	= false;
		loopMode	= OF_LOOP_NORMAL;
		volume		= 1.0f;
		this->vHeaderLink = NULL;
	}
	
	//------------------------------------------------------
	
	AudioHeaderSample::~AudioHeaderSample()
	{
		
		
	}
	
	//------------------------------------------------------
	
	void AudioHeaderSample::setup(AudioBufferSamples & buffer)
	{
		
		this->aBuffer=&buffer;
		fps=aBuffer->getFps();
		
		index	= 0;
		delay   = 0;
		in		= 0;
		out		= 1.0;
		length	= 0;
		pitch	= 1.0;
		
		playing		= false;
		loopStart	= false;
		loopMode	= OF_LOOP_NORMAL;
		volume		= 1.0f;
		this->vHeaderLink = NULL;
		
		declickCount=0;
		
	}
	//------------------------------------------------------
	
	void AudioHeaderSample::draw()
	{
		int audioBuffDrawPos = 90;
		
		float currentLength=float(index)/((float)this->aBuffer->getMaxSizeInSamples())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float currentLengthInDeclick=float(in+declickLength*pitch)/((float)this->aBuffer->getMaxSizeInSamples())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float currentLengthOutDeclick=float(out-declickLength*pitch)/((float)this->aBuffer->getMaxSizeInSamples())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float oneLength=double(ofGetWidth()-PMDRAWSPACING*2)/(double(aBuffer->getMaxSizeInSamples()));
		int bufferDrawSize = (float(aBuffer->sizeInSamples())/float(aBuffer->getMaxSizeInSamples())) * (ofGetWidth()-PMDRAWSPACING*2);
		
		ofSetColor(0,255,255);
		
		ofPushStyle();
		ofSetLineWidth(3.0);
		ofLine(currentLength+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,currentLength+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+60);

		ofSetColor(255,255,0);
		ofSetLineWidth(1.0);
		ofLine(currentLengthInDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,currentLengthInDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+60);
		ofLine(currentLengthOutDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,currentLengthOutDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+60);
		ofPopStyle();
		
		ofSetColor(0,255,255);
		ofDrawBitmapString(ofToString(index),currentLength,PMDRAWELEMENTSY+10-audioBuffDrawPos);
		
		float	inPct  = double(in)/double(aBuffer->sizeInSamples());//int(float(aBuffer->sizeInSamples()-1)*(double(in)/double(aBuffer->sizeInSamples())));
		float	outPct = double(out)/double(aBuffer->sizeInSamples());//int(float(aBuffer->size()-1)*(out));
		
		int inPos = PMDRAWSPACING	+ int((inPct)*(bufferDrawSize)); //+ oneLength/2;
		int outPos = PMDRAWSPACING	+ int((outPct)*(bufferDrawSize)); //+ oneLength/2;
		//		int outPos = PMDRAWSPACING + ((aBuffer->size()-1-outFrame) * oneLength) + oneLength/2;
		
		// draw in & out lines
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
		
		ofDrawBitmapString("[ " + ofToString(in) ,ofPoint(inPos+0,PMDRAWELEMENTSY+75-audioBuffDrawPos));
		ofDrawBitmapString(ofToString(out) + " ]" ,ofPoint(outPos-30,PMDRAWELEMENTSY+75-audioBuffDrawPos));
		//		
		//	
		//		ofCircle(inPos,650,10);
		//		ofCircle(outPos,650,10);
		
		float vol = this->getVolume();
		ofRect(20,PMDRAWELEMENTSY-200,20,vol*100.0);
		ofDrawBitmapString(ofToString(index-float(in)) +"\n" +ofToString(float(out)-index) ,ofPoint(40,PMDRAWELEMENTSY-200));
	}
	
	//------------------------------------------------------
	
	
	float AudioHeaderSample::getFps()
	{
		return fps;
	}
	
	//------------------------------------------------------
	
	
	void AudioHeaderSample::setFps(float fps)
	{
		this->fps=fps;
	}
	
	//------------------------------------------------------
	
	AudioSample* AudioHeaderSample::getNextAudioSample()
	{
		aBuffer->lock();
		index=getNextPosition();
		AudioSample* aSample = aBuffer->getAudioSample(index);
		//index = fmod((index +1.0),1024);
		
		aBuffer->unlock();
		
		return aSample;
	}
	
	//------------------------------------------------------
	
	AudioSample* AudioHeaderSample::getAudioSample(int _index)
	{
		int getIndex = CLAMP(_index,0,aBuffer->sizeInSamples());
		AudioSample* aSample = aBuffer->getAudioSample(_index);
		return aSample;
	}
	
	//------------------------------------------------------
	// returns the real position in the buffer
	
	float AudioHeaderSample::getNextPosition()
	{
		float indexPosition = index;
		unsigned int bufferSizeInSamples=aBuffer->sizeInSamples();
		int oneSoundStreamBufferSize = aBuffer->getSoundStreamBufferSize();
		
		//		if(playing) oneFrame=(TimeDiff)(1000000.0/fps/speed);
		//		else oneFrame=(TimeDiff)(1000000.0/fps/1.0);
		//		
		//		unsigned int buffer_size=buffer->size();
		//		unsigned int totalNumFr = buffer->getTotalFrames();
		//		unsigned int lastAbsFrame = totalNumFr - buffer_size; 
		//		int	inFrame  = int(float(buffer_size-1)*(in));
		//		int outFrame = int(float(buffer_size-1)*(out));
		//		int	inAbsFrame  = totalNumFr -  inFrame;
		//		int	outAbsFrame = totalNumFr - outFrame;
		//		
		//		
		////		 printf("-------------------------------\nTOTAL : %d\nSIZE : %d \nLAST : %d\n IN : %d / %d \n OUT : %d / %d \nPOSITION : %f %d\n",
		////		 totalNumFr,
		////		 buffer_size,
		////		 lastAbsFrame,
		////		 inAbsFrame,totalNumFr-inAbsFrame,
		////		 outAbsFrame,totalNumFr-outAbsFrame,
		////		 position, int(position));
		//		 	
		//		
		//		
		//		if((float)positionTS.elapsed()+(position-floor(position))*(float)abs(oneFrame)>=abs(oneFrame))
		//		{
		//			if(oneFrame!=0)
		//			{
		//				position=position + (float)positionTS.elapsed()/(float)oneFrame;
		//
		//			}
		//			// updates the time-stamp with the current time
		//			positionTS.update();
		//		}
		
		
		
		
		////		printf("new Position %f !!!!!!! %f > %f\n ",position,position,float(outAbsFrame));
		// if header is playing and loopStart is requested, set position to inPoint or outPoint depending on speed's sign !
		if(playing && loopStart)
		{
			if(pitch>0.0) indexPosition=in;
			else indexPosition=out;
			loopStart=false;
			printf("avH::loopStart!\n");
		}
		
		//printf("loop out ? :: buffSamp %d - 1 - indexP %f= %d < %d \n",int(aBuffer->sizeInSamples()),indexPosition,int(aBuffer->sizeInSamples()-1-indexPosition),int(out));
		
		// if we're playing in loop and we're reaching the outpoint
		if(playing && ( (indexPosition) > (out)))
		{
			if(loopMode==OF_LOOP_NORMAL) 
			{
				indexPosition = in;
				resetTick();
				// throw event to the videoHeader to say : "hi ! go back to loop start !"
				if(vHeaderLink!=NULL) 
				{
					int i = 1;
//					ofNotifyEvent(loopInEvent,i,vHeaderLink);
				}
				
			}
			else if (loopMode==OF_LOOP_NONE)
			{
				setPlaying(false);
			}
			else if (loopMode==OF_LOOP_PALINDROME) 
			{
				pitch=-pitch;
			}
		}
		// if we're in playing in loop and we're reaching the inpoint (while speed is negative probably)
		else if(playing && ( (indexPosition) < (in)))
		{
			if(loopMode==OF_LOOP_NORMAL)
			{ 
				indexPosition = out;
				resetTick();
				// throw event to the videoHeader to say : "hi ! go back to loop start !"
				if(vHeaderLink!=NULL) 
				{
					int i = 1;
//					ofNotifyEvent(loopInEvent,i,vHeaderLink);
				}				
			}
			else if (loopMode==OF_LOOP_NONE) setPlaying(false);
			else if (loopMode==OF_LOOP_PALINDROME) 
			{
				pitch=-pitch;
			}
		}
		
		//		// clamp position to it's limits ...
		//		if(playing) position=CLAMP(position,float(inAbsFrame),float(outAbsFrame));
		//		else position=CLAMP(position,float(lastAbsFrame),float(totalNumFr));
		//		
		//		// backpos
		//		int backpos=0;	
		//		if (!playing) backpos=0;
		//		else {
		//			backpos=int(buffer->getTotalFrames()-int(position));
		//			backpos=CLAMP(backpos,0,buffer_size-1);
		//		}
		//		
		//		int nextPos;
		//		if (playing) nextPos= (buffer_size-1) - backpos;
		//		else		 nextPos= (buffer_size-1) - (delay/oneFrame);
		//		
		//		nextPos = CLAMP(nextPos,0,buffer_size-1);
		//		if(nextPos<0) nextPos=0;
		//		
		//		return nextPos;
		//
		
		//		if (playing) nextPos= (buffer_size-1) - backpos;
		//		else		 nextPos= (buffer_size-1) - (delay/oneFrame);
		
		if (playing) indexPosition= float(indexPosition);//indexPosition= (bufferSize-1) - backpos;
		else		 indexPosition= float(bufferSizeInSamples-oneSoundStreamBufferSize) - delay + tickCount;
		
		indexPosition = CLAMP(indexPosition,0.0,float(bufferSizeInSamples-1));
		
		return indexPosition;
	}
	
	//------------------------------------------------------
	void AudioHeaderSample::setPitch(float p)
	{
		this->pitch = p;
	}
	//------------------------------------------------------
	float AudioHeaderSample::getPitch() 
	{
		return pitch;
	}	
	
	//------------------------------------------------------
	// get & set delay 
	//------------------------------------------------------
	unsigned int AudioHeaderSample::getDelaySamples() 
	{
		return delay;
	}
	//------------------------------------------------------
	void AudioHeaderSample::setDelaySamples(unsigned int delaySamples)
	{
		resetTick();
		this->delay = delaySamples;
	}
	
	//------------------------------------------------------
	// get & set in & out
	//------------------------------------------------------
	unsigned int AudioHeaderSample::getInSamples() 
	{
		return this->in;
	}
	//------------------------------------------------------
	void AudioHeaderSample::setInSamples(unsigned int inSamples)
	{
		if(declickMutex.tryLock())
		{
			resetTick();
			this->in = (aBuffer->getMaxSizeInSamples()-1)-inSamples;

			// clamp the in value
			// can't compare as "in < 0" because it's an unsigned int !
			if (int(aBuffer->getMaxSizeInSamples()-1-inSamples)<0){
				this->in=0;
			}
			else if (this->in > aBuffer->getMaxSizeInSamples()) this->in=aBuffer->getMaxSizeInSamples()-1; 

			if(abs(int(this->in - this->out)) < aBuffer->getSoundStreamBufferSize())
			{
				// we don't let make the loop smaller then SoundStream buffer size !
				this->in = this->out - aBuffer->getSoundStreamBufferSize();
			}
			
			// upate out point based on length
			this->out = in + length;
			this->out = CLAMP(this->out,0,aBuffer->getMaxSizeInSamples()-1);
			declickMutex.unlock();
			
		}
	}
	//------------------------------------------------------
	unsigned int AudioHeaderSample::getOutSamples() 
	{
		return out;
	}
	//------------------------------------------------------
	void AudioHeaderSample::setOutSamples(unsigned int outSamples)
	{
		if(declickMutex.tryLock())
		{

			resetTick();
			this->out = aBuffer->getMaxSizeInSamples()-1-outSamples;   
			this->out = CLAMP(this->out,0,aBuffer->getMaxSizeInSamples()-1);

			if(abs(int(this->in - this->out)) < aBuffer->getSoundStreamBufferSize())
			{
				// we don't let make the loop smaller then SoundStream buffer size !
				this->out = this->in + aBuffer->getSoundStreamBufferSize();
			}
			this->length = out - in;
			declickMutex.unlock();
		}
	}
	//------------------------------------------------------
	void AudioHeaderSample::setLengthSamples(unsigned int l)
	{
		printf("AHS::l=%d\n",l);
		if(declickMutex.tryLock())
		{
			resetTick();
			length = l;
			if(length<aBuffer->getSoundStreamBufferSize()) length=aBuffer->getSoundStreamBufferSize();
			this->out = this->in + this->length;
			this->out = CLAMP(this->out,0,aBuffer->getMaxSizeInSamples()-1);
			
			
			declickMutex.unlock();
		}
	}
	//------------------------------------------------------
	unsigned int AudioHeaderSample::getLengthSamples() 
	{
		return length;
	}
	
	//------------------------------------------------------
	// get & set loop & playing
	//------------------------------------------------------
	int AudioHeaderSample::getLoopMode()
	{
		return loopMode;
	}
	//------------------------------------------------------
	void AudioHeaderSample::setLoopMode(int loop)
	{
		if((loopMode!=OF_LOOP_NORMAL) || (loopMode!=OF_LOOP_NONE) || (loopMode!=OF_LOOP_PALINDROME))
		{
			printf("ofxPlaymodes::AudioHeaderSample:: Incorrect loop mode!\n");
		}
		else loopMode = loop;
	}
	
	//------------------------------------------------------
	void AudioHeaderSample::setLoopToStart()
	{
		loopStart=true;
		resetTick();
	}
	//------------------------------------------------------
	bool AudioHeaderSample::isPlaying() 
	{
		return playing;
	}
	//------------------------------------------------------
	void AudioHeaderSample::setPlaying(bool isPlaying, float _pitch)
	{
		this->pitch = _pitch;
		if(isPlaying)
		{
			// if we're entering loop mode move position to in point
			// this behaviour is to sync entering loop mode with starting at inPoint or outPoint depending on speed
			this->playing = isPlaying;
			int	loopSample;
			if(pitch>0.0f) loopSample = in;
			else loopSample = out;
			
			resetTick();
			index = loopSample;
		}
		else
		{
			// if we're gettint out of loop mode move delay to actual position
			// this behaviour is to let the header (set by delay on no loop) where the loop was when deactivated
			// other beahaviour could be to let the header on delay / inPoint / outPoint position when loop is turned off
			resetTick();
			this->playing = isPlaying;
			delay = index;
		}
	}		
	//------------------------------------------------------
	void AudioHeaderSample::setPlaying(bool isPlaying)
	{
		this->setPlaying(isPlaying,1.0);	
	}
	//------------------------------------------------------
	void AudioHeaderSample::linkToVideoHeader(VideoHeader &vH)
	{
		this->vHeaderLink=&vH;
		ofAddListener(this->loopInEvent,this->vHeaderLink,&VideoHeader::receivedLoopEvent);
	}
	
	//------------------------------------------------------
	void AudioHeaderSample::setVolume(float v) 
	{
		this->volume = CLAMP (v,0.0f,1.0f);
	}
	//------------------------------------------------------
	float AudioHeaderSample::getVolume() 
	{
		
		declickLength=100;
		
		// try to detect if we're on a loop-click situation
//		if ((this->isPlaying() && pitch>0.0))
//		{
//			
//			if((declickCount==0))
//			{
//				
//				printf("getVol tries to get Mutex");
//				declickMutex.tryLock();
//				//declickMutex.lock();
//			}
//			
//			//if((float(out)-index)<(float(declickLength)))
//			if((float(out)-index)<(float(declickLength)*pitch))
//			{
//				declickCount=int((float(out)-index)/pitch);
//				//printf("-------------------\n in %d out %d index %d \n dclickCount %d // vol = %f\n",in,out,int(index),declickCount,ofMap(float(out)-index, 0.0, declickLength, 0.0, 1.0,true));
//				return(ofMap(float(out)-index, 0.0, declickLength, 0.0, 1.0,true));
//			}
//			//if((index-float(in))<(float(declickLength)))
//			if((index-float(in))<(float(declickLength)*pitch))
//			{
//				declickCount=int((index-float(in))/pitch);
//				//printf("-------------------\n in %d out %d index %d \n dclickCount %d // vol = %f\n",in,out,int(index),declickCount,ofMap(index-float(in), 0.0, declickLength, 0.0, 1.0,true));
//				return(ofMap(index-float(in), 0.0, declickLength, 0.0, 1.0,true));
//				
//			}
//			//printf("-------------------\n in %d out %d index %d \n dclickCount %d\n",in,out,int(index),declickCount);
//			
//		 	if(declickCount == int(float(declickLength-1)/pitch)) {
//				printf("getVol releases Mutex\n");
//				declickMutex.unlock();
//				declickCount=0;
//
//			}
//		
//		}
// 
		
		bool outRange=false;
		bool inRange=false;
		if (((float(out)-index)>=0)&&((float(out)-index)<(float(declickLength)*pitch))) outRange=true;	
		if (((index-float(in))>=0)&&((index-float(in))<(float(declickLength)*pitch))) inRange=true;	
											
		// case to avoid getting stuck on short loops -> no envelope is applied.
		if(inRange&&outRange) 
		{
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!|\n");
			declickMutex.unlock();
			return(1.0);
		}
		else if(inRange)
		{
			declickMutex.tryLock();
			float attFactor = ofMap(index-float(in), 0.0, declickLength*pitch, 0.0, 1.0,true);
			return(attFactor);
		}
		
		else if(outRange) 
		{
			declickMutex.tryLock();
			float attFactor = ofMap(float(out)-index, 0.0, declickLength*pitch, 0.0, 1.0,true);
			return(attFactor);
		}
		else {
			declickMutex.unlock();
			return(1.0);
		}
		
										
										   
										   
										   
		return volume;
	}
	//------------------------------------------------------
	void AudioHeaderSample::updateTick() 
	{
		tickCount = (tickCount+1)%aBuffer->getSoundStreamBufferSize();
		index = index + pitch;
	}
	//------------------------------------------------------
	void AudioHeaderSample::resetTick() 
	{
		tickCount = 0;
	}
	//------------------------------------------------------
	int AudioHeaderSample::getIndex() 
	{
		return index;
	}

}
