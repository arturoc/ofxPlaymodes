/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "MultixRenderer.h"

MultixRenderer::MultixRenderer(VideoBuffer * buffer, int numHeaders)
{
    this->numHeaders=numHeaders;
    videoBuffer=buffer;

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

MultixRenderer::~MultixRenderer()
{
    //dtor
}

void MultixRenderer::setNumHeaders(int numHeaders){
    int prevNumHeaders = this->numHeaders;
    if(numHeaders>prevNumHeaders){
        for(int i=0;i<numHeaders-prevNumHeaders;i++){
            videoHeader.push_back( new VideoHeader(videoBuffer) );
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
            videoHeader.push_back( new VideoHeader(videoBuffer) );
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


    videoRenderer[0]->rotationY=rotationY;
    //videoRenderer[0]->rotationX=rotationX;
    videoRenderer[0]->x=x;
    videoRenderer[0]->y=y;
    videoRenderer[0]->z=z;

    //videoRenderer[0]->speed=
    for(int i=currNumHeaders-1; i>=0; i--){
        videoHeader[i]->in=in;
        videoHeader[i]->out=out;
        videoHeader[i]->loopMode=loopMode;
        videoHeader[i]->delay=delayOffset*1000*i;
        videoHeader[i]->speed=videoHeader[0]->speed+speedOffset*i;

        videoRenderer[i]->alpha=alpha;
        videoRenderer[i]->anchorX=anchorX;
        videoRenderer[i]->anchorY=anchorY;
        videoRenderer[i]->rotationZ=rotationZ+rotZOffset*i;
        if(i>0){
            videoRenderer[i]->rotationY=videoRenderer[i-1]->rotationY;
            videoRenderer[i]->rotationX=videoRenderer[i-1]->rotationX;
        }
        videoRenderer[i]->scale=scale;
        videoRenderer[i]->tintR=tintR;
        videoRenderer[i]->tintG=tintG;
        videoRenderer[i]->tintB=tintB;


        videoRenderer[i]->x=videoRenderer[0]->x-xOffset*i;
        videoRenderer[i]->y=videoRenderer[0]->y-yOffset*i;

        if(i>0){
            videoRenderer[i]->z=videoRenderer[i-1]->z;
            //videoRenderer[i]->y=videoRenderer[i-1]->y;
        }
        videoRenderer[i]->minmaxBlend=minmaxBlend;
        //videoRenderer[i]->activateShader=activateShader;
    }
    totYRot+=yRotSpeed;
}

void MultixRenderer::draw(){
    if(depthTest)
        glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glTranslatef(CAPTURE_WIDTH/2+videoRenderer[0]->x,0,videoRenderer[0]->z+anchorZ);
    glRotatef(totYRot,0,1,0);
    glTranslatef(-CAPTURE_WIDTH/2-videoRenderer[0]->x,0,-videoRenderer[0]->z-anchorZ);
        for(int i =prevNumHeaders-1; i>=0; i--){
        glPushMatrix();
            glTranslatef(CAPTURE_WIDTH/2+videoRenderer[i]->x,0,videoRenderer[i]->z+anchorZ);
            glRotatef(rotYOffset*i,0,1,0);
            glTranslatef(-CAPTURE_WIDTH/2-videoRenderer[i]->x,0,-videoRenderer[i]->z-anchorZ);
            videoRenderer[i]->draw();
        glPopMatrix();
        }
    glPopMatrix();
    if(depthTest)
        glDisable(GL_DEPTH_TEST);

}
