/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef MULTIXRENDERER_H
#define MULTIXRENDERER_H

#include "VideoBuffer.h"
#include "VideoHeader.h"
#include "VideoRenderer.h"

#include "ofMain.h"


class MultixRenderer
{
public:
	MultixRenderer();
	MultixRenderer(VideoBuffer & buffer, int numHeaders=10);

	virtual ~MultixRenderer();

	void setup(VideoBuffer & buffer, int numHeaders=10);

	void    setNumHeaders(int numHeaders);
	int     getNumHeaders();

	void update();
	void draw();

    int getAlpha() const;
    float getAnchorX() const;
    float getDelayOffset() const;
    int getDepthTest() const;
    float getIn() const;
    int getLoopMode() const;
    int getMinmaxBlend() const;
    float getOffset() const;
    int getPrevNumHeaders() const;
    float getRotSpeed() const;
    float getRotZOffset() const;
    float getRotationZ() const;
    float getScale() const;
    float getSpeedOffset() const;
    int getTintR() const;
    float getTotYRot() const;
    VideoBuffer *getVideoBuffer() const;
    float getX() const;

    void setDepthTest(int depthTest);
    void setIn(float in);
    void setLoopMode(int loopMode);
    void setMinmaxBlend(int minmaxBlend);
    void setXOffset(float offset);
    void setYOffset(float offset);
    void setPrevNumHeaders(int prevNumHeaders);
    void setRotSpeed(float rotSpeed);
    void setRotZOffset(float rotZOffset);
    void setTintR(int tintR);
    void setTotYRot(float totYRot);
    void setVideoBuffer(VideoBuffer *videoBuffer);

	void sliderValueChanged(int f){
		cout<<f<<"\n";
		setNumHeaders(f);
	}

	vector<VideoHeader*> getVideoHeaders();
	vector<VideoRenderer*> getVideoRenderers();

	VideoHeader * getHeader(int header);
	VideoRenderer * getRenderer(int renderer);
    void setAlpha(int alpha);
    void setAnchorX(float anchorX);
    void setDelayOffset(float delayOffset);
    void setOffset(float offset);
    void setRotationZ(float rotationZ);
    void setScale(float scale);
    void setSpeedOffset(float speedOffset);
    void setX(float x);

protected:
	vector<VideoHeader*>    videoHeader;
	vector<VideoRenderer*>  videoRenderer;
	VideoBuffer *           videoBuffer;

	int                     prevNumHeaders;

private:
	float scale;
	float rotationZ, rotationX, rotationY;
	float rotZOffset,rotYOffset;
	float x, y, z;
	float xOffset;
	float yOffset;
	float anchorX, anchorY, anchorZ;
	int alpha;
	int tintR, tintG, tintB;

	int depthTest;

	int minmaxBlend;

	float delayOffset;
	float speedOffset;

	float in, out;
	int loopMode;


	float yRotSpeed;
	float totYRot;

	float                     numHeaders;
};

#endif // MULTIXRENDERER_H
