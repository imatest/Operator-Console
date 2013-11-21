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
#include "ModelessDialogThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(ModelessDialogThread, CWinThread)


// ModelessDialogThread

BEGIN_MESSAGE_MAP(ModelessDialogThread, CWinThread)
	ON_THREAD_MESSAGE(MSG_MODELESS_QUIT, OnModelessQuit)
END_MESSAGE_MAP()



// ModelessDialogThread construction

ModelessDialogThread::ModelessDialogThread()
{
	m_dialog = NULL;
}

ModelessDialogThread::~ModelessDialogThread()
{
}


// ModelessDialogThread initialization

BOOL ModelessDialogThread::InitInstance()
{

	CWinThread::InitInstance();

	m_dialog   = new TextDlg;
	m_pMainWnd = m_dialog;

	return TRUE;
}


int ModelessDialogThread::ExitInstance()
{
	if (m_dialog != NULL)
	{
		delete m_dialog;
	}

	return CWinThread::ExitInstance();
}


void ModelessDialogThread::Show(bool show)
{
	if (m_dialog != NULL)
	{
		m_dialog->Show(show);
	}
}

void ModelessDialogThread::SetTitle(LPCTSTR title)
{
	if (m_dialog != NULL)
	{
		m_dialog->SetTitle(title);
	}
}

void ModelessDialogThread::SetText(LPCTSTR text)
{
	if (m_dialog != NULL)
	{
		m_dialog->SetText(text);
	}
}


void ModelessDialogThread::OnModelessQuit(WPARAM wParam, LPARAM lParam) 
{
	PostQuitMessage(0);
}
