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

#include "FileAcq.h"


FileAcq::FileAcq(void)
{
}


FileAcq::~FileAcq(void)
{
}

///
/// Copies *filepathname to m_filename
///
void FileAcq::Init(const char *filepathname)
{
	m_filename = filepathname;
}

/// 
/// Loads a file first into the CImage object m_image, and then into m_buf inherited from class ImageAcquisition if allocation is successful.
/// @return Returns TRUE if m_buf is successfully allocated, FALSE otherwise
///
bool FileAcq::Open()
{
	bool	success = false;
	int		width, height, bytesPerPixel;
	HRESULT	result;

	result = m_image.Load(m_filename);

	if (result == S_OK)
	{
		bytesPerPixel = m_image.GetBPP() / 8;
		width         = m_image.GetWidth();
		height        = m_image.GetHeight();
		success       = ImageAcquisition::Init(width, height, bytesPerPixel);

		if (success)
		{
			// copy the pixels into m_buf:
			// create a memory drawing context
			//
			HDC hdc         = m_image.GetDC();
			HBITMAP hbitmap = (HBITMAP)m_image;//GetCurrentObject(hdc, OBJ_BITMAP);
			BITMAPINFO	info;

			memset(&info, 0, sizeof(info));
			info.bmiHeader.biSize          = sizeof(info.bmiHeader);
			info.bmiHeader.biWidth         = m_width;
			info.bmiHeader.biHeight        = -m_height;	// a negative value means top-down instead of bottom-up
			info.bmiHeader.biPlanes        = 1;
			info.bmiHeader.biBitCount      = 32;
			info.bmiHeader.biCompression   = BI_RGB;
			info.bmiHeader.biSizeImage     = 0;
			info.bmiHeader.biXPelsPerMeter = 1;
			info.bmiHeader.biYPelsPerMeter = 1;
			info.bmiHeader.biClrUsed       = 0;
			info.bmiHeader.biClrImportant  = 0;

			GetDIBits(hdc, hbitmap, 0, m_height, m_buf, &info, DIB_RGB_COLORS);
			m_image.ReleaseDC();
		}
	}
	
	return success;
}

