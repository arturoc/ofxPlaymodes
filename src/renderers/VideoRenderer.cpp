/*
 * VideoRenderer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoRenderer.h"

VideoRenderer::VideoRenderer(VideoSource * source) {
    this->source=source;
    //texAllocated=false;
    scale = 1;
	rotationZ = 0;
	rotationX = 0;
	rotationY = 0;
	x = 0;
	y = 0;
	z = 0;

	anchorX = CAPTURE_WIDTH/2;
	anchorY = CAPTURE_HEIGHT/2;

	tintR=255;
	tintG=255;
	tintB=255;
	alpha=255;

	minmaxBlend=false;
	activateShader=false;
	isDrawing=true;
	//shader.loadShader("");
}

VideoRenderer::~VideoRenderer() {
}

void VideoRenderer::draw(){
    if(isDrawing){
        if(activateShader)
            shader.begin();
        if(alpha<255){
            ofEnableAlphaBlending();

            if(minmaxBlend){
                glBlendEquationEXT(GL_MAX);

                //glBlendFuncSeparateEXT( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA );
                //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                //glBlendEquationSeparateEXT(GL_MAX,GL_ADD);
            }else{
                glBlendEquationEXT(GL_MIN);


                //glBlendFuncSeparateEXT( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA );
                //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                //glBlendEquationSeparateEXT(GL_MIN,GL_ADD);
            }
            ofSetColor(tintR,tintG,tintB,alpha);
            /// drawing the video render
            drawNextFrame();
            glBlendEquationEXT(GL_FUNC_ADD);
            ofDisableAlphaBlending();
        }else{
            ofSetColor(tintR,tintG,tintB);
            drawNextFrame();
        }
        if(activateShader)
            shader.end();
    }
}

void VideoRenderer::drawNextFrame(){
    VideoFrame * frame = source->getNextVideoFrame();
    if(frame!=NULL){
        glPushMatrix();
        glTranslatef(x,y,z);
        glTranslatef((anchorX*scale+x),(anchorY*scale+y),0);
        glRotatef(rotationZ,0,0,1);
        glRotatef(rotationX,1,0,0);
        glRotatef(rotationY,0,1,0);
        glTranslatef(-(anchorX*scale+x),-(anchorY*scale+y),0);
        frame->getTextureRef().draw(0,0,frame->getWidth()*scale,frame->getHeight()*scale);
        glPopMatrix();
        frame->release();
    }
}

int VideoRenderer::getAlpha() const
{
    return alpha;
}

float VideoRenderer::getAnchorX() const
{
    return anchorX;
}

float VideoRenderer::getRotationZ() const
{
    return rotationZ;
}

float VideoRenderer::getScale() const
{
    return scale;
}

ofShader VideoRenderer::getShader() const
{
    return shader;
}

int VideoRenderer::getTintR() const
{
    return tintR;
}

float VideoRenderer::getX() const
{
    return x;
}

bool VideoRenderer::isActivateShader() const
{
    return activateShader;
}

bool VideoRenderer::isIsDrawing() const
{
    return isDrawing;
}

bool VideoRenderer::isMinmaxBlend() const
{
    return minmaxBlend;
}

void VideoRenderer::setActivateShader(bool activateShader)
{
    this->activateShader = activateShader;
}

void VideoRenderer::setAlpha(int alpha)
{
    this->alpha = alpha;
}

void VideoRenderer::setAnchorX(float anchorX)
{
    this->anchorX = anchorX;
}

void VideoRenderer::setIsDrawing(bool isDrawing)
{
    this->isDrawing = isDrawing;
}

void VideoRenderer::setMinmaxBlend(bool minmaxBlend)
{
    this->minmaxBlend = minmaxBlend;
}

void VideoRenderer::setRotationZ(float rotationZ)
{
    this->rotationZ = rotationZ;
}

void VideoRenderer::setScale(float scale)
{
    this->scale = scale;
}

void VideoRenderer::setShader(ofShader shader)
{
    this->shader = shader;
}

void VideoRenderer::setTintR(int tintR)
{
    this->tintR = tintR;
}

void VideoRenderer::setX(float x)
{
    this->x = x;
}

float VideoRenderer::getAnchorY() const
{
    return anchorY;
}

float VideoRenderer::getRotationX() const
{
    return rotationX;
}

float VideoRenderer::getRotationY() const
{
    return rotationY;
}

int VideoRenderer::getTintB() const
{
    return tintB;
}

int VideoRenderer::getTintG() const
{
    return tintG;
}

float VideoRenderer::getY() const
{
    return y;
}

float VideoRenderer::getZ() const
{
    return z;
}

void VideoRenderer::setAnchorY(float anchorY)
{
    this->anchorY = anchorY;
}

void VideoRenderer::setRotationX(float rotationX)
{
    this->rotationX = rotationX;
}

void VideoRenderer::setRotationY(float rotationY)
{
    this->rotationY = rotationY;
}

void VideoRenderer::setTintB(int tintB)
{
    this->tintB = tintB;
}

void VideoRenderer::setTintG(int tintG)
{
    this->tintG = tintG;
}

void VideoRenderer::setY(float y)
{
    this->y = y;
}

void VideoRenderer::setZ(float z)
{
    this->z = z;
}




