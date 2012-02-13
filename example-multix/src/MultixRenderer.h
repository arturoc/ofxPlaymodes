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
	MultixRenderer(ofxPm::VideoBuffer & buffer, int numHeaders=10);

	virtual ~MultixRenderer();

	void setup(ofxPm::VideoBuffer & buffer, int numHeaders=10);

	void    setNumHeaders(int numHeaders);
	int     getNumHeaders();

	void update();
	void draw();

	void sliderValueChanged(int f){
		cout<<f<<"\n";
		setNumHeaders(f);
	}

	vector<ofxPm::VideoHeader*> getVideoHeaders();
	vector<ofxPm::VideoRenderer*> getVideoRenderers();

	ofxPm::VideoHeader * getHeader(int header);
	ofxPm::VideoRenderer * getRenderer(int renderer);
    float getDelayOffset() const;
    float getIn() const;
    int getLoopMode() const;
    float getOut() const;
    int getPrevNumHeaders() const;
    float getSpeedOffset() const;
    ofColor getTint() const;
    ofxPm::VideoBuffer *getVideoBuffer() const;
    vector<ofxPm::VideoHeader*> getVideoHeader() const;
    vector<ofxPm::VideoRenderer*> getVideoRenderer() const;
    bool isMinmaxBlend() const;
    void setDelayOffset(float delayOffset);
    void setIn(float in);
    void setLoopMode(int loopMode);
    void setMinmaxBlend(bool minmaxBlend);
    void setOut(float out);
    void setPrevNumHeaders(int prevNumHeaders);
    void setSpeedOffset(float speedOffset);
    void setTint(ofColor tint);
    void setVideoBuffer(ofxPm::VideoBuffer *videoBuffer);
    void setVideoHeader(vector<ofxPm::VideoHeader*> videoHeader);
    void setVideoRenderer(vector<ofxPm::VideoRenderer*> videoRenderer);

protected:
	vector<ofxPm::VideoHeader*>    videoHeader;
	vector<ofxPm::VideoRenderer*>  videoRenderer;
	ofxPm::VideoBuffer *           videoBuffer;

	int                     prevNumHeaders;

private:
	ofColor tint;

	bool minmaxBlend;

	float delayOffset;
	float speedOffset;

	float in;
	float out;
	int loopMode;

	float numHeaders;
};

#endif // MULTIXRENDERER_H
