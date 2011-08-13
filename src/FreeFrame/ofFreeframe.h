#ifndef _OF_FREEFRAME
#define _OF_FREEFRAME

#include "FreeFrame.h"
#include "ofMain.h"

#ifdef TARGET_LINUX
    #include <dlfcn.h>
#endif
typedef struct{

	float value;
	float defaultValue;
	char* displayValue;	// this is 16 char name + null terminator
	char* name;			// this is 16 char name + null terminator

} Parameter;


class ofFreeframe{

	public:

	bool loadFF(char * filename,int W,int H);
	DWORD processFF(unsigned char* pFrame);
    void  closeFF();

    //-----------------------------------------------------------------------
    // www.freeframe.org

  	DWORD instance;
    PlugInfoStruct *pPlugInfo;
	plugMainType * plugMain;

	#ifdef TARGET_WIN32
        HINSTANCE hinstLib;
    #else
        void* hinstLib;
    #endif

	DWORD numParameters;
	Parameter ** parameters;

	bool isInitialised;
	bool isInstantiated;

	void getNumParameters();
	void setParameter(DWORD index, float value);
	void getParameterDisplays();
	void getParameterDisplay(DWORD index);
	void getParameter(DWORD index);
	void getParameters();
	void getParameterDefaults();
	void getParameterDefault( DWORD index);
	void getParameterNames();
	void getParameterName(DWORD);

    ofFreeframe();
    ~ofFreeframe();

	DWORD initialise();
	DWORD instantiate(VideoInfoStruct* videoInfo);
	DWORD processFrame(unsigned char* pFrame);
    void deInitialise();
	DWORD deInstantiate();
	void getPlugInfo();
	void unloadPlugin();
	void loadPlugin(char* pluginFile);

};//end class

#endif
