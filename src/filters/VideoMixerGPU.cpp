/*
 * VideoMixerGPU.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#include "VideoMixerGPU.h"
using Poco::ScopedLock;

#define STRINGIFY(A) #A

static string fragmentMixSrc = STRINGIFY(
		\#version 140\n
		\#extension GL_ARB_texture_rectangle : enable\n
        uniform sampler2DRect tex0;
        uniform sampler2DRect tex1;

        void main (void){
            vec2 pos = gl_FragCoord.xy;

            vec4 color = vec4(texture2DRect(tex0, pos)*0.5 + texture2DRect(tex1, pos)*0.5);

            gl_FragColor = color;
        }
        );

namespace ofxPm{
VideoMixerGPU::VideoMixerGPU()
:source1(0)
,source2(0)
,newFrame(false)
{
	// TODO Auto-generated constructor stub

}

VideoMixerGPU::~VideoMixerGPU() {
	// TODO Auto-generated destructor stub
}

void VideoMixerGPU::setup(VideoSource & _source1, VideoSource & _source2){
	source1 = &_source1;
	source2 = &_source2;
	front = VideoFrame::newVideoFrame(_source1.getNextVideoFrame());
	ofAddListener(source1->newFrameEvent,this,&VideoMixerGPU::newVideoFrame);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentMixSrc);
	shader.linkProgram();

}

VideoFrame VideoMixerGPU::getNextVideoFrame(){
	return front;
}

void VideoMixerGPU::newVideoFrame(VideoFrame & frame){
	if(source2->getNextVideoFrame()==NULL){
		front = VideoFrame::newVideoFrame(frame);
		ofNotifyEvent(newFrameEvent,front);
		return;
	}
	//front = VideoFrame::newVideoFrame(frame.getFboRef());


	front.getFboRef().begin();
	shader.begin();
	shader.setUniformTexture("tex0",frame.getTextureRef(),0);
	shader.setUniformTexture("tex1",source2->getNextVideoFrame().getTextureRef(),1);
	ofRect(0,0,frame.getWidth(),frame.getHeight());
	shader.end();
	front.getFboRef().end();
	front.setTextureOnly(true);

	ofNotifyEvent(newFrameEvent,front);
}

float VideoMixerGPU::getFps(){
	return source1->getFps();
}
}
