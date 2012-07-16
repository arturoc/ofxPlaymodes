/*
 * VideoRenderer.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoRenderer.h"


namespace ofxPm{
VideoRenderer::VideoRenderer(){
    source=NULL;
	minmaxBlend=false;
	shaderActive=false;

}

VideoRenderer::VideoRenderer(VideoSource & source) {
	setup(source);
}

VideoRenderer::~VideoRenderer() {
}


void VideoRenderer::setup(VideoSource & source){
    this->source=&source;

	minmaxBlend=false;
	shaderActive=false;
}

void VideoRenderer::draw(){
	if(shaderActive)
		shader.begin();
	if(tint.a<255){
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
		ofSetColor(tint);
		/// drawing the video render
		drawNextFrame();
		glBlendEquationEXT(GL_FUNC_ADD);
		ofDisableAlphaBlending();
	}else{
		ofSetColor(tint);
		drawNextFrame();
	}
	if(shaderActive)
		shader.end();

}

void VideoRenderer::drawNextFrame(){
    VideoFrame frame = source->getNextVideoFrame();
    if(frame!=NULL){
        frame.getTextureRef().draw(0,0);
    }
}

ofShader VideoRenderer::getShader() const
{
    return shader;
}

ofColor VideoRenderer::getTint() const
{
    return tint;
}

bool VideoRenderer::isMinmaxBlend() const
{
    return minmaxBlend;
}

bool VideoRenderer::isShaderActive() const
{
    return shaderActive;
}

void VideoRenderer::setMinmaxBlend(bool minmaxBlend)
{
    this->minmaxBlend = minmaxBlend;
}

void VideoRenderer::setShader(ofShader shader)
{
    this->shader = shader;
}

void VideoRenderer::setShaderActive(bool shaderActive)
{
    this->shaderActive = shaderActive;
}

void VideoRenderer::setTint(ofColor tint)
{
    this->tint = tint;
}


}



