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


// CPasswordDialog dialog

///
/// a class for a simple password query dialog
///
class CPasswordDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordDialog)

public:
	CPasswordDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPasswordDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_PWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit c_edit_password;
	CButton c_unlock;

private:
	CString m_password; //!< the user-supplied password attempt
	bool m_unlock;		//!< the user-supplied choice for unlocking administrator access to pass/fail settings

public:
	const CString getResponse(void); //!< returns what the user entered as the password
	const bool getUnlockStatus(void); //!< returns whether the user decided to unlock access to pass/fail settings until the program closes

	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckRetainAccess();
	
	virtual BOOL OnInitDialog();
};
