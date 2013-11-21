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

#pragma once
///
/// A class that contains a few configuration details.
///
class Config
{
public:
	Config(void);
	virtual ~Config(void);
	void Init(LPCTSTR iniFileName, LPCTSTR programPath, LPCTSTR ext, 
		LPCTSTR fileRoot, LPCTSTR serialNum, LPCTSTR partNum, int nColors);
	
protected:
	bool	Load();
	bool	Save();

public:
	const char *m_programPath;		//!< Path to Imatest
	const char *m_iniFilePathName;	//!< Fully-qualified ini file name (including full path)
	const char *m_extension;		//!< RAW file extension
	const char *m_fileRoot;			//!< File root 
	const char *m_serialNumber;		//!< Camera serial number
	const char *m_partNumber;		//!< Camera part number
	int			m_ncolors;			// 1 = raw data 3 = rgb data
};

