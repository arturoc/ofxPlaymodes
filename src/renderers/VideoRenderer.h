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
    int getAlpha() const;
    float getAnchorX() const;
    float getRotationZ() const;
    float getScale() const;
    ofShader getShader() const;
    int getTintR() const;
    float getX() const;
    bool isActivateShader() const;
    bool isIsDrawing() const;
    bool isMinmaxBlend() const;
    void setActivateShader(bool activateShader);
    void setAlpha(int alpha);
    void setAnchorX(float anchorX);
    void setIsDrawing(bool isDrawing);
    void setMinmaxBlend(bool minmaxBlend);
    void setRotationZ(float rotationZ);
    void setScale(float scale);
    void setShader(ofShader shader);
    void setTintR(int tintR);
    void setX(float x);
    float getAnchorY() const;
    float getRotationX() const;
    float getRotationY() const;
    int getTintB() const;
    int getTintG() const;
    float getY() const;
    float getZ() const;
    void setAnchorY(float anchorY);
    void setRotationX(float rotationX);
    void setRotationY(float rotationY);
    void setTintB(int tintB);
    void setTintG(int tintG);
    void setY(float y);
    void setZ(float z);
    void setDraw(bool isDrawing)
    {
        this->isDrawing = isDrawing;
    }

private:
    void drawNextFrame();
    VideoSource *source;
    ofShader shader;

    bool isDrawing;


	float scale;
	float rotationZ;
	float rotationX;
	float rotationY;
	float x;
	float y;
	float z;
	float anchorX;
	float anchorY;
	int alpha;
	int tintR;
	int tintG;
	int tintB;

	bool minmaxBlend;
	bool activateShader;
};

#endif /* VIDEORENDERER_H_ */
