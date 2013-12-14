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

// OperatorConsoleDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "StaticBmp.h"
#include "Timestamp.h"


// COperatorConsoleDlg dialog
/// The class underlying the main dialog.
class COperatorConsoleDlg : public CDialogEx
{
	// Construction
public:
	COperatorConsoleDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_OPERATORCONSOLE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void ShowStartStop(bool start, bool stop);

	// Implementation
protected:
	HICON m_hIcon;

	BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	void Quit();

	// Generated message map functions

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioBlemish();
	afx_msg void OnBnClickedRadioSfr();
	afx_msg void OnBnClickedShowjson();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedSetupButton();
	afx_msg void OnClickedPassFail();
	void DrawImage(LPDRAWITEMSTRUCT info);
	void DrawLogo(LPDRAWITEMSTRUCT info);
	void GetLog(CString &dst);
	void LogMessage(LPCTSTR msg, bool timestamp=true);
	void ReInitDialog(void);				//!< Allows the display to be resized after the user changes the image size
	void UpdateFPS(long long ms);
	void UpdateImage(void *buf);
	void UpdateStatus(bool passed, LPCTSTR reason);
	void UpdateSummary(LPCTSTR summary);
	void UpdateQuadrants(const bool *data);

protected:
#if 0
	void GetImageWindowDelta();
	static void				GetWindowCenter(CWnd *wnd, CPoint *center);
	static void				GetWindowSize(CWnd *wnd, int *width, int *height, CRect *bnds=NULL);
	void					OffsetWindowPos(CWnd *w, int dx, int dy);
	static BOOL CALLBACK	OffsetChildPos(HWND hwnd, LPARAM lParam);
	static void				OffsetWindowSize(CWnd *wnd, int dx, int dy);
	void Resize(void);
#endif
	void					CenterWindow(CWnd *wnd, int centerX, int centerY); //!< move 'wnd' so its center is at the screen coordinate (centerX, centerY)
	void					CreateFont(CWnd *wnd, int points, CFont *newFont);
	void					MoveFPS();
	void					PostParentMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	void					QsoMove();
	HBRUSH					QsoSet(CDC *cdc, EQuadrant quadrant);
	void					QsoShow(bool show);
	HBRUSH					SetBGColor(CDC *dc, CBrush brush, COLORREF color);
	void					UpdateQuadrant(const bool *passed, EQuadrant quadrant, CWnd *control);

protected:
	CBrush		m_whiteBrush;
	CBrush		m_passBrush;				//!< the brush (i.e. background color) to be used for Status item when the test has passed
	CBrush		m_failBrush;				//!< the brush (i.e. background color) to be used for Status item when the test has failed
	HBRUSH		m_statusBrush;				//!< handle to the brush to be used when drawing the Status item
	COLORREF	m_statusColor;				//!< unused if m_statusBrush == NULL
	COLORREF	m_qsoColor[eNumQuadrants];	//!< color to use for each QSO item
	CFont		m_statusFont;				//!< font to use for Status item (used to change font size for the item)
	CFont		m_qsoFont;					//!< font to use for QSO (Quadrant Status Overlay) items
#if 0
	CImage		m_image;					// static image to display until live acquisition is implemented
#endif
	CImage		m_logo;						//!< Imatest logo (goes at top of info display)
	CSize		m_delta;					//!< number of pixels to increase image item and dialog size
	Timestamp	m_timestamp;

public:
	CButton		c_startButton;
	CButton		c_stopButton;
	CEdit		c_summary;					//!< The summary display
	CStatic		c_status;					//!< The status display
	CEdit		c_reason;					//!< The reason display
	CEdit		c_log;						//!< The log display
	CButton		c_sfrPlus;					//!< The sfrPlus radial button
	CButton		c_blemish;					//!< The Blemish radial button
	CStaticBmp	c_image;					//!< owner-drawn control that displays an image from the acquisition source
	CStatic		c_logo;
	CStatic		c_qsoUL;					//!< The static text for the upper left QSO
	CStatic		c_qsoUR;					//!< The static text for the upper right QSO
	CStatic		c_qsoLL;					//!< The static text for the lower left QSO
	CStatic		c_qsoLR;					//!< The static text for the lower right QSO
	CStatic		c_qsoCENTER;				//!< The static text for the center QSO 
	CStatic		c_fps;						//!< The frames-per-second display
	CButton		c_showjson;					//!< The 'show JSON' button
	CButton		c_setup;					//!< The setup button	
	CButton		c_pass_fail;
	

};
