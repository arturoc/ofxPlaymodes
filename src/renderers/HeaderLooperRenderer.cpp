

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
		VideoFrame frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			frame.getTextureRef().draw(0,0);
		}
		vHeader.draw();
		
	}
	//------------------------------------------------------

	void HeaderLooperRenderer::draw(int x,int y,int w,int h)
	{
		
		
		VideoFrame frame = vHeader.getNextVideoFrame();
		if(frame!=NULL){
			
			ofSetColor(vHeader.getOpacity(),vHeader.getOpacity(),vHeader.getOpacity());
			frame.getTextureRef().draw(x,y,w,h);
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
