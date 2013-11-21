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
#include "Timer.h"


Timer::Timer(void)
{
	memset(&m_start, 0, sizeof(m_start));
	memset(&m_stop, 0, sizeof(m_stop));
	m_elapsed = 0;
}

Timer::~Timer(void)
{
}

void Timer::Start()
{
	_ftime_s(&m_start);
}

void Timer::Stop()
{
	_ftime_s(&m_stop);
	Elapsed();
}

///
/// calculates elapsed time in milliseconds
///
void Timer::Elapsed()	
{
	long long	start = (m_start.time * 1000)  + m_start.millitm;
	long long	stop  = (m_stop.time * 1000)   + m_stop.millitm;

	m_elapsed = stop - start;
}

///
/// gets the elapsed time in seconds
///
void Timer::Get(double *sec)	
{
	if (sec != NULL)
	{
		*sec = (double)m_elapsed / 1000.;
	}
}

///
/// gets the elapsed time in milliseconds
///
void Timer::Get(long long *ms)	
{
	if (ms != NULL)
	{
		*ms = m_elapsed;
	}
}

///
/// gets the elapsed time in a formatted string
///
void Timer::Get(CString *str)	
{
	if (str != NULL)
	{
		str->Format("%lu.%03u", (unsigned long)(m_elapsed / 1000), (unsigned)(m_elapsed % 1000));
	}
}
