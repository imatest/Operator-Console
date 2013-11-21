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
///
/// A class that resizes and centers dialog objects within a parent window.
///
class ResizeDlg
{
public:
	ResizeDlg(void);
	virtual ~ResizeDlg(void);

	static void				CenterWindow(CWnd *parent, CWnd *wnd, bool horizontal=true, bool vertical=true);
	void					GetImageWindowDelta(int width, int height);
	static void				GetWindowCenter(CWnd *wnd, CRect *bnds, CPoint *center);
	static void				GetWindowSize(CWnd *wnd, int *width, int *height, CRect *bnds=NULL, bool client=false);
	void					OffsetWindowSize(CWnd *wnd, int dx, int dy);
	void					OffsetWindowPos(CWnd *parent, CWnd *wnd, int dx, int dy);
	static BOOL CALLBACK	OffsetChildPos(HWND hwnd, LPARAM lParam);
	void					Resize(CWnd *dialog, CWnd *window, int width, int height);
	void					BidirectionalResize(CWnd *dialog, CWnd *window, int width, int height);
protected:
	CSize	m_delta;
	CWnd	*m_dialog;
	CWnd	*m_window;

public:
	
};




