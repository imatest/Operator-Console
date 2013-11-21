// MsgDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OperatorConsole.h"
#include "MsgDialog.h"
#include "afxdialogex.h"


// MsgDialog dialog

IMPLEMENT_DYNAMIC(MsgDialog, CDialogEx)

MsgDialog::MsgDialog(LPCTSTR msg, CWnd* pParent /*=NULL*/)
	: CDialogEx(MsgDialog::IDD, pParent)
{
	m_msg = msg;
}

MsgDialog::~MsgDialog()
{
}

void MsgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG, c_msg);
	DDX_Control(pDX, IDC_ICON, c_icon);
}


BEGIN_MESSAGE_MAP(MsgDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// MsgDialog message handlers

BOOL MsgDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hicon = theApp.LoadStandardIcon(IDI_ASTERISK);

	c_icon.SetIcon(hicon);
	c_msg.SetWindowText(m_msg);
	ShowWindow(SW_NORMAL);//SW_MAXIMIZE);

	// TODO: Add extra initialization here
	


	return TRUE;  // return TRUE  unless you set the focus to a control
}


void MsgDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}


void MsgDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
