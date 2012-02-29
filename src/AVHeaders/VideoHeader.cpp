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
    playing    = false;
    delay       = 0;
	opacity		= 255;
	loopStart	= false;
	loopMode	= OF_LOOP_NORMAL;
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
}

//------------------------------------------------------
VideoHeader::~VideoHeader(){
}

//------------------------------------------------------
void VideoHeader::draw(){

	ofSetColor(255,128,0);
    //float length = (float(size())/float(maxSize))*(ofGetWidth()-(PMDRAWSPACING));
    float oneLength=(float)(ofGetWidth()-PMDRAWSPACING*2)/(float)(buffer->getMaxSize());

    float currentLength=float(currentPos+1)/((float)this->buffer->getMaxSize())*(float)(ofGetWidth()-PMDRAWSPACING*2);
	// draw header line
	ofPushStyle();
    ofSetLineWidth(1);
	
	ofRect(currentLength+PMDRAWSPACING-oneLength,PMDRAWELEMENTSY+30,oneLength,10);
	ofDrawBitmapString(ofToString(buffer->size()-1-currentPos),ofPoint(currentLength+PMDRAWSPACING-(oneLength/2),PMDRAWELEMENTSY+25));
	/*
	ofLine(int(currentLength)+PMDRAWSPACING,680,int(currentLength)+PMDRAWSPACING,720);

	// draw header triangle
	ofBeginShape();
		ofVertex(int(currentLength)+PMDRAWSPACING,710);
		ofVertex(int(currentLength-5)+PMDRAWSPACING,710+5);
		ofVertex(int(currentLength+5)+PMDRAWSPACING,710+5);
	ofEndShape();
	ofPopStyle();
	*/
	
	//ofDrawBitmapString(ofToString(this->delay/1000000.0f),ofPoint(currentLength,650.0));
	//ofDrawBitmapString("Position : "+ofToString(position),ofPoint(currentLength,635.0));
	//ofDrawBitmapString("Delay  : "+ofToString(this->delay),ofPoint(currentLength,650.0));
	
    //int inPos  = int(in*(float)buffer->size()/(float)this->buffer->getMaxSize()*(float)(ofGetWidth()));
    //int outPos = int(out*(float)buffer->size()/(float)this->buffer->getMaxSize()*(float)(ofGetWidth()));
	// from 1 to size !
	int	inFrame  = int(float(buffer->size()-1)*(in));
	int outFrame = int(float(buffer->size()-1)*(out));
	
	//int inPos = ((int((1.0-in)*(float)buffer->size()-1)+1)*oneLength)-oneLength/2;
	//int outPos = ((int((1.0-out)*(float)buffer->size()-1)+1)*oneLength)-oneLength/2;
	
	//int inPos	= ((ofGetWidth() - PMDRAWSPACING) - ((inFrame) * oneLength) - oneLength/2);
	//int outPos	= ((ofGetWidth() - PMDRAWSPACING) - ((outFrame) * oneLength) - oneLength/2);
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
int VideoHeader::getFps(){
    return fps;
}
//------------------------------------------------------
void VideoHeader::setFps(int fps){
    this->fps=fps;
}

//------------------------------------------------------

VideoFrame * VideoHeader::getVideoFrame(int index)
{
	buffer->lock();
	//indexFrame = buffer->size()-1;
	int indexFrame=getNextPosition();
	indexFrame = CLAMP(index,0,buffer->size()-1);
	indexFrame = buffer->size()-10;
	VideoFrame * frame = buffer->getVideoFrame(indexFrame);
	buffer->unlock();
	return frame;
}
		
//------------------------------------------------------
VideoFrame * VideoHeader::getNextVideoFrame(){

        buffer->lock();
			// **
			currentPos=getNextPosition();
			VideoFrame * frame = buffer->getVideoFrame(currentPos);
        buffer->unlock();
	
        return frame;
	
	// taken out from here **
	/*        if(pctHasChanged && buffer->size()){
	 pct=CLAMP(pct,0,1);
	 //cout << "vheader pct: "<< pct<< "\n";
	 currentPos=pct*(buffer->size()-1);
	 positionTS.update();
	 position=buffer->getTotalFrames()-(buffer->size()-currentPos);
	 pctHasChanged=false;
	 printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	 }else {
	 currentPos=getNextPosition();
	 pct=(float)currentPos/(float)buffer->size();
	 }
	 */
	
}


//------------------------------------------------------
int VideoHeader::getNextPosition(){
	// returns the real position in the buffer . calculate the next position in frames
    // from the beginning of the recording based on speed    
	// position expresses number of frames since start
	
	// when we're not in playing speed doesn't make too much sense ?¿
    /*
	// this option with getRealFPS makes wobbling delay ... not good.
	if(loopMode) oneFrame=(TimeDiff)(1000000.0/buffer->getRealFPS()/speed);
	else oneFrame=(TimeDiff)(1000000.0/buffer->getRealFPS()/1.0);
	*/
	
	if(playing) oneFrame=(TimeDiff)(1000000.0/fps/speed);
	else oneFrame=(TimeDiff)(1000000.0/fps/1.0);
	
	unsigned int buffer_size=buffer->size();
	unsigned int totalNumFr = buffer->getTotalFrames();
	unsigned int lastAbsFrame = totalNumFr - buffer_size; 
	int	inFrame  = int(float(buffer_size-1)*(in));
	int outFrame = int(float(buffer_size-1)*(out));
	int	inAbsFrame  = totalNumFr -  inFrame;
	int	outAbsFrame = totalNumFr - outFrame;
/*
	printf("-------------------------------\nTOTAL : %d\nSIZE : %d \nLAST : %d\n IN : %d / %d \n OUT : %d / %d \nPOSITION : %f %d\n",
		   totalNumFr,
		   buffer_size,
		   lastAbsFrame,
		   inAbsFrame,totalNumFr-inAbsFrame,
		   outAbsFrame,totalNumFr-outAbsFrame,
		   position, int(position));
*/	
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

	// if under playing loopStart is requested, set position to inPoint !
	if(playing && loopStart)
	{
		position=float(inAbsFrame);
		loopStart=false;
	}
	//printf("(%d) < pos %f < (%d) \n",inAbsFrame,position,outAbsFrame);
		
    // if we're in playing :: calculates the absolute position in frames for all the duration
	if(playing && (int(position) > (outAbsFrame)))
	{
		// detect loop cylce on forward speed
		//printf("out !! \n");
		position = float(inAbsFrame);
	}
    else if(playing && (int(position) < (inAbsFrame)))
	{
		// detect loop cylce on backward speed
		position=float(outAbsFrame);
		//printf("in NEW pos = %f !! \n",position);
	}
	
	// clamp position to it's limits ...
	/*
    if(loopMode) position=CLAMP(position,float(inAbsFrame),float(outAbsFrame));
    else position=CLAMP(position,float(lastAbsFrame),float(totalNumFr));
	*/
	
	// backpos
	int backpos=0;	
	if (!playing) backpos=0;
	else {
		backpos=int(buffer->getTotalFrames()-int(position));
		//printf("BACKPos %d \n",backpos);
		backpos=CLAMP(backpos,0,buffer_size-1);
	}
	
    // original int nextPos=buffer_size-backpos-delay/oneFrame;
	int nextPos;
	if (playing)
	{ 
			nextPos=(buffer_size-1)-backpos;
	}
	else nextPos = (buffer_size-1) - delay/oneFrame;
	
	
	nextPos = CLAMP(nextPos,0,buffer_size-1);

	//printf("__________________________________________back %d >> %d\n\n",backpos,nextPos);

    return nextPos;
}

//------------------------------------------------------
VideoBuffer *VideoHeader::getBuffer() const
{
    return buffer;
}

//------------------------------------------------------
void VideoHeader::setSpeed(float speed)
{
	this->speed = speed;
}
//------------------------------------------------------
float VideoHeader::getSpeed() const
{
    return speed;
}
//------------------------------------------------------
void VideoHeader::setOpacity(int opacity) 
{
	this->opacity = CLAMP (opacity,0,255);
}
//------------------------------------------------------
int VideoHeader::getOpacity() const
{
	return opacity;
}


	
	
	
//------------------------------------------------------
// get & set delay 
//------------------------------------------------------
int VideoHeader::getDelayMs() const
{
	return delay/1000;
}
//------------------------------------------------------
int VideoHeader::getDelayFrames() const
{
	return this->getDelayMs()/(TimeDiff)(1000.0/fps/1.0);
}
//------------------------------------------------------
float VideoHeader::getDelayPct() const
{
	float res = this->getDelayFrames()/(buffer->size()-1); 
	return res;
}
//------------------------------------------------------
void VideoHeader::setDelayMs(int delayMs)
{
	TimeDiff oneFrame=(TimeDiff)(1000000.0/fps/1.0);
	int delayToSet = delayMs*1000;
    this->delay = CLAMP(delayToSet,0,(buffer->getMaxSize()-1)*oneFrame);
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
float VideoHeader::getIn() const
{
	return in;
}
//------------------------------------------------------
void VideoHeader::setInMs(int in)
{
	float oneFrameMs=(TimeDiff)(1000.0/fps/1.0);
	this->setInPct(float(in) / (oneFrameMs*float(buffer->size())));
	printf("ms to set %d translated to %f% fps %d\n",in,float(in) / (oneFrameMs*float(buffer->size())),this->fps);
}
//------------------------------------------------------
void VideoHeader::setInPct(float in)
{
	this->in=CLAMP(in,this->out,1.0);
}
//------------------------------------------------------
void VideoHeader::setInFrames(int in)
{
	float pct = float(in)/float(buffer->size());
	this->setInPct(pct);
}
	
//------------------------------------------------------
float VideoHeader::getOut() const
{
	return out;
}
//------------------------------------------------------
void VideoHeader::setOutMs(int out)
{
	float oneFrameMs=(TimeDiff)(1000.0/fps/1.0);
	this->setOutPct(float(out) / (oneFrameMs*float(buffer->size())));
}
//------------------------------------------------------
void VideoHeader::setOutPct(float out)
{
	this->out=CLAMP(out,0.0f,this->in);
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
bool VideoHeader::isPlaying() const
{
	return playing;
}

//------------------------------------------------------
void VideoHeader::setPlaying(bool isPlaying)
{
	if(playing)
	{
		// if we're entering loop mode move position to in point
		// this behaviour is to sync entering loop mode with starting at inPoint
		this->playing = isPlaying;
		int	inFrame  = int(float(buffer->size()-1)*(in));
		int	inAbsFrame  = buffer->getTotalFrames() -  inFrame;
		position = inAbsFrame; 
	}
	else
	{
		// if we're gettint out of loop mode move delay to actual position
		// this behaviour is to let the header (set by delay on no loop) where the loop was when deactivated
		// other beahaviour could be to let the header on delay / inPoint / outPoint position when loop is turned off
		this->playing = isPlaying;
		float	actualFrame  = float(buffer->getTotalFrames()) - (position);
		TimeDiff oneFrame=(TimeDiff)(1000000.0/fps/1.0);		
		delay = actualFrame*oneFrame;
	}
	
}
	


	

}

