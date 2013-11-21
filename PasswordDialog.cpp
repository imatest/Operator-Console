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

// PasswordDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OperatorConsole.h"
#include "PasswordDialog.h"
#include "afxdialogex.h"


// CPasswordDialog dialog

IMPLEMENT_DYNAMIC(CPasswordDialog, CDialogEx)

CPasswordDialog::CPasswordDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordDialog::IDD, pParent)
{

	m_password.SetString(_T(""));
	m_unlock = false;
}

CPasswordDialog::~CPasswordDialog()
{
}

void CPasswordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PWD, c_edit_password);
	DDX_Control(pDX, IDC_CHECK_RETAIN_ACCESS, c_unlock);
}


BEGIN_MESSAGE_MAP(CPasswordDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPasswordDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_RETAIN_ACCESS, &CPasswordDialog::OnClickedCheckRetainAccess)
END_MESSAGE_MAP()


// CPasswordDialog message handlers


const CString CPasswordDialog::getResponse(void)
{
	return m_password;
}


void CPasswordDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	c_edit_password.GetWindowTextA(m_password); // copy the entered password to the private variable;
}


void CPasswordDialog::OnClickedCheckRetainAccess()
{
	m_unlock = (BST_CHECKED==c_unlock.GetCheck());
}


const bool CPasswordDialog::getUnlockStatus(void)
{
	return m_unlock;
}


BOOL CPasswordDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_unlock)
	{
		c_unlock.SetCheck(BST_CHECKED);
	}
	else
	{
		c_unlock.SetCheck(BST_UNCHECKED);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
