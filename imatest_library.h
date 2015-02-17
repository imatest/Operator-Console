//
// MATLAB Compiler: 4.18.1 (R2013a)
// Date: Wed Feb 11 01:45:03 2015
// Arguments: "-B" "macro_default" "-B" "cpplib:imatest_library" "-W"
// "cpplib:imatest_library" "-T" "link:lib" "-d"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\build\imatest_it\final\libs\libra
// ry\cpp" "-I" "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\images"
// "-I" "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\imuitools" "-I"
// "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\iptutils" "-a"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\RTDepends" "-a"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ToolboxJava" "-a"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ToolboxMex"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\blemish_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\colorcheck_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\distortion_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\dotpattern_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\ois_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\random_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\sfr_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\sfrplus_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\star_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\stepchart_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\uniformity_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\source\wedge_shell.m"
// "C:\BuildTemp\2015-02-10_23-22-19_BuildTemp\depends\ImatestToolbox\inifile.m"
// "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\images\bwconncomp.m"
// "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\images\demosaic.m"
// "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\images\graythresh.m"
// "C:\Program Files (x86)\MATLAB\R2013a\toolbox\images\images\regionprops.m"
// "C:\Program Files
// (x86)\MATLAB\R2013a\toolbox\images\iptutils\getrangefromclass.m" 
//

#ifndef __imatest_library_h
#define __imatest_library_h 1

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

#ifdef EXPORTING_imatest_library
#define PUBLIC_imatest_library_C_API __global
#else
#define PUBLIC_imatest_library_C_API /* No import statement needed. */
#endif

#define LIB_imatest_library_C_API PUBLIC_imatest_library_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_imatest_library
#define PUBLIC_imatest_library_C_API __declspec(dllexport)
#else
#define PUBLIC_imatest_library_C_API __declspec(dllimport)
#endif

#define LIB_imatest_library_C_API PUBLIC_imatest_library_C_API


#else

#define LIB_imatest_library_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_imatest_library_C_API 
#define LIB_imatest_library_C_API /* No special import/export declaration */
#endif

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV imatest_libraryInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV imatest_libraryInitialize(void);

extern LIB_imatest_library_C_API 
void MW_CALL_CONV imatest_libraryTerminate(void);



extern LIB_imatest_library_C_API 
void MW_CALL_CONV imatest_libraryPrintStackTrace(void);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxBlemish_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxColorcheck_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                      *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxDistortion_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                      *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxDotpattern_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                      *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxOis_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxRandom_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxSfr_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxSfrplus_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxStar_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxStepchart_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                     *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxUniformity_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                      *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxWedge_shell(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxInifile(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxBwconncomp(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxDemosaic(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxGraythresh(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxRegionprops(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_imatest_library_C_API 
bool MW_CALL_CONV mlxGetrangefromclass(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_imatest_library
#define PUBLIC_imatest_library_CPP_API __declspec(dllexport)
#else
#define PUBLIC_imatest_library_CPP_API __declspec(dllimport)
#endif

#define LIB_imatest_library_CPP_API PUBLIC_imatest_library_CPP_API

#else

#if !defined(LIB_imatest_library_CPP_API)
#if defined(LIB_imatest_library_C_API)
#define LIB_imatest_library_CPP_API LIB_imatest_library_C_API
#else
#define LIB_imatest_library_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_imatest_library_CPP_API void MW_CALL_CONV blemish_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV colorcheck_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV distortion_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV dotpattern_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV ois_shell(int nargout, mwArray& results, const mwArray& ncontrol, const mwArray& arg1, const mwArray& arg2, const mwArray& arg3, const mwArray& passFailFile);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV random_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV sfr_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV sfrplus_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV star_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV stepchart_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV uniformity_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV wedge_shell(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV inifile(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV inifile(int nargout, mwArray& varargout);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV inifile(const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV inifile();

extern LIB_imatest_library_CPP_API void MW_CALL_CONV bwconncomp(int nargout, mwArray& CC, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV bwconncomp(int nargout, mwArray& CC);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV demosaic(int nargout, mwArray& RGB, const mwArray& I, const mwArray& sensorAlignment);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV graythresh(int nargout, mwArray& level, mwArray& em, const mwArray& I);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV regionprops(int nargout, mwArray& outstats, const mwArray& varargin);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV regionprops(int nargout, mwArray& outstats);

extern LIB_imatest_library_CPP_API void MW_CALL_CONV getrangefromclass(int nargout, mwArray& range, const mwArray& I);

#endif
#endif
