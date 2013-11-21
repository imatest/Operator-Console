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
#include "FileAcquisition.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


FileAcquisition::FileAcquisition(void)
{
	m_numPixels = 0;
	m_buf       = NULL;
	m_filename.Empty();
}


FileAcquisition::~FileAcquisition(void)
{
}

///
/// Initializes the buffers inherited from class ImageAcquisition with 2 bytes per pixel.
/// @return Returns TRUE if allocation is successful.
///
bool FileAcquisition::Init(int width, int height, const char *filename)
{
	bool	result;

	m_filename = filename;
	result     = ImageAcquisition::Init(width, height, 2);

	return result;
}

bool FileAcquisition::Open()
{
	bool result = LoadFile();

	return result;
}

bool FileAcquisition::Close()
{
	return true;
}
///
/// Loads an image file through a filestream and then copies into m_buf. Optionally, m_buf can
/// be copied into the critical buffer, m_frame.
/// @return Returns TRUE if the file is loaded into m_buf, FALSE otherwise.
///
bool FileAcquisition::LoadFile()
{
	bool				success = false;
	ifstream::pos_type	size;
	ifstream			filestream(m_filename, ios::binary);

	if (!filestream.is_open()) 
	{
		m_logMsg.Format("%s: Unable to open image file '%s'", __FUNCTION__, m_filename);
	}
	else
	{
		filestream.seekg(0, ios::end);	// Determine length of file
		size = filestream.tellg();
		filestream.seekg(0, ios::beg);	// rewind

		//
		// Make sure that our buffer is large enough to hold the file
		//
		if (m_numBytes != (unsigned int)size)
		{
			m_logMsg.Format("%s: '%s' is too big for buffer", __FUNCTION__, m_filename);
		}
		else if (m_buf == NULL)
		{
			m_logMsg.Format("%s: m_buf is NULL", __FUNCTION__);
		}
		else
		{
			//
			// Read the contents of the file into m_buf, closing the file when done
			//
			filestream.read((char *)m_buf, size);
			filestream.close();
			m_logMsg.Format("%s:  Read in %d bytes", __FUNCTION__, (long)size);
#if 0
			
			m_frame.Set(m_buf);	// this copies m_buf to shared memory that gets accessed by GetFrame()
#endif
			success = true;
		}
	}

	return success;
}
