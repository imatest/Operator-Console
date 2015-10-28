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

#include "stdafx.h"
#include "CriticalBuf.h"
#include <string>
///
/// the base class for image acquisition
///
class ImageAcquisition
{
public:
	ImageAcquisition(void);
	virtual ~ImageAcquisition(void);

	virtual	bool		CaptureFrame() = 0;	//!< capture a single image into m_buf
	virtual bool		Close()= 0;
	virtual bool		Open() = 0;

	void				GetFrame(void *buf) {m_frame.Get(buf);}
	virtual bool		Init(int width, int height, int bytesPerPixel=4);	//!< may be overridden, but be sure to call parent function also

	unsigned int		BytesPerFrame() {return m_numBytes;}
	unsigned int		PixelsPerFrame() {return m_numPixels;}
	int					GetHeight() {return m_height;}
	CString				&GetInfo()  {return m_logMsg;}
	int					GetWidth()  {return m_width;}
	void				SetHeight(int height) {m_height = height;}
	void				SetWidth(int width)   {m_width = width;}

	static UINT __cdecl ThreadProc(LPVOID param);

	int m_source_ID;		//!< The source ID for acquire_image() that indicates what type of device is in use
	int m_device_ID;		//!< [Used Epiphan only]: indicates from which of the two sources to capture 
	std::string m_ini_file; //!< The fully-qualified name (including full path) of an Imatest INI file

protected:
	CriticalBuf		m_frame;		//!< shared memory to hold copy of current frame
	int				m_width;		//!< width of image in pixels
	int				m_height;		//!< height of image in pixels
	unsigned int	m_numBytes;		//!< number of bytes in a frame
	unsigned int	m_numPixels;	//!< number of pixels in a frame
	CString			m_logMsg;		//!< error or information message
	void			*m_buf;			//!< buffer to capture into
};

