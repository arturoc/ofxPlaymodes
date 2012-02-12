/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "MultixRenderer.h"

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

	scale=1;
	rotationZ=0;
	rotationX=0;
	rotationY=0;
	rotZOffset=0;
	rotYOffset=0;
	x=0; y=0; z=0;
	xOffset=0;
	yOffset=0;
	anchorX=0;
	anchorY=0;
	anchorZ=0;
	alpha=255;
	tintR=255;
	tintG=255;
	tintB=255;

	depthTest=0;

	minmaxBlend=0;

	delayOffset=0;
	speedOffset=0;

	in=0; out=1;
	loopMode=0;


	yRotSpeed=0;
	totYRot=0;

	prevNumHeaders=numHeaders;
}

void MultixRenderer::setup(VideoBuffer & buffer, int numHeaders){

    this->numHeaders=numHeaders;
    videoBuffer=&buffer;

    scale=1;
    rotationZ=0;
    rotationX=0;
    rotationY=0;
    rotZOffset=0;
    rotYOffset=0;
    x=0; y=0; z=0;
    xOffset=0;
    yOffset=0;
    anchorX=0;
    anchorY=0;
    anchorZ=0;
    alpha=255;
    tintR=255;
    tintG=255;
    tintB=255;

    depthTest=0;

    minmaxBlend=0;

    delayOffset=0;
    speedOffset=0;

    in=0; out=1;
    loopMode=0;


    yRotSpeed=0;
    totYRot=0;

    for (int i=0;i<numHeaders;i++){
        videoHeader.push_back( new VideoHeader(buffer) );
        videoRenderer.push_back(new VideoRenderer(videoHeader[i]));
    }
    prevNumHeaders=numHeaders;
}

void MultixRenderer::setNumHeaders(int numHeaders){
    int prevNumHeaders = this->numHeaders;
    if(numHeaders>prevNumHeaders){
        for(int i=0;i<numHeaders-prevNumHeaders;i++){
            videoHeader.push_back( new VideoHeader(*videoBuffer) );
            videoRenderer.push_back(new VideoRenderer(videoHeader[prevNumHeaders+i]));
        }
    }else if(numHeaders<prevNumHeaders){
        for(int i=0;i<prevNumHeaders-numHeaders;i++){
            delete videoHeader.back();
            videoHeader.pop_back();
            delete videoRenderer.back();
            videoRenderer.pop_back();
        }
    }
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


    videoRenderer[0]->setRotationY(rotationY);
    //videoRenderer[0]->rotationX=rotationX;
    videoRenderer[0]->setX(x);
    videoRenderer[0]->setY(y);
    videoRenderer[0]->setZ(z);

    //videoRenderer[0]->speed=
    for(int i=currNumHeaders-1; i>=0; i--){
        videoHeader[i]->setIn(in);
        videoHeader[i]->setOut(out);
        videoHeader[i]->setLoopMode(loopMode);
        videoHeader[i]->setDelay(delayOffset*1000*i);
        videoHeader[i]->setSpeed(videoHeader[0]->getSpeed()+speedOffset*i);

        videoRenderer[i]->setAlpha(alpha);
        videoRenderer[i]->setAnchorX(anchorX);
        videoRenderer[i]->setAnchorY(anchorY);
        videoRenderer[i]->setRotationZ(rotationZ+rotZOffset*i);
        if(i>0){
            videoRenderer[i]->setRotationY(videoRenderer[i-1]->getRotationY());
            videoRenderer[i]->setRotationX(videoRenderer[i-1]->getRotationX());
        }
        videoRenderer[i]->setScale(scale);
        videoRenderer[i]->setTintR(tintR);
        videoRenderer[i]->setTintG(tintG);
        videoRenderer[i]->setTintB(tintB);


        videoRenderer[i]->setX(videoRenderer[0]->getX()-xOffset*i);
        videoRenderer[i]->setY(videoRenderer[0]->getY()-yOffset*i);

        if(i>0){
            videoRenderer[i]->setZ(videoRenderer[i-1]->getZ());
            //videoRenderer[i]->y=videoRenderer[i-1]->y;
        }
        videoRenderer[i]->setMinmaxBlend(minmaxBlend);
        //videoRenderer[i]->activateShader=activateShader;
    }
    totYRot+=yRotSpeed;
}

void MultixRenderer::draw(){
	ofPushStyle();
    if(depthTest)
        glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glTranslatef(CAPTURE_WIDTH/2+videoRenderer[0]->getX(),0,videoRenderer[0]->getZ()+anchorZ);
    glRotatef(totYRot,0,1,0);
    glTranslatef(-CAPTURE_WIDTH/2-videoRenderer[0]->getX(),0,-videoRenderer[0]->getZ()-anchorZ);
        for(int i =prevNumHeaders-1; i>=0; i--){
        glPushMatrix();
            glTranslatef(CAPTURE_WIDTH/2+videoRenderer[i]->getX(),0,videoRenderer[i]->getZ()+anchorZ);
            glRotatef(rotYOffset*i,0,1,0);
            glTranslatef(-CAPTURE_WIDTH/2-videoRenderer[i]->getX(),0,-videoRenderer[i]->getZ()-anchorZ);
            videoRenderer[i]->draw();
        glPopMatrix();
        }
    glPopMatrix();
    if(depthTest)
        glDisable(GL_DEPTH_TEST);
    ofPopStyle();
}

int MultixRenderer::getAlpha() const
{
    return alpha;
}

float MultixRenderer::getAnchorX() const
{
    return anchorX;
}

float MultixRenderer::getDelayOffset() const
{
    return delayOffset;
}

int MultixRenderer::getDepthTest() const
{
    return depthTest;
}

float MultixRenderer::getIn() const
{
    return in;
}

int MultixRenderer::getLoopMode() const
{
    return loopMode;
}

int MultixRenderer::getMinmaxBlend() const
{
    return minmaxBlend;
}

float MultixRenderer::getOffset() const
{
    return xOffset;
}

int MultixRenderer::getPrevNumHeaders() const
{
    return prevNumHeaders;
}

float MultixRenderer::getRotSpeed() const
{
    return yRotSpeed;
}

float MultixRenderer::getRotZOffset() const
{
    return rotZOffset;
}

float MultixRenderer::getRotationZ() const
{
    return rotationZ;
}

float MultixRenderer::getScale() const
{
    return scale;
}

float MultixRenderer::getSpeedOffset() const
{
    return speedOffset;
}

int MultixRenderer::getTintR() const
{
    return tintR;
}

float MultixRenderer::getTotYRot() const
{
    return totYRot;
}

VideoBuffer *MultixRenderer::getVideoBuffer() const
{
    return videoBuffer;
}

float MultixRenderer::getX() const
{
    return x;
}

void MultixRenderer::setDepthTest(int depthTest)
{
    this->depthTest = depthTest;
}

void MultixRenderer::setIn(float in)
{
    this->in = in;
}

void MultixRenderer::setLoopMode(int loopMode)
{
    this->loopMode = loopMode;
}

void MultixRenderer::setMinmaxBlend(int minmaxBlend)
{
    this->minmaxBlend = minmaxBlend;
}

void MultixRenderer::setXOffset(float offset)
{
    xOffset = offset;
}

void MultixRenderer::setYOffset(float offset)
{
    yOffset = offset;
}

void MultixRenderer::setPrevNumHeaders(int prevNumHeaders)
{
    this->prevNumHeaders = prevNumHeaders;
}

void MultixRenderer::setRotSpeed(float rotSpeed)
{
    yRotSpeed = rotSpeed;
}

void MultixRenderer::setRotZOffset(float rotZOffset)
{
    this->rotZOffset = rotZOffset;
}

void MultixRenderer::setTintR(int tintR)
{
    this->tintR = tintR;
}

void MultixRenderer::setTotYRot(float totYRot)
{
    this->totYRot = totYRot;
}

void MultixRenderer::setVideoBuffer(VideoBuffer *videoBuffer)
{
    this->videoBuffer = videoBuffer;
}

vector<VideoHeader*> MultixRenderer::getVideoHeaders(){
	return videoHeader;
}

vector<VideoRenderer*> MultixRenderer::getVideoRenderers(){
	return videoRenderer;
}

VideoHeader * MultixRenderer::getHeader(int header){
	if(prevNumHeaders){
		header = CLAMP(header,0,prevNumHeaders-1);
		return videoHeader[header];
	}else{
		return NULL;
	}

}

VideoRenderer * MultixRenderer::getRenderer(int renderer){
	if(prevNumHeaders){
		renderer = CLAMP(renderer,0,prevNumHeaders-1);
		return videoRenderer[renderer];
	}else{
		return NULL;
	}

}

void MultixRenderer::setAlpha(int alpha)
{
    this->alpha = alpha;
}

void MultixRenderer::setAnchorX(float anchorX)
{
    this->anchorX = anchorX;
}

void MultixRenderer::setDelayOffset(float delayOffset)
{
    this->delayOffset = delayOffset;
}

void MultixRenderer::setOffset(float offset)
{
    xOffset = offset;
}

void MultixRenderer::setRotationZ(float rotationZ)
{
    this->rotationZ = rotationZ;
}

void MultixRenderer::setScale(float scale)
{
    this->scale = scale;
}

void MultixRenderer::setSpeedOffset(float speedOffset)
{
    this->speedOffset = speedOffset;
}

void MultixRenderer::setX(float x)
{
    this->x = x;
}


