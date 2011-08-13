/*
 * VideoRenderer.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEORENDERER_H_
#define VIDEORENDERER_H_

#include "VideoSink.h"
#include "VideoSource.h"
#include "constants.h"
#include "ofShader.h"

class VideoRenderer: public VideoSink {
public:
	VideoRenderer(VideoSource * source);
	virtual ~VideoRenderer();

	void draw();


	float scale;
	float rotationZ, rotationX, rotationY;
	float x, y, z;
	float anchorX, anchorY;
	int alpha;
	int tintR, tintG, tintB;

	bool minmaxBlend;
	bool activateShader;

	void setDraw(bool isDrawing){
	    this->isDrawing = isDrawing;
	}

    void	setZ(const void * sender,float & Z){
		z=Z;
	}
	void	setX(const void * sender,float & X){
		x=X;
	}
	void	setY(const void * sender,float & Y){
		y=Y;
	}
	void	setRX(const void * sender,float & RX){
		rotationX=RX;
	}
	void	setRY(const void * sender,float & RY){
		rotationY=RY;
	}
	void	setRZ(const void * sender,float & RZ){
		rotationZ=RZ;
	}
private:
    void drawNextFrame();

    VideoSource * source;
    //ofTexture texture;
    //bool texAllocated;
    ofShader shader;

    bool isDrawing;
};

#endif /* VIDEORENDERER_H_ */
