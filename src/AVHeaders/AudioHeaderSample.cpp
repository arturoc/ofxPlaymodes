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
//		in		= 0;
//		out		= 0;
		length	= 0;
		pitch	= 1.0;
		
		playing		= false;
		loopStart	= false;

		isCrossfading = false;
		lengthChanged = false;
		inChanged = false;
		nextLength = 0.0;
		nextIn=0.0;
		
		loopMode	= OF_LOOP_NORMAL;
		volume		= 1.0f;

	}
	
	//------------------------------------------------------
	
	AudioHeaderSample::~AudioHeaderSample()
	{
		ofRemoveListener(ofEvents().update,this,&AudioHeaderSample::update);
	}
	
	//------------------------------------------------------
	
	void AudioHeaderSample::setup(AudioBufferSamples & buffer)
	{
		
		this->aBuffer=&buffer;
		fps=aBuffer->getFps();
		
		index	= 0;
		delay   = 0;
		//in		= 0;
		//out		= aBuffer->getSoundStreamBufferSize();
//		length	= out - in;
		pitch	= 1.0;
		
		playing		= false;
		loopStart	= false;
		
		isCrossfading = false;
		lengthChanged = false;
		inChanged = false;
		nextLength = 0.0;
		nextIn=0.0;
		
		
		loopMode	= OF_LOOP_NORMAL;
		volume		= 1.0f;
		
		markIn.setup(int(0),buffer);
		markOut.setup(aBuffer->getUsedSizeInSamples(),buffer);
		length	= markOut.getIndex()-markIn.getIndex();
		
		// addListener for testApp::update event -> this class update!
		ofAddListener(ofEvents().update,this,&AudioHeaderSample::update);
		
		/*
		printf("AHS_setup:: markIn index:%d   leng:%d    in:%d     out:%d \n",
				int(markIn.getIndex()),
				int(markIn.getLength()),
				int(markIn.getMin()),
			    int(markIn.getMax()));
		*/
		
		

		
	}
	//------------------------------------------------------
	
	void AudioHeaderSample::draw()
	{
		int audioBuffDrawPos = 90;
		
		
		float currentLength=float(index-aBuffer->getUnusedSamples())/((float)this->aBuffer->getUsedSizeInSamples())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float currentLengthInDeclick=float(markIn.getIndex()-markIn.getLength()*pitch)/((float)this->aBuffer->getUsedSizeInSamples())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float currentLengthOutDeclick=float(markOut.getIndex()-markOut.getLength()*pitch)/((float)this->aBuffer->getUsedSizeInSamples())*(float)(ofGetWidth()-PMDRAWSPACING*2);
		float oneLength=double(ofGetWidth()-PMDRAWSPACING*2)/(double(aBuffer->getUsedSizeInSamples()));
		int bufferDrawSize = (float(aBuffer->sizeInSamples())/float(aBuffer->getUsedSizeInSamples())) * (ofGetWidth()-PMDRAWSPACING*2);
						
		ofSetColor(0,255,50);
		ofDrawBitmapString(ofToString(index-1-aBuffer->getUnusedSamples()),currentLength,PMDRAWELEMENTSY+10-audioBuffDrawPos);
		
		float	inPct  = double(markIn.getIndex())/double(aBuffer->sizeInSamples());//int(float(aBuffer->sizeInSamples()-1)*(double(in)/double(aBuffer->sizeInSamples())));
		float	outPct = double(markOut.getIndex())/double(aBuffer->sizeInSamples());//int(float(aBuffer->size()-1)*(out));
		
		int inPos = PMDRAWSPACING	+ int((inPct)*(bufferDrawSize)); //+ oneLength/2;
		int outPos = PMDRAWSPACING	+ int((outPct)*(bufferDrawSize)); //+ oneLength/2;
		
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
		
		ofDrawBitmapString("[ " + ofToString(markIn.getIndex()) ,ofPoint(inPos+0,PMDRAWELEMENTSY+75-audioBuffDrawPos));
		ofDrawBitmapString(ofToString(markOut.getIndex()) + " ]" ,ofPoint(outPos-30,PMDRAWELEMENTSY+75-audioBuffDrawPos));
		
		// draw audioHeader
		ofPushStyle();
		if(isCrossfading)
		{
			ofSetLineWidth(3.0);
			ofSetColor(255,0,0);
		}
		ofLine(currentLength+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,currentLength+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+60);
		ofRect(PMDRAWSPACING,PMDRAWELEMENTSY,-100,10);
		// we draw the lines of the crossfade area
		ofSetColor(64,128,128);
		ofSetLineWidth(1.0);
		// the limits
		ofLine(currentLengthInDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,currentLengthInDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+50);
		ofLine(currentLengthOutDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,currentLengthOutDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+50);
		// the ramp
		ofSetColor(0,128,128);
		ofLine(currentLengthOutDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos,outPos,PMDRAWELEMENTSY+10-audioBuffDrawPos+50);		
		ofLine(currentLengthInDeclick+PMDRAWSPACING,PMDRAWELEMENTSY+10-audioBuffDrawPos+50,inPos,PMDRAWELEMENTSY+10-audioBuffDrawPos);		
		ofPopStyle();
		
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
	
	AudioSample AudioHeaderSample::getNextAudioSample()
	{
		
		aBuffer->lock();

		index=getNextPosition();
	
		/*
		printf("AHS::getNextAudioSample = index %d ||  pct = %f || markIn le %d ; min %d max %d || markOut le %d ; min %d max %d \n",
			   index,
			   float(index-markOut.getMin())/float(markOut.getLength()),
			   int(markIn.getLength()),
			   int(markIn.getMin()),
			   int(markIn.getMax()),
			   int(markOut.getLength()),
			   int(markOut.getMin()),
			   int(markOut.getMax()));
		*/

		//create audiosample to modify it with the mix
		float pct = float(index-markOut.getMin())/float(markOut.getLength()) ;
		AudioSample aSample = aBuffer->getAudioSample(index);
		//printf("done NextAudioSample()...index %d || channels %d \n",index,aSample.getChannels());
		
		if(isPlaying()&&(int(index)>markOut.getMin()))//&&(int(index)<markOut.getIndex()))
		{
			if(!isCrossfading) isCrossfading=true;
			
			int mixB = int(markIn.getMin()) + (int(index)-int(markOut.getMin()));
			//printf("mixing index %d with mixB %d || mIn.min %d \n",int(index),int(mixB),int(markIn.getMin()));
			aSample = crossfade(aSample,mixB,pct);
			
			// case to avoid the crossfading boolean to be always true
			// when loops is as small as the crossfade region
			//if (int(index)>=markIn.getIndex())
			//{
			//	isCrossfading=false;
			//}
		}
		else if (isPlaying())
		{
			//printf("crosffdading false\n");
			if (isCrossfading) isCrossfading = false;
		}
		
		aBuffer->unlock();		
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
			if(pitch>0.0) indexPosition=markIn.getIndex();
			else indexPosition=markOut.getIndex();
			loopStart=false;
			//printf("avH::loopStart!\n");
		}
		
		//printf("loop out ? :: buffSamp %d - 1 - indexP %f= %d < %d \n",int(aBuffer->sizeInSamples()),indexPosition,int(aBuffer->sizeInSamples()-1-indexPosition),int(out));
		
		// if we're playing in loop and we're reaching the outpoint
		if(playing && ( (indexPosition) > (markOut.getIndex())))
		{
			if(loopMode==OF_LOOP_NORMAL) 
			{
				indexPosition = markIn.getIndex();
				resetTick();
				// throw event to the videoHeader to say : "hi ! go back to loop start !"
				
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
		else if(playing && ( (indexPosition) < (markIn.getIndex())))
		{
			if(loopMode==OF_LOOP_NORMAL)
			{ 
				indexPosition = markOut.getIndex();
				resetTick();
				// throw event to the videoHeader to say : "hi ! go back to loop start !"
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
		
//		if (playing) indexPosition= float(indexPosition);//indexPosition= (bufferSize-1) - backpos;
//		else		 indexPosition= float(bufferSizeInSamples-oneSoundStreamBufferSize) - delay + tickCount;

		//		else		 indexPosition= float(markIn.getIndex()) + tickCount;
		if ((playing)&&(aBuffer->getIsStopped())) 
		{
			// PLAY i BUFF.FREEZED
			// NORMAL PLAY IN LOOP SITUATION
			indexPosition= float(indexPosition);//indexPosition= (bufferSize-1) - backpos;
		}
		else if ((playing)&&(!aBuffer->getIsStopped()))
		{
			// PLAY i BUFF.NO FREEZED
			// ko !
			indexPosition= float(indexPosition);
			
		}
		else if ((!playing)&&(aBuffer->getIsStopped()))
		{
			// Ko Raro
			// NO PLAY i BUFF.FREEZED
			indexPosition= float(bufferSizeInSamples-oneSoundStreamBufferSize) - delay + tickCount;		
		}
		else if ((!playing)&&(!aBuffer->getIsStopped())) 
		{
			// Ok
			// NO PLAY i BUFF. NO FREEZED
			indexPosition= float(bufferSizeInSamples-oneSoundStreamBufferSize) - delay + tickCount;		
		}
		
		indexPosition = CLAMP(indexPosition,0.0,float(bufferSizeInSamples-1));
		
		return indexPosition;
	}
	
	
	//------------------------------------------------------
	void AudioHeaderSample::update(ofEventArgs &arg)
	{
		
		if((!isCrossfading)&&(lengthChanged))
		{
			lengthChanged=false;			
			// setting length()
			markOut.setIndex(nextLength);		
			// control out of bounds ...
			if(markOut.getIndex() < 0) markOut.setIndex(0);
			if(markOut.getIndex() > aBuffer->getUsedSizeInSamples()-1) markOut.setIndex(aBuffer->getUsedSizeInSamples()-1);
		}
		if((!isCrossfading)&&(inChanged))
		{
			inChanged=false;			
			// setting length()
			markIn.setIndex(nextIn);		
			// control out of bounds ...
			if(markIn.getIndex() < markIn.getLength()) markIn.setIndex(markIn.getLength());
			if(markIn.getIndex() > aBuffer->getUsedSizeInSamples()-1) markIn.setIndex(aBuffer->getUsedSizeInSamples()-1);
		}
		
	}
	
	//------------------------------------------------------
	void AudioHeaderSample::updateTick() 
	{
		tickCount = (tickCount+1)%aBuffer->getSoundStreamBufferSize();
		if ((playing)&&(!aBuffer->getIsStopped()))
		{
			index = index + pitch - 1.0;
		}
		else index = index + pitch;
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
		return markIn.getIndex();
//		return this->in;
		
	}
	//------------------------------------------------------
	void AudioHeaderSample::setInSamples(unsigned int inSamples)
	{
		// the incoming value is expressed as samples from origin=realTime(right side)
		// we pass the value "v" to in&out markers as "maxSamples - v" !!
		
		// control bounds of inSamples
		if (inSamples>=aBuffer->getUsedSizeInSamples()) inSamples = aBuffer->getUsedSizeInSamples()-1;
		else if (inSamples <0) inSamples=0;
		
		//to keep tick count on renderer working
		resetTick();

		
		int auxIn = (aBuffer->getUsedSizeInSamples()-1)-inSamples; 

		printf("AHS::trying %d / aux %d",int(inSamples),int(auxIn));
		
		// we check the left border, we have to be carefull with the crossfading length
		if (((auxIn) < markIn.getLength() )&&(!isCrossfading)) 
		{				
			// if inPoint is less then crossfadingLength -> =crossfadingLength 
			markIn.setIndex(markIn.getLength());
		}
		else if (!isCrossfading)
		{
			// now let's control the maximum to the right the inpoint could be.
			// inpoint could be as close to real time as max(soundStreamBufferSize , crossfade length) 
			int minValue = max(aBuffer->getSoundStreamBufferSize(),int(markIn.getLength()));
			if (auxIn > aBuffer->getUsedSizeInSamples()-1-aBuffer->getSoundStreamBufferSize()) 
			{
				markIn.setIndex(aBuffer->getUsedSizeInSamples()-1-aBuffer->getSoundStreamBufferSize());
				markOut.setIndex(markIn.getIndex() + aBuffer->getSoundStreamBufferSize());
				//this->length = aBuffer->getSoundStreamBufferSize();
			}
			else 
			{
				
//				if(!isCrossfading)
//				{
//					markOut.setIndex(markIn.getIndex() + this->length);
//					// control out of bounds ...
//					if(markOut.getIndex() < 0) markOut.setIndex(0);
//					if(markOut.getIndex() > aBuffer->getMaxSizeInSamples()-1) markOut.setIndex(aBuffer->getMaxSizeInSamples()-1);
//					
//				}
//				else if (isCrossfading)
//				{
//					nextLength = markIn.getIndex() + this->length;
//					lengthChanged=true;
//				}
				
				
				
				
				markIn.setIndex(auxIn);
				if((markIn.getIndex() + length) > aBuffer->getUsedSizeInSamples()-1)
				{
					//this->length = (aBuffer->getUsedSizeInSamples() - 1) - markIn.getIndex();
					markOut.setIndex((aBuffer->getUsedSizeInSamples() - 1));
				}
				else 
				{
					markOut.setIndex(markIn.getIndex() + this->length);
				}

			}

		}	
		else if (isCrossfading)
		{
			
			nextIn = markIn.getIndex();
			inChanged=true;
		}
		
	}
	//------------------------------------------------------
	unsigned int AudioHeaderSample::getOutSamples() 
	{
		return markOut.getIndex();
		//		return out;
	}
	//------------------------------------------------------
	void AudioHeaderSample::setOutSamples(unsigned int outSamples)
	{
		
		resetTick();

		int auxOut = (aBuffer->getUsedSizeInSamples()-1)-outSamples; 
		
		if(auxOut > (aBuffer->getUsedSizeInSamples()-1)) 
		{
			markOut.setIndex(aBuffer->getUsedSizeInSamples()-1);
		}
		else if (auxOut < (markIn.getIndex() + aBuffer->getSoundStreamBufferSize())) 
		{
			markOut.setIndex(markIn.getIndex() + aBuffer->getSoundStreamBufferSize());

		}
		else 
		{
			markOut.setIndex(auxOut);
			
		}

		this->length = markIn.getIndex() - markOut.getIndex();
	}
	//------------------------------------------------------
	void AudioHeaderSample::setLengthSamples(unsigned int l)
	{
		
		resetTick();
		length = l;
		int minValue = max(aBuffer->getSoundStreamBufferSize(),int(markIn.getLength()));

		
		// control the case where length is too small
		if(length < minValue)
		{
			length=minValue;
		}
		if ((markOut.getIndex()-length)<=markIn.getIndex())
		{
			markOut.setIndex(markIn.getIndex() + this->length);
			// control out of bounds ...
			if(markOut.getIndex() < 0) markOut.setIndex(0);
			if(markOut.getIndex() > aBuffer->getUsedSizeInSamples()-1) markOut.setIndex(aBuffer->getUsedSizeInSamples()-1);			
		}
		// control the case where length goes beyond ranges
		else if ((markIn.getIndex() + length) > (aBuffer->getUsedSizeInSamples()-1)) 
		{
			length=(aBuffer->getUsedSizeInSamples()-1)-markIn.getIndex();
		}
		if(!isCrossfading)
		{
			markOut.setIndex(markIn.getIndex() + this->length);
			// control out of bounds ...
			if(markOut.getIndex() < 0) markOut.setIndex(0);
			if(markOut.getIndex() > aBuffer->getUsedSizeInSamples()-1) markOut.setIndex(aBuffer->getUsedSizeInSamples()-1);
			
		}
		else if (isCrossfading)
		{
			nextLength = markIn.getIndex() + this->length;
			lengthChanged=true;
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
			if(pitch>0.0f) loopSample = markIn.getIndex();
			else loopSample = markOut.getIndex();
			
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
			delay = aBuffer->sizeInSamples()-1-markOut.getIndex();
		}
	}		
	//------------------------------------------------------
	void AudioHeaderSample::setPlaying(bool isPlaying)
	{
		this->setPlaying(isPlaying,1.0);	
	}
//	//------------------------------------------------------
//	void AudioHeaderSample::linkToVideoHeader(VideoHeader &vH)
//	{
//		this->vHeaderLink=&vH;
//		ofAddListener(this->loopInEvent,this->vHeaderLink,&VideoHeader::receivedLoopEvent);
//	}
	
	//------------------------------------------------------
	void AudioHeaderSample::setVolume(float v) 
	{
		this->volume = CLAMP (v,0.0f,1.0f);
	}
	//------------------------------------------------------
	float AudioHeaderSample::getVolume() 
	{
		return(volume);
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
	//------------------------------------------------------
	unsigned int AudioHeaderSample::getDelay() 
	{
		return delay;
	}
	//------------------------------------------------------
	AudioSample AudioHeaderSample::crossfade(const AudioSample & sampleA,int mixB,float pct)
//	AudioSample*  AudioHeaderSample::crossfade(int mixA,int mixB,float pct)
	{

		//printf("AHS::sampleA channels %d :: mixB %d :: \n",sampleA->getChannels(),mixB);
		AudioSample sampleB = aBuffer->getAudioSample(int(mixB));
		float * samples = new float[sampleA.getChannels()];
		float mix;
		for(int i=0;i<sampleA.getChannels();i++)
		{
			mix = ((1.0f-pct)*sampleA.getAudioData()[i]) + ((pct)*sampleB.getAudioData()[i]);
			samples[i] = mix;
		}
		
		return AudioSample(samples,sampleA.getChannels());
		
	}
	
	

	//------------------------------------------------------
	void AudioHeaderSample::setDeClickLength(int nSamples)
	{
		// case where crossfade is b
		printf("--");
		markOut.setLength(nSamples);
		markIn .setLength(nSamples);
	}
	
	//-------------------------------------------------------
	int AudioHeaderSample::getCrossfadingWidth()
	{
		return markIn.getLength();
		
	}

	//-------------------------------------------------------
	int AudioHeaderSample::getAudioBufferUnusedSamples()
	{
		return aBuffer->getUnusedSamples();
		
	}

	//-------------------------------------------------------
}
