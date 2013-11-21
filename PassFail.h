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
#include "afxpropertygridctrl.h"
#include "afxwin.h"
#include "PassFailSettings.h"
///
/// The class underlying the PassFail dialog
///
class CPassFail : public CDialogEx
{
	DECLARE_DYNAMIC(CPassFail)

public:
	CPassFail(CWnd* pParent = NULL);   // standard constructor
	CPassFail(CPassFailSettings& InPassFail, CWnd* pParent = NULL);
	virtual ~CPassFail();

// Dialog Data
	enum { IDD = IDD_PASS_FAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic c_proplist_location; //!< The place-holder for CMFCPropertyGridCtrl
	CButton c_ok;
	CButton c_cancel;
	virtual BOOL OnInitDialog();
	CMFCPropertyGridCtrl m_wndPropList;
	CPassFailSettings PFSettings;

	afx_msg void OnBnClickedOk();
};
