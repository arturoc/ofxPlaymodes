
#include "videoHeaderRenderer.h"
#include "VideoFrame.h"

using namespace ofxPm;



	//------------------------------------------------------
	videoHeaderRenderer::videoHeaderRenderer(){
		vBuffer = NULL;
	}

	//------------------------------------------------------
	void videoHeaderRenderer::setup(VideoBuffer &_buffer){
		this->vBuffer = &_buffer;
		vHeader.setup(*vBuffer);
	}

	//------------------------------------------------------
	videoHeaderRenderer::~videoHeaderRenderer() {
		// TODO Auto-generated destructor stub
	}

	//------------------------------------------------------
	void videoHeaderRenderer::draw(){
		VideoFrame * frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			frame->getTextureRef().draw(0,0);
			frame->release();
		}
		vHeader.draw();
	}
		
	//------------------------------------------------------
	void videoHeaderRenderer::draw(int x,int y,int w,int h){
		VideoFrame * frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			frame->getTextureRef().draw(x,y,w,h);
			frame->release();
		}
		vHeader.draw();

	}
	//------------------------------------------------------
	VideoHeader videoHeaderRenderer::getHeader()
	{
		return vHeader;
	}


	

