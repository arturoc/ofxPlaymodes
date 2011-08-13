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
        MultixRenderer(VideoBuffer * buffer, int numHeaders=10);
        virtual ~MultixRenderer();

        void    setNumHeaders(int numHeaders);
        int     getNumHeaders();


        /*void setScale(float scale);
        void setRotationZ(float rotZ);
        void setRotationX(float rotX);
        void setRotationY(float rotY);
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void setAnchorX(float anchorX);
        void setAnchorY(float anchorY);
        void setAlpha(int alpha);

        void setDelayOffset(int delayOffset);
        void setSpeedOffset(float speedOffset);*/

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

        void update();
        void draw();

        void sliderValueChanged(int f){
            cout<<f<<"\n";
            setNumHeaders(f);
        }

        vector<VideoHeader*> getVideoHeaders(){
            return videoHeader;
        }

        vector<VideoRenderer*> getVideoRenderers(){
            return videoRenderer;
        }

        VideoHeader * getHeader(int header){
            if(prevNumHeaders){
                header = CLAMP(header,0,prevNumHeaders-1);
                return videoHeader[header];
            }else{
                return NULL;
            }

        }

        VideoRenderer * getRenderer(int renderer){
            if(prevNumHeaders){
                renderer = CLAMP(renderer,0,prevNumHeaders-1);
                return videoRenderer[renderer];
            }else{
                return NULL;
            }

        }
    protected:
        vector<VideoHeader*>    videoHeader;
        vector<VideoRenderer*>  videoRenderer;
        VideoBuffer *           videoBuffer;

        int                     prevNumHeaders;

    private:
};

#endif // MULTIXRENDERER_H
