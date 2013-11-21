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

#include "ThreadControl.h"


ThreadControl::ThreadControl()
{
	m_thread    = NULL;
	m_data      = NULL;
	m_runEvent  = new CEvent(FALSE, FALSE);
	m_doneEvent = new CEvent(FALSE, FALSE);
	m_events[RUN_INDEX]  = m_runEvent->m_hObject;
	m_events[DONE_INDEX] = m_doneEvent->m_hObject;
}


ThreadControl::~ThreadControl(void)
{
	if (m_runEvent != NULL)
	{
		delete m_runEvent;
	}

	if (m_doneEvent != NULL)
	{
		delete m_doneEvent;
	}

	if (m_thread != NULL)
	{
		delete m_thread;
	}
}

bool ThreadControl::Init(UINT threadMsg, DWORD parentID, AFX_THREADPROC threadProc, void *data)
{
	bool	success;

	m_threadMsg = threadMsg;
	m_parentID  = parentID;
	m_data      = data;

	success = InitThread(threadProc);

	return success;
}

bool ThreadControl::InitThread(AFX_THREADPROC threadProc)
{
	SECURITY_ATTRIBUTES	security;

	security.nLength              = sizeof(security);
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle       = TRUE;

	//
	// Create the thread.  We set auto delete to false so that we can tell when the thread
	// quits (otherwise Windows would delete the thread when threadProc returns, and its handle
	// would be invalid).
	//
	// In order to *safely* set auto delete, we have to create the thread in suspended mode.  
	// This ensures that the thread won't quit before we set auto delete.
	//
	m_thread = ::AfxBeginThread(threadProc, this, 0, 0, CREATE_SUSPENDED, &security);//NULL);

	if (m_thread != NULL)
	{
		m_thread->m_bAutoDelete = FALSE;
		m_thread->ResumeThread();
	}

	return m_thread != NULL;
}

void ThreadControl::Quit()
{
	if (m_thread != NULL)
	{
		m_doneEvent->SetEvent();								// trigger the Quit event
		::WaitForSingleObject(m_thread->m_hThread, INFINITE);	// wait for it to quit
		delete m_thread;										// delete the thread
		m_thread = NULL;
	}
}

void ThreadControl::Run()
{
	m_runEvent->SetEvent();	// trigger the Run event
}
