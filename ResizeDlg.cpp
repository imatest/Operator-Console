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

#include "stdafx.h"
#include "ResizeDlg.h"
///
///  A struct for containing data relevant for moving a window
///
typedef struct MoveData
{
	CSize	*delta; //!< points to a CSize that details how much to shift the window horizontally and vertically
	HWND	skip; //!< pointer to the window that should not be moved i.e. the main window during a resize event
	HWND	dialog; //!< pointer to the dialog that needs to be moved
} MoveData;

ResizeDlg::ResizeDlg(void)
{
}


ResizeDlg::~ResizeDlg(void)
{
}

///
/// Resizes an object within a parent window but does not allow the object to shrink vertically
/// @param dialog pointer to the parent window
/// @param window pointer to the object to be resized
/// @param width the new width in pixels
/// @param height the new height in pixels
///
void ResizeDlg::Resize(CWnd *dialog, CWnd *window, int width, int height)
{
	MoveData	data;
	int			dy;

	m_dialog = dialog;
	m_window = window;

	//
	// Get the difference in size between the window and the desired width and height
	//
	GetImageWindowDelta(width, height);

	if (m_delta.cx != 0 || m_delta.cy != 0)
	{
		//
		// Resize the window
		//
		OffsetWindowSize(window, m_delta.cx, m_delta.cy);

		if (m_delta.cx < -16)
			m_delta.cx += 16;

		//
		// Resize the dialog.  It can grow or shrink in x, but
		// its height may not be less than its original height.
		// 
		dy = m_delta.cy > 0 ? m_delta.cy : 0;
		OffsetWindowSize(dialog, m_delta.cx, dy);		// resize the dialog

		//
		// Center the window vertically in the dialog
		//
		CenterWindow(dialog, window, false, true);

		//
		// Move the rest of the items dx pixels to the left or right.  We're not 
		// going to change the y coordinates, so we set m_delta.cy to 0.
		//
		m_delta.cy = 0;

		//
		// Move all of the dialog items to compensate for the new size of 'window'.  EnumChildWindows() 
		// will call OffsetChildPos() for each of the Dialog's child windows (i.e. all dialog items).  
		// We want to move every item except for the image item.  
		//
		data.delta  = &m_delta;
		data.skip   = window->m_hWnd;	// we don't want to move 'window'
		data.dialog = dialog->m_hWnd;	// handle to the dialog window (we need this for converting coordinates)
		::EnumChildWindows(dialog->m_hWnd, OffsetChildPos, (LPARAM)&data);
	}
}

///
/// Calculates the differences in window height and width between the current and new sizes.
///
void ResizeDlg::GetImageWindowDelta(int width, int height)
{
	int	itemWidth,  itemHeight;


	//
	// Get the current width and height of the image dialog item
	//
	GetWindowSize(m_window, &itemWidth, &itemHeight);

	//
	// Get the difference in the width and height.
	// This is how much the sizes of the image item and dialog need to be increased.
	// This is also how much we need to move the other dialog items to accommodate the new image size.
	//
	m_delta.cx = width  - itemWidth;
	m_delta.cy = height - itemHeight;
}

///
/// Changes the window size
///
void ResizeDlg::OffsetWindowSize(CWnd *wnd, int dx, int dy)
{
	int width, height;

	GetWindowSize(wnd, &width, &height);
	wnd->SetWindowPos(NULL, 0, 0, width + dx, height + dy, SWP_NOMOVE | SWP_NOZORDER);
}

///
/// Changes the window position
///
void ResizeDlg::OffsetWindowPos(CWnd *parent, CWnd *wnd, int dx, int dy)
{
	if (wnd != NULL)
	{
		CRect	rect;
		wnd->GetWindowRect(&rect);			// current window location in screen coordinates
		parent->ScreenToClient(&rect);		// current location in client coordinates (relative to client rect of parent)
		rect.OffsetRect(dx, dy);			// offset the rect by (dx, dy)
		wnd->MoveWindow(&rect, TRUE);		// move the window
	}
}


BOOL CALLBACK ResizeDlg::OffsetChildPos(HWND hwnd, LPARAM lParam)
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

		width  = rect.right  - rect.left;
		height = rect.bottom - rect.top;

		::MoveWindow(hwnd, ul.x + data->delta->cx, ul.y + data->delta->cy, width, height, TRUE);
	}

	return TRUE;
}

//
// Gets the window center and returns it as CPoint
//
void ResizeDlg::GetWindowCenter(CWnd *wnd, CRect *bnds, CPoint *center)
{
	if (wnd != NULL && center != NULL)
	{
		CRect	rect;
		CRect	*r = (bnds != NULL) ? bnds : &rect;

		wnd->GetWindowRect(r);
		*center = r->CenterPoint();
	}
}

///
/// A function that acquires either the window or client size. Note that
/// the client size does not include the window border, whereas the window
/// size does.
/// @param *wnd the window
/// @param *width pointer to an int into which the width is stored
/// @param *height pointer to an int into which the height is stored
/// @param *bnds [OPTIONAL] a CRect to store the window dimensions; supply NULL if not desired
/// @param client Supply TRUE if the client size is desired, FALSE if the window size is desired
///
void ResizeDlg::GetWindowSize(CWnd *wnd, int *width, int *height, CRect *bnds, bool client)
{
	if (wnd != NULL)
	{
		CRect	rect;

		CRect *r = (bnds == NULL) ? &rect : bnds;

		if (client)
		{
			wnd->GetClientRect(r);
		}
		else
		{
			wnd->GetWindowRect(r);
		}

		if (width != NULL)
		{
			*width  = r->Width();
		}

		if (height != NULL)
		{
			*height = r->Height();
		}
	}
}

#if 0
void ResizeDlg::CenterWindow(CWnd *parent, CWnd *wnd, bool horizontal, bool vertical)
{
	if (wnd != NULL)
	{
		CRect	rect;
		CPoint	pos, center;
		int		dx, dy;

		GetWindowCenter(parent, NULL, &center);		// center of parent window in screen coordinates
		GetWindowCenter(wnd, &rect, &pos);			// center of wnd in screen coordinates
		dx = horizontal ? center.x - pos.x : 0;
		dy = vertical   ? center.y - pos.y : 0;
		rect.OffsetRect(dx, dy);					// move the rect so that its center is at (center.x, center.y) in screen coordinates
		parent->ScreenToClient(&rect);				// make coordinates relative to client rect of parent
		wnd->MoveWindow(&rect, TRUE);				// move the window
	}
}
#else
///
/// A function that centers an object with respect to the parent with the option to select only vertical or horizontal centering.
/// @param *parent pointer to the parent window
/// @param *wnd pointer to the object to be centered
/// @param horizontal set to TRUE if horizontal centering is desired
/// @param vertical set to TRUE if vertical centering is desired
///
void ResizeDlg::CenterWindow(CWnd *parent, CWnd *wnd, bool horizontal, bool vertical)
{
	if (wnd != NULL)
	{
		CRect	r, rect, client;
		CPoint	ul;
		int		width, height, w, h;

		//
		// Get the width, height, and screen coordinates of the windows
		//
		GetWindowSize(parent, &w,     &h,      &r, true);	// r = client rect of parent
		GetWindowSize(wnd,    &width, &height, &rect,false);		// rect is in screen coordinates

		//
		// Convert the upper left coordinate of wnd to local coordinates (relative to parent)
		//
		ul.x = rect.left;
		ul.y = rect.top;
		parent->ScreenToClient(&ul);

		if (horizontal)
		{
			ul.x = (w - width)  / 2;	// local coordinate
		}
		
		if (vertical)
		{
			ul.y = (h - height) / 2;	// local coordinate
		}

		//
		// For some reason the x and y coordinates are off by a factor of 2, so I'm adjusting here.
		// I don't know why that is, but I did a screen capture and measured the window positions
		// in a paint program, and they were twice as much as they should have been.
		//

		if (horizontal)
		{
			ul.x /= 2;
		}

		if (vertical)
		{
			ul.y /= 2;
		}

		wnd->MoveWindow(ul.x, ul.y, width, height,true);
		//wnd->SetWindowPos(NULL, ul.x, ul.y, width, height, SWP_NOSIZE|SWP_NOZORDER);

	}
}
#endif

///
/// Resizes an object within a parent window that allows the object to shrink vertically, but not below a minimum size.
/// @param *dialog pointer to the parent window
/// @param *window pointer to the object to be resized
/// @param width the new width in pixels
/// @param height the new height in pixels
///
void ResizeDlg::BidirectionalResize(CWnd *dialog, CWnd *window, int width, int height)
{
	MoveData	data;
	int			dy;
	int	clientWidth,  clientHeight;
	int screenWidth, screenHeight;
	m_dialog = dialog;
	m_window = window;
	GetWindowSize(dialog, &clientWidth, &clientHeight,NULL,true); 
	GetWindowSize(dialog, &screenWidth, &screenHeight); 
	//
	// Get the difference in size between the window an the desired width and height
	//
	GetImageWindowDelta(width, height);

	if (m_delta.cx != 0 || m_delta.cy != 0)
	{
		//
		// Resize the window
		//
		OffsetWindowSize(window, m_delta.cx, m_delta.cy);

		if (m_delta.cx < -16)
			m_delta.cx += 16;

		//
		// Resize the dialog.  It can grow or shrink in x uncontrolled, but
		// its height may not be less than a minimum value. Also, if we need to
		// resize, it shouldn't make the window needlessly large.
		// 
		int border = 16;
		int min_window_size = 660;
		if ( m_delta.cy >= 0) // only change size if needed
		{
			if (clientHeight < height)
			{
				dy = height - clientHeight+border;
			}
			else
			{
				dy = 0;
			}

		}
		else if ( clientHeight + m_delta.cy> min_window_size) // allow window to shrink if it won't go below minimum size
		{
			dy = m_delta.cy;
		}
		else
		{
			dy = min_window_size - screenHeight+(screenHeight - clientHeight);	// shrink size to minimum size + width of the 
		}																		// screen border (screenHeight - clientHeight)
		OffsetWindowSize(dialog, m_delta.cx, dy);
		//
		// Center the window vertically in the dialog
		//
		CenterWindow(dialog, window, false, true);

		//
		// Move the rest of the items dx pixels to the left or right.  We're not 
		// going to change the y coordinates, so we set m_delta.cy to 0.
		//
		m_delta.cy = 0;

		//
		// Move all of the dialog items to compensate for the new size of 'window'.  EnumChildWindows() 
		// will call OffsetChildPos() for each of the Dialog's child windows (i.e. all dialog items).  
		// We want to move every item except for the image item.  
		//
		data.delta  = &m_delta;
		data.skip   = window->m_hWnd;	// we don't want to move 'window'
		data.dialog = dialog->m_hWnd;	// handle to the dialog window (we need this for converting coordinates)
		::EnumChildWindows(dialog->m_hWnd, OffsetChildPos, (LPARAM)&data);
	}
}
