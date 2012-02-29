

#include "HeaderLooperRenderer.h"
#include "VideoFrame.h"

//------------------------------------------------------

namespace ofxPm
{
	HeaderLooperRenderer::HeaderLooperRenderer(VideoBuffer & buffer) 
	{
		this->vBuffer = &buffer;
		vHeader.setup(*vBuffer);
	}

	//------------------------------------------------------

	HeaderLooperRenderer::HeaderLooperRenderer(){
		vBuffer = NULL;
	}
	//------------------------------------------------------

	void HeaderLooperRenderer::setup(VideoBuffer & buffer){
		this->vBuffer = &buffer;
		vHeader.setup(*vBuffer);
	}
	//------------------------------------------------------

	HeaderLooperRenderer::~HeaderLooperRenderer() {
		// TODO Auto-generated destructor stub
	}

	//------------------------------------------------------

	void HeaderLooperRenderer::draw()
	{
		VideoFrame * frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			frame->getTextureRef().draw(0,0);
			frame->release();
		}
		vHeader.draw();
		
	}
	//------------------------------------------------------

	void HeaderLooperRenderer::draw(int x,int y,int w,int h)
	{
		VideoFrame * frame = vHeader.getNextVideoFrame();
		//VideoFrame * frame2;
		//if(vBuffer->size()>174) 
		//	  frame2= vHeader.getVideoFrame(0);;
		if(frame!=NULL){
			
//			ofEnableBlendMode(OF_BLENDMODE_ADD);
//			ofSetColor(128,128,128);
//			if(vBuffer->size()>174)frame2->getTextureRef().draw(x,y,w,h);
			
			ofSetColor(vHeader.getOpacity(),vHeader.getOpacity(),vHeader.getOpacity());
			frame->getTextureRef().draw(x,y,w,h);
			frame->release();
//			if(vBuffer->size()>174)frame2->release();
			//ofDisableBlendMode();
		}
		vHeader.draw();
		ofSetColor(255,255,255);
		
	}

	//------------------------------------------------------

	VideoHeader* HeaderLooperRenderer::getHeader()
	{
		return &vHeader;
	}
		
	//------------------------------------------------------
	
} //endNameSpace
