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


// OperatorConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OperatorConsole.h"
#include "OperatorConsoleDlg.h"
#include "qso.h"
#include "afxdialogex.h"
#include "ResizeDlg.h"
#include "Setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PASS_COLOR	RGB(0, 0xff, 0)			// green
#define FAIL_COLOR	RGB(0xff, 0, 0)			// red
#define WHITE		RGB(0xff, 0xff, 0xff)
#define BLACK		RGB(   0,    0,    0)

#define PASSED_TEXT	"PASSED"
#define FAILED_TEXT	"FAILED"
#define QSO_PASSED	"O"
#define QSO_FAILED	"X"
#define MARGIN_Y	16	// number of pixels to inset FPS display relative to bottom of image 

//
// Font size to use for Status and Quadrant status overlays (QSO)
//
#define STATUS_SIZE	16		// points
#define QSO_SIZE	36		// points


///
/// CAboutDlg dialog used for App About
///
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COperatorConsoleDlg dialog



COperatorConsoleDlg::COperatorConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COperatorConsoleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_qsoColor, 0, sizeof(m_qsoColor));
}

void COperatorConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STOP, c_stopButton);
	DDX_Control(pDX, IDC_START, c_startButton);
	DDX_Control(pDX, IDC_LOG, c_log);
	DDX_Control(pDX, IDC_SUMMARY, c_summary);
	DDX_Control(pDX, IDC_STATUS, c_status);
	DDX_Control(pDX, IDC_REASON, c_reason);
	DDX_Control(pDX, IDC_RADIO_SFR, c_sfrPlus);
	DDX_Control(pDX, IDC_RADIO_BLEMISH, c_blemish);
	DDX_Control(pDX, IDC_IMAGE, c_image);
	DDX_Control(pDX, IDC_LOGO, c_logo);
	DDX_Control(pDX, IDC_QSO_UL, c_qsoUL);
	DDX_Control(pDX, IDC_QSO_UR, c_qsoUR);
	DDX_Control(pDX, IDC_QSO_LL, c_qsoLL);
	DDX_Control(pDX, IDC_QSO_LR, c_qsoLR);
	DDX_Control(pDX, IDC_QSO_CENTER, c_qsoCENTER);
	DDX_Control(pDX, IDC_FPS, c_fps);
	DDX_Control(pDX, IDC_SHOWJSON, c_showjson);
	DDX_Control(pDX, IDC_SETUP_BUTTON, c_setup);
	DDX_Control(pDX, IDC_PASS_FAIL, c_pass_fail);
}

BEGIN_MESSAGE_MAP(COperatorConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_START, &COperatorConsoleDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &COperatorConsoleDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_RADIO_SFR, &COperatorConsoleDlg::OnBnClickedRadioSfr)
	ON_BN_CLICKED(IDC_RADIO_BLEMISH, &COperatorConsoleDlg::OnBnClickedRadioBlemish)
	ON_WM_DRAWITEM()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_SHOWJSON, &COperatorConsoleDlg::OnBnClickedShowjson)
	ON_BN_CLICKED(IDC_SETUP_BUTTON, &COperatorConsoleDlg::OnBnClickedSetupButton)
	ON_BN_CLICKED(IDC_PASS_FAIL, &COperatorConsoleDlg::OnClickedPassFail)
END_MESSAGE_MAP()



// COperatorConsoleDlg message handlers

BOOL COperatorConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_NORMAL);//SW_MAXIMIZE);

	// TODO: Add extra initialization here

	c_log.SetLimitText(0x7ffffffe);
	c_sfrPlus.SetCheck(BST_CHECKED);	// select the SFRplus radio button
	OnBnClickedRadioSfr();				// tell the app that the button is selected (so that it knows which test to run)
	ShowStartStop(true, false);			// show the Start button and hide the Stop button
#if 0
	m_image.Load(IMAGE_NAME);
#endif
	m_logo.Load(LOGO_NAME);

#if 1
	//m_logo.SetTransparentColor(WHITE);
	m_logo.SetHasAlphaChannel(false);
#endif

	//	Resize();							// resize dialog to make it big enough for image item (also resizes c_image if needed)
	ResizeDlg resize;
	// HDK COMMENTED THESE FOR TEST
	resize.Resize(this, &c_image, theApp.GetImageWidth(), theApp.GetImageHeight());
	CWnd::CenterWindow();				// center the dialog on the screen
	QsoMove();
	MoveFPS();

	c_image.Init();						// create bitmap and drawing context-- do this after the control has been resized [i.e. after calling Resize()]

	//
	// Create brushes to use to draw the background of the Status item.  
	// We use different colors for passed and failed tests.
	//
	m_passBrush.CreateSolidBrush(PASS_COLOR);
	m_failBrush.CreateSolidBrush(FAIL_COLOR);

	//
	// Create a brush with a sold white background.  This can be used
	// by any item that wants a white background.
	//
	m_whiteBrush.CreateSolidBrush(WHITE);


	//
	// Set the brush handle to NULL to indicate that we don't want to change the background color 
	// of the Status item (the color doesn't get changed until a test has been run).
	//
	m_statusBrush = NULL;
	m_statusColor = 0;

	//
	// Create fonts to use for the status and qso items.  The fonts are the 
	// same as the current font except that the point size is changed.
	//
	CreateFont(&c_status, STATUS_SIZE, &m_statusFont);
	CreateFont(&c_qsoUL, QSO_SIZE, &m_qsoFont);

	c_status.SetFont(&m_statusFont);
	c_qsoUL.SetFont(&m_qsoFont);
	c_qsoUR.SetFont(&m_qsoFont);
	c_qsoLL.SetFont(&m_qsoFont);
	c_qsoLR.SetFont(&m_qsoFont);
	c_qsoCENTER.SetFont(&m_qsoFont);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COperatorConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COperatorConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COperatorConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COperatorConsoleDlg::OnBnClickedStart()
{
#if 1
	ShowStartStop(false, true);	// hide the start button, show the stop button
	PostParentMessage(MSG_START, 0, 0);
	c_setup.ShowWindow(SW_HIDE); // hide the setup button while running

#else
	//
	// This is test code that checks the color and font of
	// the Status item without running an actual test.
	//
	static bool pass = true;
	UpdateStatus(pass, "");
	pass = !pass;

	//
	// This is test code that toggles the values of the QSO display
	// without running an actual test
	//
	static bool quad[eNumQuadrants] = { false, false, false, false, false };
	//static bool quad[eNumQuadrants] = {true, true, true, true, true};
	UpdateQuadrants(quad);
	for (int i = 0; i < eNumQuadrants; i++)
		quad[i] = !quad[i];
#endif

}


void COperatorConsoleDlg::OnBnClickedStop()
{
	ShowStartStop(true, false);	// hide the stop button, show the start button
	PostParentMessage(MSG_STOP, 0, 0);
	c_setup.ShowWindow(SW_SHOW); // show the setup button once the stop button is pressed
}


void COperatorConsoleDlg::ShowStartStop(bool start, bool stop)
{
	//
	// Show or hide the start and stop buttons
	//
	c_startButton.ShowWindow(start ? SW_SHOW : SW_HIDE);
	c_stopButton.ShowWindow(stop ? SW_SHOW : SW_HIDE);
}

void COperatorConsoleDlg::UpdateImage(void *buf, int width, int height)
{
	c_image.Update(buf, width, height);
}

void COperatorConsoleDlg::UpdateStatus(bool passed, LPCTSTR reason)
{
	COLORREF color = passed ? PASS_COLOR : FAIL_COLOR;

	if (passed)
	{
		m_statusColor = PASS_COLOR;
		m_statusBrush = m_passBrush;
	}
	else
	{
		m_statusColor = FAIL_COLOR;
		m_statusBrush = m_failBrush;
	}

	// Set the text size (ignore for now)
	c_status.SetWindowText(passed ? PASSED_TEXT : FAILED_TEXT);	// set PASS/FAIL text
	c_reason.SetSel(0, -1);										// select all of the text
	c_reason.ReplaceSel(passed ? "" : (LPCTSTR)reason);			// if test failed, display reason
}

void COperatorConsoleDlg::UpdateSummary(LPCTSTR summary)
{
	c_summary.SetSel(0, -1);		// select all the text
	c_summary.ReplaceSel(summary);	// replace selection
}

void COperatorConsoleDlg::UpdateQuadrants(const bool *data)
{
	UpdateQuadrant(data, eUpperLeft, &c_qsoUL);
	UpdateQuadrant(data, eUpperRight, &c_qsoUR);
	UpdateQuadrant(data, eLowerLeft, &c_qsoLL);
	UpdateQuadrant(data, eLowerRight, &c_qsoLR);
	UpdateQuadrant(data, eCenter, &c_qsoCENTER);

	c_image.Invalidate();	// if we don't do this the old QSO value doesn't get erased
}


void COperatorConsoleDlg::UpdateFPS(long long ms)
{
	double	fps = 1000. / (double)ms;
	char	str[40];

	sprintf_s(str, sizeof(str) - 1, "%.3lf FPS", fps);

	c_fps.SetWindowText(str);
	c_fps.Invalidate();
	c_image.Invalidate();	// if we don't do this the old QSO value doesn't get erased
}


void COperatorConsoleDlg::UpdateQuadrant(const bool *passed, EQuadrant quadrant, CWnd *control)
{
	COLORREF	color;
	const char	*str;

	if (passed[quadrant])
	{
		color = PASS_COLOR;
		str = QSO_PASSED;
	}
	else
	{
		color = FAIL_COLOR;
		str = QSO_FAILED;
	}

	m_qsoColor[quadrant] = color;
	control->SetWindowText(str);
	control->Invalidate();
}

void COperatorConsoleDlg::GetLog(CString &dst)
{
	if (c_log)
	{
		c_log.GetWindowTextA(dst);
	}
}

void COperatorConsoleDlg::LogMessage(LPCTSTR msg, bool timestamp)
{
	if (msg && *msg)
	{
		int	end = c_log.GetWindowTextLength();

		if (timestamp)
		{
			c_log.SetSel(end, end);
			c_log.ReplaceSel(m_timestamp.Get());
			end = c_log.GetWindowTextLength();
		}

		c_log.SetSel(end, end);
		c_log.ReplaceSel(msg);
	}
}


void COperatorConsoleDlg::OnBnClickedRadioSfr()
{
	QsoShow(true);	// hide the quadrant overlays (they're not used in Blemish)

	//
	// Tell the application to start running SFRplus tests
	//
	PostParentMessage(MSG_SET_SFRPLUS, 0, 0);
}


void COperatorConsoleDlg::OnBnClickedRadioBlemish()
{
	QsoShow(false);	// hide the quadrant overlays (they're not used in Blemish)

	//
	// Tell the application to start running Blemish tests
	//
	PostParentMessage(MSG_SET_BLEMISH, 0, 0);
}

HBRUSH COperatorConsoleDlg::QsoSet(CDC *cdc, EQuadrant quadrant)
{
	HBRUSH	hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);	// this necessary to get transparent background

	cdc->SetTextColor(m_qsoColor[quadrant]);
	cdc->SetBkMode(TRANSPARENT);

	return hbr;
}

void COperatorConsoleDlg::QsoShow(bool show)
{
	int	value = show ? SW_SHOW : SW_HIDE;

	c_qsoUL.ShowWindow(value);
	c_qsoUR.ShowWindow(value);
	c_qsoLL.ShowWindow(value);
	c_qsoLR.ShowWindow(value);
	c_qsoCENTER.ShowWindow(value);
}


HBRUSH COperatorConsoleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	int		dialogItem;
	HBRUSH	hbr;

	//
	// Call the base class implementation first! Otherwise, it may 
	// undo what we're trying to accomplish here.
	//
	hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	dialogItem = pWnd->GetDlgCtrlID();

	switch (dialogItem)
	{
	case IDC_STATUS:
		if (m_statusBrush != NULL)
		{
			pDC->SetBkColor(m_statusColor);
			hbr = m_statusBrush;
		}
		break;

	case IDC_LOG:
		pDC->SetBkColor(WHITE);
		hbr = m_whiteBrush;
		break;
	case IDC_QSO_UL:
		hbr = QsoSet(pDC, eUpperLeft);
		break;
	case IDC_QSO_UR:
		hbr = QsoSet(pDC, eUpperRight);
		break;
	case IDC_QSO_LL:
		hbr = QsoSet(pDC, eLowerLeft);
		break;
	case IDC_QSO_LR:
		hbr = QsoSet(pDC, eLowerRight);
		break;
	case IDC_QSO_CENTER:
		hbr = QsoSet(pDC, eCenter);
		break;

	case IDC_FPS:
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);	// this necessary to get transparent background
		//			pDC->SetTextColor(WHITE);
		pDC->SetBkMode(TRANSPARENT);
	}


	return hbr;
}

HBRUSH COperatorConsoleDlg::SetBGColor(CDC *dc, CBrush brush, COLORREF color)
{
	dc->SetBkColor(color);
	return (HBRUSH)brush;
}

void COperatorConsoleDlg::CreateFont(CWnd *wnd, int points, CFont *newFont)
{
	CFont	*font;
	LOGFONT	logFont;

	font = wnd->GetFont();
	font->GetLogFont(&logFont);
	logFont.lfHeight = points * 10;	// lfHeight will be interpreted as 10ths of a point
	newFont->CreatePointFontIndirect(&logFont);
}

void COperatorConsoleDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDCtl)
	{
	case IDC_IMAGE:
		DrawImage(lpDrawItemStruct);
		break;

	case IDC_LOGO:
		DrawLogo(lpDrawItemStruct);
		break;

	default:
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;
	}
}

void COperatorConsoleDlg::DrawImage(LPDRAWITEMSTRUCT info)
{
#if 0
	m_image.Draw(info->hDC, info->rcItem);
#else
	c_image.Draw(this);
#endif
}

#include <gdiplus.h>

void COperatorConsoleDlg::DrawLogo(LPDRAWITEMSTRUCT info)
{
	int	srcWidth, srcHeight;
	int dstWidth, dstHeight;

	srcWidth = m_logo.GetWidth();
	srcHeight = m_logo.GetHeight();
	dstWidth = info->rcItem.right - info->rcItem.left;
	dstHeight = info->rcItem.bottom - info->rcItem.top;

	m_logo.SetHasAlphaChannel(false);
	m_logo.Draw(info->hDC, info->rcItem);
}


void COperatorConsoleDlg::QsoMove()
{
	CRect	rect;
	CPoint	center;
	int		width, height, dx, dy;

	//
	// Get the location, size, and center point of the image
	//
	ResizeDlg::GetWindowSize(&c_image, &width, &height, &rect);
	center = rect.CenterPoint();

	//
	// Calculate the distance in x and y from the center
	// of the image rect to the center of a quadrant
	//
	dx = (width / 4);
	dy = (height / 4);

	//
	// Now move each window so that it's centered at the center of its quadrant
	//
	CenterWindow(&c_qsoUL, center.x - dx, center.y - dy);	// upper left
	CenterWindow(&c_qsoUR, center.x + dx, center.y - dy);	// upper right
	CenterWindow(&c_qsoLL, center.x - dx, center.y + dy);	// lower left
	CenterWindow(&c_qsoLR, center.x + dx, center.y + dy);	// lower right
	CenterWindow(&c_qsoCENTER, center.x, center.y);		// center
}


void COperatorConsoleDlg::MoveFPS()
{
	CRect	rect;
	CPoint	imageCenter, fpsCenter;
	int		dx, dy, bottom;

	// center the FPS item horizontally in the image, and inset it MARGIN_Y pixels from the bottom of the image

	//
	// Get the center point of the image in screen coordinates
	//
	c_image.GetWindowRect(&rect);
	imageCenter = rect.CenterPoint();

	bottom = rect.bottom - MARGIN_Y;	// we want the bottom of the fps item to be inset MARGINY pixels from the bottom of the image

	//
	// Get the fps item's rectangle and center point in screen coordinates
	//
	c_fps.GetWindowRect(&rect);
	fpsCenter = rect.CenterPoint();

	dx = imageCenter.x - fpsCenter.x;	// number of pixels in X to move fps so that it is centered at imageCenter.x
	dy = bottom - rect.bottom;			// number of pixels in Y to move fps so that its bottom is MARGIN_Y pixels from bottom of image
	rect.OffsetRect(dx, dy);			// move the rect 

	ScreenToClient(&rect);				// convert to client coordinates (relative to client rect of dialog)
	c_fps.MoveWindow(&rect, TRUE);		// move the window
}

void COperatorConsoleDlg::CenterWindow(CWnd *wnd, int centerX, int centerY)
{
	if (wnd != NULL)
	{
		CRect	rect;
		CPoint	pos;

		wnd->GetWindowRect(&rect);							// current window location in screen coordinates
		pos = rect.CenterPoint();							// center of rect in screen coordinates
		rect.OffsetRect(centerX - pos.x, centerY - pos.y);	// move the rect so that its center is at (centerX, centerY) in screen coordinates
		ScreenToClient(&rect);								// convert to client coordinates (relative to client rect of dialog)
		wnd->MoveWindow(&rect, TRUE);						// move the window
	}
}


void COperatorConsoleDlg::OnOK()	// the user clicked on IDOK (the Quit button)
{
	Quit();
	CDialogEx::OnOK();
}

void COperatorConsoleDlg::OnCancel()	// the user clicked on IDCANCEL
{
	Quit();
	CDialogEx::OnCancel();
}


void COperatorConsoleDlg::OnClose()		// the user closed the window
{
	Quit();
	CDialogEx::OnClose();
}


void COperatorConsoleDlg::Quit()
{
	//
	// We need to tell the application that we're ready to close the dialog window so that it
	// can do any cleanup (e.g. waiting for threads to finish) before the dialog closes.
	// This allows a message to be posted to the log window while the app finishes.
	// 
	// Once the app is done, we can go ahead and close the dialog.
	//
	// TDC 2014/08/07 Must save the log outside of running theApp.Quit() otherwise the 
	// coordinate system of c_log gets garbled and c_log.GetWindowText returns an 
	// empty string.

	theApp.Quit();
	theApp.SaveLog();
}


BOOL COperatorConsoleDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL	result;
#if 0
	CRect	rect;
	CWnd	*wnd = pDC->GetWindow();

	if ((wnd == &c_qsoUL) || (wnd == &c_qsoUR) || (wnd == &c_qsoLL) || (wnd == &c_qsoLR) || (wnd == &c_qsoCENTER))
	{
		wnd->GetWindowRect(&rect);
		pDC->FillSolidRect(rect, BLACK);
		result = 1;
	}
	else
#endif
	{
		result = CDialogEx::OnEraseBkgnd(pDC);
	}

	return result;
}


void COperatorConsoleDlg::OnBnClickedShowjson()
{
	PostParentMessage(MSG_JSON, 0, 0);
}

void COperatorConsoleDlg::PostParentMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	::PostThreadMessage(theApp.m_nThreadID, message, 0, 0);
}


#if 0
typedef struct MoveData
{
	CSize	*delta;
	HWND	skip;
	HWND	dialog;
} MoveData;

void COperatorConsoleDlg::Resize()
{
	MoveData	data;

	GetImageWindowDelta();


	OffsetWindowSize(&c_image, m_delta.cx, m_delta.cy);	// resize the image item

	if (m_delta.cx > 0 || m_delta.cy > 0)
	{
		OffsetWindowSize(this, m_delta.cx, m_delta.cy);		// resize the dialog

		//
		// Move the rest of the items dx pixels to the right.  We're not going to change
		// the y coordinates, so we set m_delta.cy to 0.
		//
		m_delta.cy = 0;

		//
		// Move all of the dialog items to compensate for the new size of the image item.  EnumChildWindows() 
		// will call OffsetChildPos() for each of the Dialog's child windows (i.e. all dialog items).  We want
		// to move every item except for the image item.  
		//
		data.delta = &m_delta;
		data.skip = c_image.m_hWnd;	// we don't want to move the image dialog item
		data.dialog = m_hWnd;			// handle to the dialog window (we need this for converting coordinates)
		::EnumChildWindows(this->m_hWnd, OffsetChildPos, (LPARAM)&data);

		//
		// Now we need to center the QSO items in each quadrant of the image
		//
		QsoMove();

		MoveFPS();	// center the FPS item horizontally in the image, and inset it MARGIN_Y pixels from the bottom of the image
	}
}

BOOL CALLBACK COperatorConsoleDlg::OffsetChildPos(HWND hwnd, LPARAM lParam)
{
	MoveData	*data = (MoveData *)lParam;
	RECT	rect;
	int		width, height;
	POINT	ul;

	if (hwnd != data->skip)
	{
		::GetWindowRect(hwnd, &rect);	// rect is in screen coordinates

		ul.x = rect.left;
		ul.y = rect.top;
		::ScreenToClient(data->dialog, &ul);	// make ul coordinates relative to dialog

		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		::MoveWindow(hwnd, ul.x + data->delta->cx, ul.y + data->delta->cy, width, height, TRUE);
	}

	return TRUE;
}


void COperatorConsoleDlg::GetImageWindowDelta()
{
	int	imageWidth, imageHeight;
	int	itemWidth, itemHeight;

	//
	// Get the actual width and height of the image
	//
	imageWidth = theApp.GetImageWidth();
	imageHeight = theApp.GetImageHeight();

	//
	// Get the current width and height of the image dialog item
	//
	GetWindowSize(&c_image, &itemWidth, &itemHeight);

	//
	// Get the difference in the width and height.
	// This is how much the sizes of the image item and dialog need to be increased.
	// This is also how much we need to move the other dialog items to accommodate the new image size.
	//
	m_delta.cx = imageWidth - itemWidth;
	m_delta.cy = imageHeight - itemHeight;

#if 0
	if (m_delta.cx < 0)
	{
		m_delta.cx = 0;
	}

	if (m_delta.cy < 0)
	{
		m_delta.cy = 0;
	}
#endif
}


void COperatorConsoleDlg::GetWindowCenter(CWnd *wnd, CPoint *center)
{
	if (wnd != NULL && center != NULL)
	{
		CRect	rect;
		wnd->GetWindowRect(rect);
		*center = rect.CenterPoint();
	}
}


void COperatorConsoleDlg::GetWindowSize(CWnd *wnd, int *width, int *height, CRect *bnds)
{
	if (wnd != NULL)
	{
		CRect	rect;

		CRect *r = (bnds == NULL) ? &rect : bnds;

		wnd->GetWindowRect(r);

		if (width != NULL)
		{
			*width = r->Width();
		}

		if (height != NULL)
		{
			*height = r->Height();
		}
	}
}

void COperatorConsoleDlg::OffsetWindowSize(CWnd *wnd, int dx, int dy)
{
	int width, height;

	GetWindowSize(wnd, &width, &height);
	wnd->SetWindowPos(NULL, 0, 0, width + dx, height + dy, SWP_NOMOVE | SWP_NOZORDER);
}

void COperatorConsoleDlg::OffsetWindowPos(CWnd *wnd, int dx, int dy)
{
	if (wnd != NULL)
	{
		CRect	rect;
		wnd->GetWindowRect(&rect);	// current window location in screen coordinates
		ScreenToClient(&rect);		// current location in client coordinates (relative to client rect of dialog)
		//		wnd->SetWindowPos(NULL, rect.left+dx, rect.top+dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		rect.OffsetRect(dx, dy);
		wnd->MoveWindow(&rect, TRUE);
	}
}
#endif


void COperatorConsoleDlg::OnBnClickedSetupButton()
{
	//
	// Tell the application to launch the setup dialog
	//
	PostParentMessage(MSG_SETUP, 0, 0);
}


void COperatorConsoleDlg::ReInitDialog(void)
{
	ShowWindow(SW_HIDE);
	int width = theApp.GetImageWidth();
	int height = theApp.GetImageWidth();
	c_image.Update(NULL, width, height);
	//	Resize();							// resize dialog to make it big enough for image item (also resizes c_image if needed)
	ResizeDlg resize;
	resize.BidirectionalResize(this, &c_image, theApp.GetImageWidth(), theApp.GetImageHeight());
	CWnd::CenterWindow();				// center the dialog on the screen
	QsoMove();
	MoveFPS();
	//c_image.Init();						// create bitmap and drawing context-- do this after the control has been resized [i.e. after calling Resize()]
	c_image.ReInit();						// create bitmap and drawing context-- do this after the control has been resized [i.e. after calling Resize()]
	ShowWindow(SW_NORMAL);
}


void COperatorConsoleDlg::OnClickedPassFail()
{
	//
	// Tell the application to launch the pass/fail dialog
	//
	PostParentMessage(MSG_PASS_FAIL, 0, 0);
}
