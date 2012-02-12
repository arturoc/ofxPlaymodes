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

	void sliderValueChanged(int f){
		cout<<f<<"\n";
		setNumHeaders(f);
	}

	vector<VideoHeader*> getVideoHeaders();
	vector<VideoRenderer*> getVideoRenderers();

	VideoHeader * getHeader(int header);
	VideoRenderer * getRenderer(int renderer);
    float getDelayOffset() const;
    float getIn() const;
    int getLoopMode() const;
    float getOut() const;
    int getPrevNumHeaders() const;
    float getSpeedOffset() const;
    ofColor getTint() const;
    VideoBuffer *getVideoBuffer() const;
    vector<VideoHeader*> getVideoHeader() const;
    vector<VideoRenderer*> getVideoRenderer() const;
    bool isMinmaxBlend() const;
    void setDelayOffset(float delayOffset);
    void setIn(float in);
    void setLoopMode(int loopMode);
    void setMinmaxBlend(bool minmaxBlend);
    void setOut(float out);
    void setPrevNumHeaders(int prevNumHeaders);
    void setSpeedOffset(float speedOffset);
    void setTint(ofColor tint);
    void setVideoBuffer(VideoBuffer *videoBuffer);
    void setVideoHeader(vector<VideoHeader*> videoHeader);
    void setVideoRenderer(vector<VideoRenderer*> videoRenderer);

protected:
	vector<VideoHeader*>    videoHeader;
	vector<VideoRenderer*>  videoRenderer;
	VideoBuffer *           videoBuffer;

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
