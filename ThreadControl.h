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

////////////////////////////////////////////////////////////////////////
///
///@class ThreadControl 
/// Thread synchronization and communication class.
///
/// This class simplifies the task of having 2 threads communicate with each other.
///
/// Thread control is done using CEvent objects, by calling WaitForMultipleObjects().
/// The child thread will block until it receives either of 2 objects:
/// RUN_OBJECT  tells the thread to run its function, and send a message to its parent thread
/// DONE_OBJECT tells the thread to quit
///
/// A pointer to a ThreadControl object should be passed as the thread function's single parameter
///
////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxmt.h>

//
// The thread is controlled using CEvents, by calling WaitForMultipleObjects().
// The thread waits until it receives either of 2 objects:
// RUN_OBJECT  tells the thread to run its function, and send a message to its parent thread
// DONE_OBJECT tells the thread to quit
//
// RUN_INDEX and DONE_INDEX define the position in the array of CEvents
//
#define RUN_INDEX			0
#define DONE_INDEX			1
#define NUM_THREAD_EVENTS	2
#define RUN_OBJECT			(WAIT_OBJECT_0 + RUN_INDEX)
#define DONE_OBJECT			(WAIT_OBJECT_0 + DONE_INDEX)


class ThreadControl
{
public:
	ThreadControl();
	virtual ~ThreadControl(void);

	bool	Init(UINT threadMsg, DWORD parentID, AFX_THREADPROC threadProc, void *data);
	void	Quit();
	void	Run();

protected:
	void	InitEvents();
	bool	InitThread(AFX_THREADPROC threadProc);

public:
	CWinThread	*m_thread;		//!< the thread that we want to control
	CEvent		*m_runEvent;	//!< when this event is received, run the child's function once
	CEvent		*m_doneEvent;	//!< when this event is received, terminate the thread
	HANDLE		m_events[2];
	UINT		m_threadMsg;	//!< message to send to parent thread
	DWORD		m_parentID;		//!< the thread to send the message to
	void		*m_data;		//!< pointer to whatever is useful
};

