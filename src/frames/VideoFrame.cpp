/*
 * VideoFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoFrame.h"

int VideoFrame::total_num_frames=0;

VideoFrame::VideoFrame(unsigned char * videoFrame, int w, int h) {
    texture = new ofTexture();
	pixels=new unsigned char[w*h*3];
	memcpy(pixels,videoFrame,sizeof(unsigned char)*w*h*3);
	this->w=w;
	this->h=h;
	total_num_frames++;
    ofAddListener(ofEvents.update,this,&VideoFrame::update);
	texAllocated=false;
	doRelease=false;
	//cout << "total num frames: " << total_num_frames <<"\n";

}

VideoFrame::~VideoFrame() {
    total_num_frames--;
    delete texture;
	delete[] pixels;
}

void VideoFrame::update(ofEventArgs & args){
	ofRemoveListener(ofEvents.update,this,&VideoFrame::update);
    if(!texAllocated){
        texture->allocate(w,h,GL_RGB);
        texture->loadData(pixels,w,h,GL_RGB);
        texAllocated=true;
    }
    if(doRelease){
        delete this;
    }
}

void VideoFrame::release() {
    ScopedLock<FastMutex>(*mutex);
	_useCountOfThisObject--;
	//printf("ofxObjCPointer::release() - Use Count: %i\n", _useCountOfThisObject);
	if(_useCountOfThisObject == 0) {
		//printf("    deleting object\n");
		doRelease=true;
        ofAddListener(ofEvents.update,this,&VideoFrame::update);
	}
}
unsigned char * VideoFrame::getVideoFrame(){
    return  pixels;
}

ofTexture * VideoFrame::getTexture(){
    return texture;
}

bool VideoFrame::isTexAllocated(){
    return texAllocated;
}

void VideoFrame::setTexAllocated(bool isAllocated){
    texAllocated=isAllocated;
    /*if(!isAllocated)
        ofEvents::addUpdateListener(this);*/
}
