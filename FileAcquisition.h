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
#include "ImageAcquisition.h"

///
/// Another class for loading an image file that loads into the critical buffer m_frame inherited from class ImageAcquisition.
///
class FileAcquisition :	public ImageAcquisition
{
public:
	FileAcquisition(void);
	~FileAcquisition(void);
	bool	CaptureFrame()  {return true;}	//!< image is already in m_frame, so there's nothing to do
	bool	Close();
	bool	Init(int width, int height, const char *filename);
	bool	Open();
	void	SetFile(const CString filePathName) {m_filename = filePathName;}

protected:
	bool	LoadFile();

protected:
	CString	m_filename;		//!< fully-qualified image file name
};

