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
#include "CriticalCString.h"
#include "StdoutRedirect.h"
///
/// Defines a thread class for capturing stdout
///
class StdoutThread : public StdoutRedirect
{
public:
	StdoutThread(void);
	virtual ~StdoutThread(void);
	
	void				Get(CString &dst, bool clear) {m_str.Get(dst, clear);}
	void				Quit();
	bool				InitThread(int bufLen, UINT threadMsg, DWORD parentID);
	static UINT __cdecl ThreadProc(void *param);

protected:
	int					m_bufLen;	//!< the size of the stdio buffer
	CriticalCString		m_str;		//!< this stores the data that was read
	UINT				m_msg;		//!< message to send when data has been read
	DWORD				m_parentID;	//!< ID of thread to send m_msg to 
	CWinThread			*m_thread;	//!< pointer to our thread
};

