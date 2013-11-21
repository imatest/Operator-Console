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
#include "afxwin.h"
#include "resource.h"		// main symbols

///
/// TextDlg dialog
///
class TextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TextDlg)

public:
	TextDlg(CWnd* pParent = NULL);   //!< standard constructor
	virtual ~TextDlg();
	void SetText(LPCTSTR text);		//!< set the contents of the window to the string pointed to by 'text'
	void SetTitle(LPCTSTR title);	//!< sets the window's title
	void Show(bool show);			//!<  show or hide the window


// Dialog Data
	enum { IDD = IDD_TEXTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    //!< for DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	CEdit c_text;
};
