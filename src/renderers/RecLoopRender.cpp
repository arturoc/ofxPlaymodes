/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "RecLoopRender.h"
ofEventArgs recLoopVoidArgs;
RecLoopRender::RecLoopRender(VideoSource * bufferSource, VideoSource * liveSource)
{
    this->buffer=bufferSource;
    this->live=liveSource;
    imageAllocated=false;
    image.setUseTexture(false);
}

RecLoopRender::~RecLoopRender()
{
    //dtor
}


void RecLoopRender::draw(){
    VideoFrame * bufferFrame = buffer->getNextVideoFrame();
    VideoFrame * liveFrame   = live->getNextVideoFrame();
    if(bufferFrame!=NULL && liveFrame!=NULL){
        if(!imageAllocated){
            image.allocate(liveFrame->w,liveFrame->h,OF_IMAGE_COLOR);
            imageAllocated=true;
        }

        if(!bufferFrame->isTexAllocated())
            bufferFrame->update(recLoopVoidArgs);
        if(!liveFrame->isTexAllocated())
            liveFrame->update(recLoopVoidArgs);

        ofEnableAlphaBlending();
        ofSetColor(tintR,tintG,tintB,alpha);
        if(minmaxBlend)
            glBlendEquationEXT(GL_MAX);
        else
            glBlendEquationEXT(GL_MIN);

        bufferFrame->getTexture()->draw(0,0);
        liveFrame->getTexture()->draw(0,0);

        image.grabScreen(0,0,liveFrame->w,liveFrame->h);

        //unsigned char * pixels = bufferFrame->getVideoFrame();

        //memcpy(pixels,image.getPixels(),liveFrame->w*liveFrame->h*3*sizeof(unsigned char));

        bufferFrame->getTexture()->loadData(image.getPixels(),liveFrame->w,liveFrame->h,GL_RGB);

        liveFrame->release();

        //bufferFrame->setTexAllocated(false);
        //bufferFrame->update();

        bufferFrame->release();

        ofDisableAlphaBlending();
        glBlendEquationEXT(GL_FUNC_ADD);

    }
}
