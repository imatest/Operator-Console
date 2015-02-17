//
// MATLAB Compiler: 4.18.1 (R2013a)
// Date: Wed Feb 11 01:48:39 2015
// Arguments: "-B" "macro_default" "-B" "cpplib:imatest_acquisition" "-W"
// "cpplib:imatest_acquisition" "-T" "link:lib" "-d"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\build\imatest_it\final\libs\acqui
// sition\cpp" "-I" "C:\Program Files
// (x86)\MATLAB\R2013a\toolbox\images\images" "-I" "C:\Program Files
// (x86)\MATLAB\R2013a\toolbox\images\iptutils" "-I" "C:\Program Files
// (x86)\MATLAB\R2013a\toolbox\images\imuitools" "-a"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\RTDepends" "-a"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ToolboxJava" "-a"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ToolboxMex"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ImatestToolbox\acquire_im
// age.m" "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\list_devices.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ImatestToolbox\acquisitio
// n\stop_devware.m" 
//

#include <stdio.h>
#define EXPORTING_imatest_acquisition 1
#include "imatest_acquisition.h"

static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#include <windows.h>

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (GetModuleFileName(hInstance, path_to_dll, _MAX_PATH) == 0)
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_imatest_acquisition_C_API
#define LIB_imatest_acquisition_C_API /* No special import/export declaration */
#endif

LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV imatest_acquisitionInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!GetModuleFileName(GetModuleHandle("imatest_acquisition"), path_to_dll, _MAX_PATH))
    return false;
    {
        mclCtfStream ctfStream = 
            mclGetEmbeddedCtfStream(path_to_dll);
        if (ctfStream) {
            bResult = mclInitializeComponentInstanceEmbedded(   &_mcr_inst,
                                                                error_handler, 
                                                                print_handler,
                                                                ctfStream);
            mclDestroyStream(ctfStream);
        } else {
            bResult = 0;
        }
    }  
    if (!bResult)
    return false;
  return true;
}

LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV imatest_acquisitionInitialize(void)
{
  return imatest_acquisitionInitializeWithHandlers(mclDefaultErrorHandler, 
                                                   mclDefaultPrintHandler);
}

LIB_imatest_acquisition_C_API 
void MW_CALL_CONV imatest_acquisitionTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_imatest_acquisition_C_API 
void MW_CALL_CONV imatest_acquisitionPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(&stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV mlxAcquire_image(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "acquire_image", nlhs, plhs, nrhs, prhs);
}

LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV mlxList_devices(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "list_devices", nlhs, plhs, nrhs, prhs);
}

LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV mlxStop_devware(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "stop_devware", nlhs, plhs, nrhs, prhs);
}

LIB_imatest_acquisition_CPP_API 
void MW_CALL_CONV acquire_image(int nargout, mwArray& im_orig, mwArray& vstr, const 
                                mwArray& source_id, const mwArray& varargin)
{
  mclcppMlfFeval(_mcr_inst, "acquire_image", nargout, 2, -2, &im_orig, &vstr, &source_id, &varargin);
}

LIB_imatest_acquisition_CPP_API 
void MW_CALL_CONV list_devices(int nargout, mwArray& devices)
{
  mclcppMlfFeval(_mcr_inst, "list_devices", nargout, 1, 0, &devices);
}

LIB_imatest_acquisition_CPP_API 
void MW_CALL_CONV stop_devware()
{
  mclcppMlfFeval(_mcr_inst, "stop_devware", 0, 0, 0);
}

