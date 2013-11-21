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

#include "StdoutThread.h"


StdoutThread::StdoutThread(void)
{
	m_bufLen   = 0;
	m_msg      = 0;
	m_parentID = 0;
}


StdoutThread::~StdoutThread(void)
{
}

void StdoutThread::Quit()
{
	if (m_thread != NULL)
	{
		Stop();
		Close();	// close the pipe-- this will cause ThreadProc to return (i.e. terminate)
		::WaitForSingleObject(m_thread->m_hThread, INFINITE);	// wait for it to quit
		delete m_thread;										// delete the thread
		m_thread = NULL;
	}
}


bool StdoutThread::InitThread(int bufLen, UINT threadMsg, DWORD parentID)
{
	SECURITY_ATTRIBUTES	security;

	m_bufLen   = bufLen;
	m_msg      = threadMsg;
	m_parentID = parentID;

	Init(m_bufLen);	// open the pipes
	Start();		// start getting output

	//
	// Create the thread.  We set auto delete to false so that we can tell when the thread
	// quits (otherwise Windows would delete the thread when threadProc returns, and its handle
	// would be invalid).
	//
	// In order to *safely* set auto delete, we have to create the thread in suspended mode.  
	// This ensures that the thread won't quit before we set auto delete.
	//
	security.nLength              = sizeof(security);
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle       = TRUE;

	m_thread = ::AfxBeginThread(ThreadProc, this, 0, 0, CREATE_SUSPENDED, &security);

	if (m_thread != NULL)
	{
		m_thread->m_bAutoDelete = FALSE;
		m_thread->ResumeThread();
	}

	return m_thread != NULL;
}


static int FindChar(const char *buf, char c, int len, int delta)
{
	const char	*start, *end, *ch;
	int	index;


	if (delta > 0)
	{
		start = buf;
		end   = buf+len;
	}
	else
	{
		start = buf+len-1;
		end   = buf;
	}

	index = -1;

	for (ch = start; (ch != end) && (index = -1); ch += delta)
	{
		if (*ch == c)
		{
			index = ch - start;
		}
	}


	return index;
}


static int FindChar(const char *buf, char c, int len)
{
	int	i, index;

	index = -1;

	for (i = len-1; (i >= 0) && (index < 0); i--)
	{
		if (buf[i] == c)
		{
			index = i;
		}
	}

	return index;
}


#define BUF_LEN	256


UINT __cdecl StdoutThread::ThreadProc(void *param)
{
	char			*buf[2];
	StdoutThread	*data = (StdoutThread *)param;
	char			*src, *dst, *ch;
	int				bufIndex;	// tells which buffer to read data into (buf[0] or buf[1])
	int				len;		// the number of characters in the buffer
	int				bytesRead;	// the number of bytes that were just read from the pipe
	int				leftover;	// the number of characters remaining in the buffer after the newline
	int				index;		// the array index of the last newline ('\n') in the buffer


	//
	// Allocate 2 buffers to hold the data read from the pipe
	//
	buf[0] = new char[data->m_bufLen];
	buf[1] = new char[data->m_bufLen];


	if ((buf[0] == NULL) || (buf[1] == NULL))
	{
		data->m_str.Set("Unable to allocate buffers in StdoutThread\n\r");
		::PostThreadMessage(data->m_parentID, data->m_msg, (WPARAM)0, (LPARAM)0);	// tell the parent thread the data is ready
	}
	else
	{
		memset(buf[0], 0, data->m_bufLen);
		memset(buf[1], 0, data->m_bufLen);

		bufIndex = 0;
		len      = 0;

		do
		{
			//
			// Read all available data (up BUF_LEN bytes) from the pipe.  Note that GetBuffer() will block until
			// data is available or the pipe is closed.  We start reading new data into the end of the buffer.
			//
			ch = buf[bufIndex] + len;
			bytesRead = data->GetBuffer(ch, BUF_LEN);

			if (bytesRead > 0)	// 0 means that the pipe was closed
			{
				//
				// Look for a newline character in the buffer, starting at the end of the buffer
				//
				len += bytesRead;							// total number of bytes in buf
				index = FindChar(buf[bufIndex], '\n', len);	// find last newline (if any) in buf (-1 means no newline was found)
			
				//
				// If we found a newline, copy the string into the shared data buffer.  It is possible that
				// the string may contain more than one newline (i.e. more that one line of text), but that's
				// okay-- we just want to make sure that we copy complete lines into the destination buffer.
				//
				if (index >= 0)
				{
					//
					// Copy buf[0] to buf[index] (inclusive) into m_str.  If data->m_str was empty before the copy, 
					// post a message to tell the parent thread that data is ready.  If data->m_str wasn't empty
					// that means that the parent thread hasn't gotten around to processing the previous message yet, 
					// so we don't need to post another message.
					//
					// An alternative to this system would be to pass a pointer to the string in PostThreadMessage,
					// but then we would need to have several messages allocated ahead of time.
					//
					if (data->m_str.Append(buf[bufIndex], index+1))
					{
						::PostThreadMessage(data->m_parentID, data->m_msg, (WPARAM)0, (LPARAM)0);
					}

					//
					// Copy any leftover characters into the other buffer
					//
					leftover = (len - index) - 1;

					if (leftover)
					{
						src = buf[bufIndex] + (index+1);
						dst = buf[bufIndex ^ 1];
						memcpy(dst, src, leftover);					// if I used memmove() instead, I would only need 1 buffer
						memset(buf[bufIndex], 0, data->m_bufLen);	// this makes debugging easier
						bufIndex ^= 1;								// switch to other buffer 0 -> 1,  1-> 0
					}

					len = leftover;
				}
			}
		}
		while (bytesRead > 0);	// 0 means that the pipe has been closed

		delete [] buf[0];
		delete [] buf[1];
	}

	return 0;
}
