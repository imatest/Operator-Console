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

// TextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextDlg.h"
#include "afxdialogex.h"


// TextDlg dialog

IMPLEMENT_DYNAMIC(TextDlg, CDialogEx)

TextDlg::TextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(TextDlg::IDD, pParent)
{
	Create(TextDlg::IDD);
}

TextDlg::~TextDlg()
{
	DestroyWindow();
}

void TextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT, c_text);
}


BEGIN_MESSAGE_MAP(TextDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// TextDlg message handlers


void TextDlg::OnClose()
{
	ShowWindow(false);	// if the user clicked on the Cloe button, just hide the window rather than destroying it
}

void TextDlg::SetText(LPCTSTR text)
{
	c_text.SetSel(0, -1);		// select all the text
	c_text.ReplaceSel(text);	// replace selection
}


void TextDlg::Show(bool show)
{
	ShowWindow(show ? SW_SHOW : SW_HIDE);

	if (show)
		BringWindowToTop();
}

void TextDlg::SetTitle(LPCTSTR title)
{
	SetWindowText(title);
}

