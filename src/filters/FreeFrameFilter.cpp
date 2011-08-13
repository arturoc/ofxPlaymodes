/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "FreeFrameFilter.h"


//----------------------------------------------------------
FreeFrameFilter::FreeFrameFilter(VideoSource * source,int _w,int _h) {
    currentFrame=0;
	this->source=source;
	//newFrameEvent.init("PlayModes.FreeFrameFilter.newFrame");
	///
	//loadPlugin("./data/PeteKaleidascope.dll");
    w = _w;
    h = _h;
}

//----------------------------------------------------------
FreeFrameFilter::~FreeFrameFilter() {
	// TODO Auto-generated destructor stub
}

//----------------------------------------------------------
VideoFrame * FreeFrameFilter::getNextVideoFrame(){
    // para que los elementos del pipeline que no
    // utilizan eventos puedan recoger el frame
    return currentFrame;
}

//----------------------------------------------------------
int FreeFrameFilter::getFps(){
    return source->getFps();
}

//----------------------------------------------------------
void FreeFrameFilter::newVideoFrame(VideoFrame & frame){
    // aqui tienes que hacer el filtrado del frame
    // para que el frame no sea borrado mientras lo estamos utilizando:

    frame.retain();

    //// test de acceso directo a los pixels
        /*unsigned char * framePixels;// = new unsigned char[frame->w*frame->h*3];
        framePixels = frame.getVideoFrame();
        for(int i=0;i<(frame.w*frame.h*3);i++)
        {
            if(i%3==0)framePixels[i]=255;
        }*/

    // llamamos a procesar el frame por el FF
    ff->processFF(frame.getVideoFrame());
    //marcamos el frame con textura no actualizada para que se realice la carga en el siguiente update()

    frame.setTexAllocated(false);

    //una vez que hemos terminado con un frame lo liberamos para que pueda ser borrado
    //y copiamos a currentFrame el actual (para el acceso no por eventos)
    if(currentFrame)
        currentFrame->release();
    currentFrame=&frame;


    // y pasarlo al siguiente elemento del pipeline notificando el evento...
    newFrameEvent.notify(this,frame);




}
//----------------------------------------------------------
void FreeFrameFilter::loadPlugin(char * filename){
    ff = new ofFreeframe();
    ff->loadFF(filename,w,h);
    //ff->getNumParameters();
	source->addListener(this);
    printf("num Parameters %d",ff->numParameters);

}
//----------------------------------------------------------
void FreeFrameFilter::setFFParameter(int index,float value){
    if((index>=0)&&(index<ff->numParameters)) ff->setParameter(index,value);
    else printf("FreeFrameFiler Error :: setParameter [%d, %f] out of bounds!!\n",index,value);
}
