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

#ifndef __imatest_acquisition_h
#define __imatest_acquisition_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_imatest_acquisition
#define PUBLIC_imatest_acquisition_C_API __global
#else
#define PUBLIC_imatest_acquisition_C_API /* No import statement needed. */
#endif

#define LIB_imatest_acquisition_C_API PUBLIC_imatest_acquisition_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_imatest_acquisition
#define PUBLIC_imatest_acquisition_C_API __declspec(dllexport)
#else
#define PUBLIC_imatest_acquisition_C_API __declspec(dllimport)
#endif

#define LIB_imatest_acquisition_C_API PUBLIC_imatest_acquisition_C_API


#else

#define LIB_imatest_acquisition_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_imatest_acquisition_C_API 
#define LIB_imatest_acquisition_C_API /* No special import/export declaration */
#endif

extern LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV imatest_acquisitionInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV imatest_acquisitionInitialize(void);

extern LIB_imatest_acquisition_C_API 
void MW_CALL_CONV imatest_acquisitionTerminate(void);



extern LIB_imatest_acquisition_C_API 
void MW_CALL_CONV imatest_acquisitionPrintStackTrace(void);

extern LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV mlxAcquire_image(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV mlxList_devices(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_acquisition_C_API 
bool MW_CALL_CONV mlxStop_devware(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_imatest_acquisition
#define PUBLIC_imatest_acquisition_CPP_API __declspec(dllexport)
#else
#define PUBLIC_imatest_acquisition_CPP_API __declspec(dllimport)
#endif

#define LIB_imatest_acquisition_CPP_API PUBLIC_imatest_acquisition_CPP_API

#else

#if !defined(LIB_imatest_acquisition_CPP_API)
#if defined(LIB_imatest_acquisition_C_API)
#define LIB_imatest_acquisition_CPP_API LIB_imatest_acquisition_C_API
#else
#define LIB_imatest_acquisition_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_imatest_acquisition_CPP_API void MW_CALL_CONV acquire_image(int nargout, mwArray& im_orig, mwArray& vstr, const mwArray& source_id, const mwArray& varargin);

extern LIB_imatest_acquisition_CPP_API void MW_CALL_CONV list_devices(int nargout, mwArray& devices);

extern LIB_imatest_acquisition_CPP_API void MW_CALL_CONV stop_devware();

#endif
#endif
