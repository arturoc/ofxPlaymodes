#include "ofFreeframe.h"
using namespace std;


//------------------------------------------------------------------------------

bool ofFreeframe::loadFF(char * filename,int W, int H)
{
  loadPlugin(filename);
  VideoInfoStruct* videoInfo = new VideoInfoStruct();
  videoInfo->frameWidth = (DWORD)W;
  videoInfo->frameHeight = (DWORD)H;
  videoInfo->bitDepth = (DWORD)1;
  videoInfo->orientation = (DWORD)1;
  initialise();
  DWORD res = instantiate(videoInfo);

  if (res != FF_FAIL)
  {
    getPlugInfo();
    getNumParameters();
    printf("ofFreeFrame// Plugin loaded :: %s :: numParams %d\n",pPlugInfo->pluginName,numParameters);
    return(0);
  }
  else
  {
    printf("ofFreeFrame// Plugin load ERROR!! :: %s :: numParams %d\n",pPlugInfo->pluginName,numParameters);
    return(-1);
  }
}
//------------------------------------------------------------------------------
DWORD ofFreeframe::processFF(unsigned char* myPixels)
{
    return(processFrame(myPixels));
}

//------------------------------------------------------------------------------
//void  ofFreeFrame::closeFFPlugin(){
//}

//------------------------------------------------------------------------------
// Freeframe Host Construction/Destruction
//------------------------------------------------------------------------------

ofFreeframe::ofFreeframe()
{
	hinstLib = 0;
	plugMain = 0;
	pPlugInfo = 0;
	isInitialised = false;
	numParameters = 0;
	parameters = 0;

}

ofFreeframe::~ofFreeframe()
{
	if (hinstLib) {
		unloadPlugin();
	}
	if (parameters) {
	// de-allocate memory for parameternames strings
		for (DWORD n=0; n<numParameters; n++) {
			delete parameters[n];
		}
		//de-allocate memory for parameternames array
		delete parameters;
	}
}
//------------------------------------------------------------------------------
// load & unload plugin
//------------------------------------------------------------------------------

void ofFreeframe::loadPlugin(char* pluginFile)
{
#ifdef TARGET_WIN32
   BOOL fRunTimeLinkSuccess = FALSE;
    hinstLib = LoadLibraryA(pluginFile);
   // If the handle is valid, try to get the function address.
    if (hinstLib != NULL)
    {
        plugMain = (FF_Main_FuncPtr) GetProcAddress(hinstLib, "plugMain");
    }
	else printf("ofFreeFrame :: Couldn't load Plugin\n");
#else
    hinstLib = dlopen(pluginFile, RTLD_NOW);
    if (hinstLib != NULL){
        plugMain=(plugMainType*)dlsym(hinstLib, "plugMain");
    }
	else printf("ofFreeFrame :: Couldn't load Plugin\n");
#endif
}

void ofFreeframe::unloadPlugin()
{
	//De-initialise if not already done
	if (isInstantiated) {
		deInstantiate();
		isInstantiated = 0;
	}
	if (isInitialised) {
		deInitialise();
		isInitialised = 0;
	}
  // free the DLL module
  BOOL fFreeResult;
  if (hinstLib) {
      #ifdef TARGET_WIN32
        fFreeResult = FreeLibrary(hinstLib);
      #else
        if (!dlclose(hinstLib))
        {
          hinstLib = NULL;
        }
        else
        {
          printf("couldn't free dynamic library\n");
        }
      #endif
	  hinstLib = 0;
  }
}

//------------------------------------------------------------------------------

void ofFreeframe::getPlugInfo()
{
     /*
// plugMain function return values
typedef union plugMainUnionTag {
	DWORD ivalue;
	float fvalue;
	VideoInfoStruct* VISvalue;
	PluginInfoStruct* PISvalue;
	char* svalue;
} plugMainUnion;
     */
 //pPlugInfo = (PlugInfoStruct*)(plugMain)(FF_GETINFO, 0, 0);
 plugMainUnion pmu;
 pmu = (plugMain)(FF_GETINFO, 0, 0);
 pPlugInfo = (PlugInfoStruct*)pmu.PISvalue;
}
//------------------------------------------------------------------------------

DWORD ofFreeframe::initialise()
{
	isInitialised = true;
    //	return (DWORD)(plugMain)(FF_INITIALISE, (void*)videoInfo, 0);
    plugMainUnion pmu;
    pmu =(plugMain)(FF_INITIALISE,0, 0);
	return (DWORD)pmu.ivalue;
}
//------------------------------------------------------------------------------

DWORD ofFreeframe::instantiate(VideoInfoStruct* videoInfo)
{
//	DWORD res = (DWORD)(plugMain)(FF_INSTANTIATE, (void*)videoInfo, 0);
    plugMainUnion pmu;
    pmu.ivalue=DWORD(100);
    pmu = (plugMain)(FF_INSTANTIATE,videoInfo, 0);
    DWORD res = pmu.ivalue;

	if (res != FF_FAIL) {
		isInstantiated = true;
		instance = res;
		return FF_SUCCESS;
	}
	return FF_FAIL;
}
//------------------------------------------------------------------------------

DWORD ofFreeframe::deInstantiate()
{
	if (isInstantiated) {
//		DWORD res = (DWORD)(plugMain)(FF_DEINSTANTIATE, 0, instance);
        plugMainUnion pmu;
        pmu = (plugMain)(FF_DEINSTANTIATE,0,instance);
        DWORD res = pmu.ivalue;

		if (res != FF_FAIL) {
			instance = 0;
			isInstantiated = false;
		}
	} else {
		return FF_FAIL;
	}
	return FF_SUCCESS;
}

//------------------------------------------------------------------------------

DWORD ofFreeframe::processFrame(unsigned char* pFrame)
{
    plugMainUnion pmu;
    pmu = (plugMain)(FF_PROCESSFRAME,pFrame,instance);

    return pmu.ivalue;

//	return (DWORD)(plugMain)(FF_PROCESSFRAME, pFrame, instance);

}

void ofFreeframe::deInitialise()
{
	if (isInitialised){
		isInitialised = false;
		(plugMain)(FF_DEINITIALISE, 0, 0);
	}
}

//------------------------------------------------------------------------------

void ofFreeframe::getNumParameters()
{
//	numParameters = (DWORD)(plugMain)(FF_GETNUMPARAMETERS, 0, 0);
    plugMainUnion pmu;
    pmu = (plugMain)(FF_GETNUMPARAMETERS,0,0);
    numParameters = pmu.ivalue;

	//allocate memory for the parameterNames array
	parameters = new Parameter* [numParameters];
	for (DWORD n=0; n<numParameters; n++) {
		parameters[n] = new Parameter;
		parameters[n]->value = 0.0;
		parameters[n]->defaultValue = 0.0;
		parameters[n]->displayValue = new char[17];
		parameters[n]->name = new char[17];
	}
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameterName(DWORD param)
{
    plugMainUnion pmu;
    pmu = (plugMain)(FF_GETPARAMETERNAME,(void*)param,0);
//!    char *pName = pmu.svalue;

       char *pName = new char[16];
       pName = pmu.svalue;
//!

//    char *pName = (char *)(plugMain)(FF_GETPARAMETERNAME, (LPVOID)param, 0);

	memcpy(parameters[param]->name, pName, 16);
}


//------------------------------------------------------------------------------
void ofFreeframe::getParameterNames()
{
	for (DWORD n=0; n<numParameters; n++) {
		getParameterName(n);
	}
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameterDefault(DWORD param)
{
    plugMainUnion pmu;
    pmu = (plugMain)(FF_GETPARAMETERDEFAULT, (void*)param, 0);
    parameters[param]->defaultValue = pmu.fvalue;

//    parameters[param]->defaultValue = (float)(DWORD)(plugMain)(FF_GETPARAMETERDEFAULT, (LPVOID)param, 0);
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameterDefaults()
{
	for (DWORD n=0; n<numParameters; n++) {
		getParameterDefault(n);
	}
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameter(DWORD index)
{
    plugMainUnion pmu;
	pmu = (plugMain)(FF_GETPARAMETER, (void*)index, instance);
//	uvalue.pvalue = (plugMain)(FF_GETPARAMETER, (DWORD)index, instance);
	parameters[index]->value = pmu.fvalue;
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameters()
{
	for (DWORD n=0; n<numParameters; n++) {
		getParameter(n);
	}
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameterDisplay(DWORD index)
{
    plugMainUnion pmu;
    pmu = (plugMain)(FF_GETPARAMETERDISPLAY, (void*)index, instance);
//!    char *pDisplay =  pmu.svalue;
    char *pDisplay = new char[16];
    pDisplay = pmu.svalue;
//!


//	char *pDisplay = (char *)(plugMain)(FF_GETPARAMETERDISPLAY, (LPVOID)index, instance);

    memcpy(parameters[index]->displayValue, pDisplay, 16);
}

//------------------------------------------------------------------------------
void ofFreeframe::getParameterDisplays()
{
	for (DWORD n=0; n<numParameters; n++) {
		getParameterDisplay(n);
	}
}

//------------------------------------------------------------------------------
void ofFreeframe::setParameter(DWORD index, float value)
{
    float *ff = new float(value);

	if (index<numParameters) {
//! 		SetParameterStruct* paramStruct = new SetParameterStruct();


		SetParameterStruct* paramStruct;
        paramStruct = (SetParameterStruct*)malloc(sizeof(SetParameterStruct));
        paramStruct = new SetParameterStruct();

//!
		paramStruct->index = index;
        memcpy(&(paramStruct->value),ff,sizeof(float));

        plugMainUnion pmu;
        pmu = (plugMain)(FF_SETPARAMETER, paramStruct, instance);
        if(pmu.ivalue!=FF_SUCCESS) {printf("FF setParameter error\n");}

        //pmu = (plugMain)(FF_GETPARAMETER,int(0),instance );


//		if ((plugMain)(FF_SETPARAMETER, (DWORD)&paramStruct, instance) != FF_SUCCESS) printf("FF setParameter error\n");
//		LPVOID test = (plugMain)(FF_GETPARAMETER, (LPVOID)index, 0);
	}
}



