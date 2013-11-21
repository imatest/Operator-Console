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

//
// OperatorConsole.h : 
///
///Main thread/header file for the Operator Console application
///

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// AppThread:
// See AppThread.cpp for the implementation of this class
//

class AppThread : public CWinApp
{
public:
	AppThread();
	~AppThread();
	void Quit();

// Overrides
public:
	BOOL	InitInstance();

protected:
	void	CreatePipes();

public:

// Implementation
protected:
//	COperatorConsoleApp	*m_thread;
	HANDLE				m_stderr_Rd;
	HANDLE				m_stderr_Wr;
	HANDLE				m_stdout_Rd;
	HANDLE				m_stdout_Wr;

	DECLARE_MESSAGE_MAP()

};


extern AppThread theApp;