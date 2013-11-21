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

#include "CriticalCString.h"


CriticalCString::CriticalCString(void)
{
}


CriticalCString::~CriticalCString(void)
{
}

void CriticalCString::Clear()
{
	Lock();
	m_string.Empty();
	Unlock();
}


void CriticalCString::Get(CString &dest, bool clear)
{
	Lock();
	dest = m_string;					// copy m_string into dest str

	if (clear)
	{
		m_string.Empty();
	}
	Unlock();
}

void CriticalCString::Set(CString &src)
{
	Lock();
	m_string = src;						// copy src into m_string
	Unlock();
}

void CriticalCString::Set(std::string &src)
{
	Lock();
	m_string.SetString(src.c_str());	// copy src into m_string
	Unlock();
}

void CriticalCString::Set(const char *src)
{
	if (src != NULL)
	{
		Lock();
		m_string.SetString(src);			// copy src into m_string
		Unlock();
	}
}

void CriticalCString::Set(const char *str, int len)
{
	if (str != NULL)
	{
		Lock();
		m_string.SetString(str, len);		// copy len characters of str into m_string
		m_string.AppendChar('\0');
		Unlock();
	}
}

bool CriticalCString::Append(CString &str)
{
	bool wasEmpty;

	Lock();
	wasEmpty = m_string.IsEmpty();

	if (!str.IsEmpty())
	{
		m_string.Append(str);			// concatenate str onto the end of m_string
	}

	Unlock();

	return wasEmpty;
}

bool CriticalCString::Append(const char *str, int len)
{
	bool wasEmpty;

	Lock();
	wasEmpty = m_string.IsEmpty();

	if (str != NULL)
	{
		m_string.Append(str,len);			// concatenate str onto the end of m_string
	}

	Unlock();

	return wasEmpty;
}
