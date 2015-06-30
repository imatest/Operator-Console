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
#include "ImageAcquisition.h"
#include "ThreadControl.h"


ImageAcquisition::ImageAcquisition(void)
{
	m_width     = 0;
	m_height    = 0;
	m_numBytes  = 0;
	m_numPixels = 0;
   m_bytesPerPixel = 0;
   m_ncolors   = 1;
	m_buf       = NULL;
}


ImageAcquisition::~ImageAcquisition(void)
{
	if (m_buf != NULL)
	{
		delete [] m_buf;
	}
}

bool ImageAcquisition::Init(int width, int height, int bytesPerPixel)
{
	bool	success = false;

	m_width     = width;
	m_height    = height;
   m_bytesPerPixel = bytesPerPixel;
	m_numPixels = m_width * m_height;
	m_numBytes  = m_numPixels * bytesPerPixel;

	if ( m_buf != NULL) // allow for resizing of buffer by re-initialization
	{
		delete[] m_buf;
	}

	m_buf       = new byte[m_numBytes];

	if (m_buf == NULL)
	{
		m_logMsg.Format("%s: Unable to allocate image buffer (%u bytes)", __FUNCTION__, m_numPixels);
	}
	else
	{
		success = m_frame.Init(m_numBytes);
	}

   if ( m_displayBuf != NULL) // allow for resizing of buffer by re-initialization
	{
		delete[] m_displayBuf;
	}

   unsigned int displayBytes = m_numPixels*4;
	m_displayBuf       = new byte[displayBytes];

	if (m_displayBuf == NULL)
	{
		m_logMsg.Format("%s: Unable to allocate display buffer (%u bytes)", __FUNCTION__, displayBytes);
	}
	else
	{
		success = m_displayFrame.Init(displayBytes);
	}


	return success;
}


UINT __cdecl ImageAcquisition::ThreadProc(LPVOID param)
{
	ThreadControl		*info = (ThreadControl *)param;
	ImageAcquisition	*acq = (ImageAcquisition *)info->m_data;
	bool				done  = false;
	DWORD				eventIndex;

	while (!done)
	{
		eventIndex = ::WaitForMultipleObjects(NUM_THREAD_EVENTS, info->m_events, FALSE, INFINITE);

		if (eventIndex == DONE_INDEX)
		{
			done = TRUE;
		}
		else if (eventIndex == RUN_INDEX)
		{
			acq->CaptureFrame();
			acq->m_frame.Set(acq->m_buf);
         acq->m_displayFrame.Set(acq->m_displayBuf);
			::PostThreadMessage(info->m_parentID, info->m_threadMsg, (WPARAM)0, (LPARAM)0);
		}
	}

	return 0;	// this terminates the thread
}
