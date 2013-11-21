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
#include <string>

///
/// A class for acquiring live images through the Imatest library.
///
class ImatestLibAcq :	public ImageAcquisition
{
public:
	ImatestLibAcq(void);
	~ImatestLibAcq(void);

	bool	CaptureFrame();	//!< capture a single frame into m_buf
	bool	Close();
	bool	Open();

protected:
	int		m_numCameras;
	int		m_cameraIndex;
	bool	m_inited;		//!< A boolean that is TRUE if the camera has been initialized
public:
	int m_source_ID;		//!< The source ID for acquire_image() that indicates what type of device is in use
	int m_device_ID;		//!< [Used Epiphan only]: indicates from which of the two sources to capture 
	std::string m_ini_file; //!< The fully-qualified name (including full path) of an Imatest INI file
};

