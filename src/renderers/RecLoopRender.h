#ifndef RECLOOPRENDER_H
#define RECLOOPRENDER_H

#include <VideoSink.h>
#include <VideoSource.h>


class RecLoopRender : public VideoSink
{
    public:
        RecLoopRender(VideoSource * bufferSource, VideoSource * liveSource);
        virtual ~RecLoopRender();
        void draw();

		void stop();
		void resume();

        int alpha;
        int tintR, tintG, tintB;
        bool minmaxBlend;
    protected:
    private:
        VideoSource * buffer;
        VideoSource * live;
        ofImage       image;
        bool          imageAllocated;
        bool		  stopped;
};

#endif // RECLOOPRENDER_H
