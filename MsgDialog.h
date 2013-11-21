#pragma once
#include "afxwin.h"


// MsgDialog dialog

class MsgDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MsgDialog)

public:
	MsgDialog(LPCTSTR msg, CWnd* pParent = NULL);   // standard constructor
	virtual ~MsgDialog();
	
// Dialog Data
	enum { IDD = IDD_MSGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CStatic c_icon;
	CEdit	c_msg;
	CString	m_msg;
};
