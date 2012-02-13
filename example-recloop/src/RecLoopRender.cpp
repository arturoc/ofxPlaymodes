/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "RecLoopRender.h"
#include "VideoFrame.h"

using namespace ofxPm;

RecLoopRender::RecLoopRender(VideoSource & bufferSource, VideoSource & liveSource)
{
    setup(bufferSource,liveSource);
}

RecLoopRender::RecLoopRender(){
	buffer=NULL;
	live=NULL;
	imageAllocated=false;
	image.setUseTexture(false);
	stopped = false;
}

RecLoopRender::~RecLoopRender()
{
    //dtor
}

void RecLoopRender::setup(VideoSource & bufferSource, VideoSource & liveSource){
    buffer=&bufferSource;
    live=&liveSource;
    imageAllocated=false;
    image.setUseTexture(false);
    stopped = false;
}

void RecLoopRender::draw(){
    VideoFrame * bufferFrame = buffer->getNextVideoFrame();
    VideoFrame * liveFrame   = live->getNextVideoFrame();
    if(bufferFrame!=NULL && liveFrame!=NULL){
        if(!imageAllocated){
            image.allocate(liveFrame->getWidth(),liveFrame->getHeight(),OF_IMAGE_COLOR);
            imageAllocated=true;
        }

        ofEnableAlphaBlending();
        ofSetColor(tintR,tintG,tintB,alpha);
        if(minmaxBlend)
            glBlendEquationEXT(GL_MAX);
        else
            glBlendEquationEXT(GL_MIN);

        liveFrame->getTextureRef().draw(0,0);

        if(!stopped){
            bufferFrame->getTextureRef().draw(0,0);
			image.grabScreen(0,0,liveFrame->getWidth(),liveFrame->getHeight());
			bufferFrame->getTextureRef().loadData(image.getPixelsRef());
        }

        liveFrame->release();
        bufferFrame->release();

        ofDisableAlphaBlending();
        glBlendEquationEXT(GL_FUNC_ADD);

    }
}


void RecLoopRender::stop(){
	stopped = true;
}

void RecLoopRender::resume(){
	stopped = false;
}
