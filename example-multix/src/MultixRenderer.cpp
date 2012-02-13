/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "MultixRenderer.h"
using namespace ofxPm;

MultixRenderer::MultixRenderer(VideoBuffer & buffer, int numHeaders)
{
	setup(buffer,numHeaders);
}

MultixRenderer::~MultixRenderer()
{
    //dtor
}

MultixRenderer::MultixRenderer(){
	numHeaders=0;
	videoBuffer=NULL;

	minmaxBlend=0;

	delayOffset=0;
	speedOffset=0;

	in=0; out=1;
	loopMode=0;

	prevNumHeaders=numHeaders;
}

void MultixRenderer::setup(VideoBuffer & buffer, int numHeaders){

    this->numHeaders=numHeaders;
    videoBuffer=&buffer;

    minmaxBlend=0;

    delayOffset=0;
    speedOffset=0;

    in=0; out=1;
    loopMode=0;

    for (int i=0;i<numHeaders;i++){
        videoHeader.push_back( new VideoHeader(buffer) );
        videoRenderer.push_back(new VideoRenderer(videoHeader[i]));
    }
    prevNumHeaders=numHeaders;
}

void MultixRenderer::setNumHeaders(int numHeaders){
    this->numHeaders=numHeaders;
}

int MultixRenderer::getNumHeaders(){
    return prevNumHeaders;
}

void MultixRenderer::update(){
    int currNumHeaders = numHeaders;
    if(currNumHeaders>prevNumHeaders){
        for(int i=0;i<currNumHeaders-prevNumHeaders;i++){
            videoHeader.push_back( new VideoHeader(*videoBuffer) );
            videoRenderer.push_back(new VideoRenderer(videoHeader[prevNumHeaders+i]));
        }
    }else if(currNumHeaders<prevNumHeaders){
        for(int i=0;i<prevNumHeaders-currNumHeaders;i++){
            delete videoHeader.back();
            videoHeader.pop_back();
            delete videoRenderer.back();
            videoRenderer.pop_back();
        }
    }
    prevNumHeaders=currNumHeaders;

    //videoRenderer[0]->speed=
    for(int i=currNumHeaders-1; i>=0; i--){
        videoHeader[i]->setIn(in);
        videoHeader[i]->setOut(out);
        videoHeader[i]->setLoopMode(loopMode);
        videoHeader[i]->setDelay(delayOffset*1000*i);
        videoHeader[i]->setSpeed(videoHeader[0]->getSpeed()+speedOffset*i);

        videoRenderer[i]->setTint(tint);
        videoRenderer[i]->setMinmaxBlend(minmaxBlend);
        //videoRenderer[i]->activateShader=activateShader;
    }
}

void MultixRenderer::draw(){
	for(int i = videoRenderer.size()-1; i>=0; i--){
		videoRenderer[i]->draw();
	}
}

float MultixRenderer::getDelayOffset() const
{
    return delayOffset;
}

float MultixRenderer::getIn() const
{
    return in;
}

int MultixRenderer::getLoopMode() const
{
    return loopMode;
}

float MultixRenderer::getOut() const
{
    return out;
}

int MultixRenderer::getPrevNumHeaders() const
{
    return prevNumHeaders;
}

float MultixRenderer::getSpeedOffset() const
{
    return speedOffset;
}

ofColor MultixRenderer::getTint() const
{
    return tint;
}

VideoBuffer *MultixRenderer::getVideoBuffer() const
{
    return videoBuffer;
}

vector<VideoHeader*> MultixRenderer::getVideoHeader() const
{
    return videoHeader;
}

vector<VideoRenderer*> MultixRenderer::getVideoRenderer() const
{
    return videoRenderer;
}

bool MultixRenderer::isMinmaxBlend() const
{
    return minmaxBlend;
}

void MultixRenderer::setDelayOffset(float delayOffset)
{
    this->delayOffset = delayOffset;
}

void MultixRenderer::setIn(float in)
{
    this->in = in;
}

void MultixRenderer::setLoopMode(int loopMode)
{
    this->loopMode = loopMode;
}

void MultixRenderer::setMinmaxBlend(bool minmaxBlend)
{
    this->minmaxBlend = minmaxBlend;
}

void MultixRenderer::setOut(float out)
{
    this->out = out;
}

void MultixRenderer::setPrevNumHeaders(int prevNumHeaders)
{
    this->prevNumHeaders = prevNumHeaders;
}

void MultixRenderer::setSpeedOffset(float speedOffset)
{
    this->speedOffset = speedOffset;
}

void MultixRenderer::setTint(ofColor tint)
{
    this->tint = tint;
}

void MultixRenderer::setVideoBuffer(VideoBuffer *videoBuffer)
{
    this->videoBuffer = videoBuffer;
}

void MultixRenderer::setVideoHeader(vector<VideoHeader*> videoHeader)
{
    this->videoHeader = videoHeader;
}

void MultixRenderer::setVideoRenderer(vector<VideoRenderer*> videoRenderer)
{
    this->videoRenderer = videoRenderer;
}


VideoHeader * MultixRenderer::getHeader(int header){
	return videoHeader[header];
}

VideoRenderer * MultixRenderer::getRenderer(int renderer){
	return videoRenderer[renderer];
}
