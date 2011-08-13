/*
 * VideoRenderer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoRenderer.h"
ofEventArgs videoRendererVoidArgs;

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
            shader.setShaderActive(true);
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
            ofDisableAlphaBlending();
            glBlendEquationEXT(GL_FUNC_ADD);
        }else{
            ofSetColor(tintR,tintG,tintB);
            drawNextFrame();
        }
        if(activateShader)
            shader.setShaderActive(false);
    }
}

void VideoRenderer::drawNextFrame(){
    VideoFrame * frame = source->getNextVideoFrame();
    if(frame!=NULL){
        if(!frame->isTexAllocated())
            frame->update(videoRendererVoidArgs);
        glPushMatrix();
        glTranslatef(x,y,z);
        glTranslatef((anchorX*scale+x),(anchorY*scale+y),0);
        glRotatef(rotationZ,0,0,1);
        glRotatef(rotationX,1,0,0);
        glRotatef(rotationY,0,1,0);
        glTranslatef(-(anchorX*scale+x),-(anchorY*scale+y),0);
        frame->getTexture()->draw(0,0,frame->w*scale,frame->h*scale);
        glPopMatrix();
        frame->release();
    }
}
