/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoHeader.h"

namespace ofxPm{
VideoHeader::VideoHeader(VideoBuffer & buffer){
setup(buffer);
}

//------------------------------------------------------
VideoHeader::VideoHeader(){
    fps         = 25;
    position    = 0;
    oneFrame    = (TimeDiff)round(1000000.0/(float)fps);
    speed       = 1;
    in          = 1;
    out         = 0;
    delay       = 0;
	opacity		= 255;

    playing     = false;
	loopStart	= false;
	loopMode	= OF_LOOP_NORMAL;
	driveMode	= 0;
}


//------------------------------------------------------
void VideoHeader::setup(VideoBuffer & buffer){
    //newFrameEvent.init("Playmodes.VideoHeader.newFrame");
    this->buffer= &buffer;
    fps         = buffer.getFps();
    position    = buffer.size();
    oneFrame    = (TimeDiff)round(1000000.0/(float)fps);
    speed       = 1;
    in          = 1;
    out         = 0;
    playing    = false;
    delay       = 0;
	opacity		= 255;
	loopStart	= false;
	loopMode	= OF_LOOP_NORMAL;
	driveMode	= 0;

}

//------------------------------------------------------
VideoHeader::~VideoHeader(){
}

//------------------------------------------------------
void VideoHeader::draw(){

	
    float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(buffer->getMaxSize());
    float currentLength=float(currentPos+1)/((float)this->buffer->getMaxSize())*(float)(ofGetWidth()-PMDRAWSPACING*2);
	
	// draw header line
    ofSetLineWidth(1);
	if(playing) ofSetColor(0,200,0);
	else ofSetColor(255,128,0);

	ofRect(currentLength+PMDRAWSPACING-oneLength,PMDRAWELEMENTSY+30,oneLength,10);
	ofDrawBitmapString(ofToString(buffer->size()-1-currentPos),ofPoint(currentLength+PMDRAWSPACING-(oneLength/2),PMDRAWELEMENTSY+25));

	ofSetColor(255,128,0);
	
	int	inFrame  = int(float(buffer->size()-1)*(in));
	int outFrame = int(float(buffer->size()-1)*(out));
	int inPos = PMDRAWSPACING + ((buffer->size()-1-inFrame) * oneLength) + oneLength/2;
	int outPos = PMDRAWSPACING + ((buffer->size()-1-outFrame) * oneLength) + oneLength/2;
		
	// draw in & out lines
	ofSetLineWidth(1.0);
	ofLine(inPos,PMDRAWELEMENTSY+10,inPos,PMDRAWELEMENTSY+60);
    ofLine(outPos,PMDRAWELEMENTSY+10,outPos,PMDRAWELEMENTSY+60);
	ofLine(inPos,PMDRAWELEMENTSY+60,outPos,PMDRAWELEMENTSY+60);
	// draw inPos triangle
	ofBeginShape();
		ofVertex(inPos,PMDRAWELEMENTSY+10);
		ofVertex(inPos+5,PMDRAWELEMENTSY+5);
		ofVertex(inPos,PMDRAWELEMENTSY);
	ofEndShape();
	// draw outPos triangle
	ofBeginShape();
		ofVertex(outPos,PMDRAWELEMENTSY+10);
		ofVertex(outPos-5,PMDRAWELEMENTSY+5);
		ofVertex(outPos,PMDRAWELEMENTSY);
	ofEndShape();
	
	ofDrawBitmapString("[ " + ofToString(inFrame),ofPoint(inPos+0,PMDRAWELEMENTSY+75));
	ofDrawBitmapString(ofToString(outFrame) + " ]" ,ofPoint(outPos-30,PMDRAWELEMENTSY+75));
	
	ofSetColor(255,255,255);
}

//------------------------------------------------------
float VideoHeader::getFps(){
    return fps;
}
//------------------------------------------------------
void VideoHeader::setFps(float fps){
    this->fps=fps;
}

//------------------------------------------------------

VideoFrame * VideoHeader::getVideoFrame(int index)
{
	buffer->lock();
		int indexFrame = CLAMP(index,0,buffer->size()-1);
		VideoFrame * frame = buffer->getVideoFrame(indexFrame);
	buffer->unlock();
	return frame;
}
		
//------------------------------------------------------
VideoFrame * VideoHeader::getNextVideoFrame(){

        buffer->lock();
			currentPos=getNextPosition();
			VideoFrame * frame = buffer->getVideoFrame(currentPos);
        buffer->unlock();
	printf("current Pos %d\n",currentPos);
        return frame;
}

//------------------------------------------------------
int VideoHeader::getNextPosition(){
	// returns the real position in the buffer . calculate the next position in frames
    // from the beginning of the recording based on speed    
	// position expresses number of frames since start
	
	// calculate how much microseconds is a frame
	// if we're playing, speed has sense, if not not ?
	
	unsigned int buffer_size;
	unsigned int totalNumFr;
	unsigned int lastAbsFrame;
	int	inFrame;
	int outFrame;
	int	inAbsFrame;
	int	outAbsFrame;
	int backpos;
	int nextPos;
	
	switch (driveMode) 
	{
		case 0 :
			// normal mode, based on time
			
			if(playing) oneFrame=(TimeDiff)(1000000.0/fps/speed);
			else oneFrame=(TimeDiff)(1000000.0/fps/1.0);
			
			buffer_size=buffer->size();
			totalNumFr = buffer->getTotalFrames();
			lastAbsFrame = totalNumFr - buffer_size; 
			inFrame  = int(float(buffer_size-1)*(in));
			outFrame = int(float(buffer_size-1)*(out));
			inAbsFrame  = totalNumFr -  inFrame;
			outAbsFrame = totalNumFr - outFrame;
			
			//	printf("VIDEOVIDEOVIDEOVIDEO______________________________\n\n");
			//	printf("-------------------------------\nTOTAL : %d\nSIZE : %d \nLAST : %d\n IN : %d / %d \n OUT : %d / %d \nPOSITION : %f %d\n",
			//		   totalNumFr,
			//		   buffer_size,
			//		   lastAbsFrame,
			//		   inAbsFrame,totalNumFr-inAbsFrame,
			//		   outAbsFrame,totalNumFr-outAbsFrame,
			//		   position, int(position));
			//
			//	printf("VIDEOVIDEOVIDEOVIDEO______________________________\n\n");
			
			// if time spend since last positionTS.update() + portion to next frame is >= oneFrame
			// means that we need to update the position !!
			if((float)positionTS.elapsed()+(position-floor(position))*(float)abs(oneFrame)>=abs(oneFrame))
			{
				if(oneFrame!=0)
				{
					position=position + (float)positionTS.elapsed()/(float)oneFrame;
				}
				// updates the time-stamp with the current time
				positionTS.update();		
			}
			
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
				if(loopMode==OF_LOOP_NORMAL) position = float(inAbsFrame);
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
				if(loopMode==OF_LOOP_NORMAL) position = float(outAbsFrame);
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
			backpos=0;	
			if (!playing) backpos=0;
			else {
				backpos=int(buffer->getTotalFrames()-int(position));
				backpos=CLAMP(backpos,0,buffer_size-1);
			}
			
			nextPos;
			if (playing) nextPos= (buffer_size-1) - backpos;
			else		 nextPos= (buffer_size-1) - (delay/oneFrame);
			
			nextPos = CLAMP(nextPos,0,buffer_size-1);
			return nextPos;
		
			break;
			
		case 1 :
			// position driven by audio trough calls to delay !!
			oneFrame=(TimeDiff)(1000000.0/fps/1.0);
			buffer_size=buffer->size();
			nextPos= int(buffer_size-1) - int(float(delay)/float(oneFrame));
			nextPos = CLAMP(nextPos,0,buffer_size-1);
			printf("nextPos %d delay %d\n",nextPos,delay);
			return nextPos;
			
			break;
		default:
			break;
	}
}

//------------------------------------------------------
VideoBuffer *VideoHeader::getBuffer()
{
    return buffer;
}

//------------------------------------------------------
void VideoHeader::setSpeed(float speed)
{
	this->speed = speed;
}
//------------------------------------------------------
float VideoHeader::getSpeed() 
{
    return speed;
}
//------------------------------------------------------
void VideoHeader::setOpacity(int opacity) 
{
	this->opacity = CLAMP (opacity,0,255);
}
//------------------------------------------------------
int VideoHeader::getOpacity() 
{
	return opacity;
}


	
	
	
//------------------------------------------------------
// get & set delay 
//------------------------------------------------------
int VideoHeader::getDelayMs() 
{
	return delay/1000;
}
//------------------------------------------------------
int VideoHeader::getDelayFrames() 
{
	return this->getDelayMs()/(TimeDiff)(1000.0/fps/1.0);
}
//------------------------------------------------------
float VideoHeader::getDelayPct() 
{
	float res = this->getDelayFrames()/(buffer->size()-1); 
	return res;
}
//------------------------------------------------------
void VideoHeader::setDelayMs(float delayMs)
{
	TimeDiff oneFrame=(TimeDiff)(1000000.0/fps/1.0);
	float delayToSet = delayMs*1000.0;
    this->delay = CLAMP(int(delayToSet),0,(buffer->getMaxSize()-1)*oneFrame);
	// ? eloi hack
	//this->position = 0;
}
//------------------------------------------------------
void VideoHeader::setDelayFrames(int delayFrames)
{
	this->setDelayMs(int((float(delayFrames) / float(this->getFps()))*1000.0));
}
//------------------------------------------------------
void VideoHeader::setDelayPct(float pct)
{
	this->setDelayFrames(int(float(pct) * float(buffer->size())));
}

	
//------------------------------------------------------
// get & set in & out
//------------------------------------------------------
float VideoHeader::getIn() 
{
	return in;
}
//------------------------------------------------------
void VideoHeader::setInMs(float in)
{
	float oneFrameMs=(TimeDiff)(1000000.0/fps/1.0);
	float fAux = float(in*1000.0f) / (oneFrameMs*float(buffer->size()));
	this->setInPct(CLAMP(fAux,0.0,1.0));    
	
	printf("vH :: in %d = pct %f\n",in,float(in*1000.0f) / (oneFrameMs*float(buffer->size())));
	//printf("ms to set %f translated to %f fps %f\n",in,float(in) / (oneFrameMs*float(buffer->size())),this->fps);
	//printf("VIDEO inMs %d :: %f \n",in,float(in) / (oneFrameMs*float(buffer->size())));
}
//------------------------------------------------------
void VideoHeader::setInPct(float in)
{
	//this->in=CLAMP(in,this->out,1.0);
	this->in=in;
}
//------------------------------------------------------
void VideoHeader::setInFrames(int in)
{
	float pct = float(in)/float(buffer->size());
	this->setInPct(pct);
}
	
//------------------------------------------------------
float VideoHeader::getOut() 
{
	return out;
}
//------------------------------------------------------
void VideoHeader::setOutMs(float out)
{
	float oneFrameMs=(TimeDiff)(1000000.0/fps/1.0);
	float fAux = float(out*1000.0f) / (oneFrameMs*float(buffer->size()));
	this->setOutPct(CLAMP(fAux,0.0,1.0));    

}
//------------------------------------------------------
void VideoHeader::setOutPct(float out)
{
	this->out=CLAMP(out,0.0f,this->in);
	this->out=out;

}
//------------------------------------------------------
void VideoHeader::setOutFrames(int out)
{
	float pct = float(out)/float(buffer->size());
	this->setInPct(pct);
}

//------------------------------------------------------
// get & set loop & playing
//------------------------------------------------------
int VideoHeader::getLoopMode()
{
	return loopMode;
}
//------------------------------------------------------
void VideoHeader::setLoopMode(int loop)
{
	if((loopMode!=OF_LOOP_NORMAL) || (loopMode!=OF_LOOP_NONE) || (loopMode!=OF_LOOP_PALINDROME))
	{
		printf("ofxPlaymodes::VideoHeader:: Incorrect loop mode!\n");
	}
	else loopMode = loop;
}
	
//------------------------------------------------------
void VideoHeader::setLoopToStart()
{
	loopStart=true;
}
//------------------------------------------------------
bool VideoHeader::isPlaying() 
{
	return playing;
}
//------------------------------------------------------
void VideoHeader::setPlaying(bool isPlaying, float speed)
{
	this->speed = speed;
	if(isPlaying)
	{
		// if we're entering loop mode move position to in point
		// this behaviour is to sync entering loop mode with starting at inPoint or outPoint depending on speed
		this->playing = isPlaying;
		int	loopFrame;
		if(speed>0.0f) loopFrame = int(float(buffer->size()-1)*(in));
		else loopFrame = int(float(buffer->size()-1)*(out));
		int	inAbsFrame  = buffer->getTotalFrames() -  loopFrame;
		position = inAbsFrame; 
	}
	else
	{
		// if we're gettint out of loop mode move delay to actual position
		// this behaviour is to let the header (set by delay on no loop) where the loop was when deactivated
		// other beahaviour could be to let the header on delay / inPoint / outPoint position when loop is turned off
		this->playing = isPlaying;
		float	actualFrame  = float(buffer->getTotalFrames()-1) - (position);
		TimeDiff oneFrame=(TimeDiff)(1000000.0/fps/1.0);		
		delay = (actualFrame-1)*oneFrame;
	}
}		
//------------------------------------------------------
void VideoHeader::setPlaying(bool isPlaying)
{
	this->setPlaying(isPlaying,1.0);	
}
	

	//------------------------------------------------------
	void VideoHeader::receivedLoopEvent(int &i)
	{
		setLoopToStart();
	}	
	void VideoHeader::setDriveMode(int mode)
	{
		driveMode = mode;
	}
	

}

