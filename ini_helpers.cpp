#include "ini_helpers.h"
#ifdef INI_SEPARATE_PARAMS
void MW_CALL_CONV inifile(int nargout, mwArray& varargout, const mwArray& varargin)
{
	mwArray filePath = varargin.Get(1, 1);
	mwArray mode = varargin.Get(1, 2);

	mwArray vararginNew = mwArray(1, varargin.NumberOfElements() - 2, mxCELL_CLASS);

	for (mwSize i = 3; i <= varargin.NumberOfElements(); i++)
	{
		vararginNew.Get(1, i - 2).Set(varargin.Get(1, i));
	}

	inifile(nargout, varargout, filePath, mode, vararginNew);
}

void MW_CALL_CONV inifile(int nargout, mwArray& varargout)
{

}

void MW_CALL_CONV inifile(const mwArray& varargin)
{
	mwArray vargoutNone = mwArray();
	inifile(0, vargoutNone, varargin);
}
#endif