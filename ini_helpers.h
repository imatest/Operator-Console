#ifndef INI_HELPERS_GUARD
#define INI_HELPERS_GUARD

#include "op_console_preferences.h"
#include "imatest_library.h"
#ifdef INI_SEPARATE_PARAMS
void MW_CALL_CONV inifile(int nargout, mwArray& varargout, const mwArray& varargin);

void MW_CALL_CONV inifile(int nargout, mwArray& varargout);

void MW_CALL_CONV inifile(const mwArray& varargin);
#endif

#endif
