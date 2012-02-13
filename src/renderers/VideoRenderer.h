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
#include "ofShader.h"


namespace ofxPm{
class VideoRenderer: public VideoSink {
public:
	VideoRenderer(VideoSource * source);
	virtual ~VideoRenderer();

	void draw();

    ofShader getShader() const;
    ofColor getTint() const;
    bool isMinmaxBlend() const;
    bool isShaderActive() const;

    void setMinmaxBlend(bool minmaxBlend);
    void setShader(ofShader shader);
    void setShaderActive(bool shaderActive);
    void setTint(ofColor tint);

private:
    void drawNextFrame();
    VideoSource *source;
    ofShader shader;

    ofColor tint;

	bool minmaxBlend;
	bool shaderActive;
};
}

#endif /* VIDEORENDERER_H_ */
