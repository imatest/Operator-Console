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

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "TextDlg.h"

#define MSG_MODELESS_QUIT	(WM_APP + 0x40)

///
/// a thread class for creating modeless dialogs which can operate without halting the main dialog
///
class ModelessDialogThread : public CWinThread
{
public:
	DECLARE_DYNCREATE(ModelessDialogThread)
	ModelessDialogThread();
	virtual ~ModelessDialogThread();

	void SetText(LPCTSTR text);
	void SetTitle(LPCTSTR title);
	void Show(bool show);

protected:
	void OnModelessQuit(WPARAM wParam, LPARAM lParam);

// Overrides
public:
	BOOL	InitInstance();
	int		ExitInstance();

protected:

// Implementation
protected:
	TextDlg	*m_dialog;
	
	DECLARE_MESSAGE_MAP()

};
