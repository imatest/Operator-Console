/****************************************************************************	
*	Operator Console - an extensible user interface for the Imatest IT 		*
*	library																	*
*	Copyright (C) 2013 Imatest LLC.											*
*																			*
*	This program is free software: you can redistribute it and/or modify	*
*	it under the terms of the GNU General Public License as published by	*
*	the Free Software Foundation, either version 3 of the License, or		*
*	(at your option) any later version.										*
*																			*
*	This program is distributed in the hope that it will be useful,			*
*	but WITHOUT ANY WARRANTY; without even the implied warranty of			*
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
*	GNU General Public License for more details.							*
*																			*
*	You should have received a copy of the GNU General Public License		*
*	along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*
****************************************************************************/

#include "stdafx.h"
#include "Config.h"


Config::Config(void)
{
	m_iniFilePathName = NULL;
	m_programPath     = NULL;
	m_extension       = NULL;
	m_fileRoot        = NULL;
	m_serialNumber    = NULL;
	m_partNumber      = NULL;
	m_ncolors         = 0;
}

void Config::Init(LPCTSTR iniFileName, LPCTSTR programPath, LPCTSTR ext, LPCTSTR fileRoot, LPCTSTR serialNum, LPCTSTR partNum, int ncolors)
{
	m_iniFilePathName = iniFileName;
	m_programPath     = programPath;
	m_extension       = ext;
	m_fileRoot        = fileRoot;
	m_serialNumber    = serialNum;
	m_partNumber      = partNum;
	m_ncolors         = ncolors;
}


Config::~Config(void)
{
}

bool Config::Load(void)
{
	return true;
}

bool Config::Save(void)
{
	return true;
}