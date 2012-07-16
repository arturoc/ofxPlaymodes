/*
 * BasicVideoRenderer.cpp
 *
 *  Created on: 11/02/2012
 *      Author: arturo
 */

#include "BasicVideoRenderer.h"
#include "VideoFrame.h"

namespace ofxPm{
BasicVideoRenderer::BasicVideoRenderer(VideoSource & source) {
	this->source = &source;
}

BasicVideoRenderer::BasicVideoRenderer(){
	source = NULL;
}

void BasicVideoRenderer::setup(VideoSource & source){
	this->source = &source;
}

BasicVideoRenderer::~BasicVideoRenderer() {
	// TODO Auto-generated destructor stub
}

void BasicVideoRenderer::draw(){
	VideoFrame frame = source->getNextVideoFrame();
	if(frame!=NULL){
		frame.getTextureRef().draw(0,0);
	}
}
	
void BasicVideoRenderer::draw(int x,int y,int w,int h){
	VideoFrame frame = source->getNextVideoFrame();
	if(frame!=NULL){
		frame.getTextureRef().draw(x,y,w,h);
	}
}
	

	
}
