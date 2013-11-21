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

#include "CriticalBuf.h"
#include "OperatorConsole.h"


CriticalBuf::CriticalBuf(void)
{
	m_buf    = NULL;
	m_bufLen = 0;
}


CriticalBuf::~CriticalBuf(void)
{
	if (m_buf != NULL)
	{
		delete [] m_buf;
	}
}

bool CriticalBuf::Init(unsigned int bufLen)
{
	bool	success;

	Lock();
	
	if (m_buf != NULL)
	{
		delete [] m_buf;
	}

	m_bufLen = bufLen;
	m_buf    = new byte[m_bufLen];
	success  = m_buf != NULL;

	Unlock();

	return success;
}

void CriticalBuf::Get(void *buf)
{
	if (buf != NULL)
	{
		Lock();

		if (m_buf != NULL)
		{
			memcpy(buf, m_buf, m_bufLen);	// copy m_buf into buf (buf must be at least m_bufLen bytes)
		}

		Unlock();
	}
}

void CriticalBuf::Set(void *buf)
{
	if (buf != NULL)
	{
		Lock();

		if (m_buf != NULL)
		{
			memcpy(m_buf, buf, m_bufLen);	// copy buf into m_buf
		}

		Unlock();
	}
}
