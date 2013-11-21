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

#include "StaticBmp.h"


CStaticBmp::CStaticBmp(void)
{
	m_buf    = NULL;
	m_width  = 0;
	m_height = 0;
}

CStaticBmp::~CStaticBmp(void)
{
}

bool CStaticBmp::Init()
{
	CDC		*cdc = GetDC();
	CRect	rect;
	bool	success;


	//
	// Create a memory drawing context and bitmap the size of the window
	//
	GetWindowRect(&rect);
	m_width  = rect.Width();
	m_height = rect.Height();

	if (m_cdc.CreateCompatibleDC(cdc))
	{
		if (m_bmp.CreateCompatibleBitmap(cdc, m_width, m_height))
		{
			m_cdc.SelectObject(&m_bmp);
			success = true;
		}
	}

	return success;
}

void CStaticBmp::Update(void *buf)
{
	m_buf = buf;
	Invalidate();
}

void CStaticBmp::Draw(const CWnd *parent)
{
	CRect	rect;
	DWORD	numBytes = m_width * m_height * 4;

	GetWindowRect(&rect);
	parent->ScreenToClient(&rect);
	m_bmp.SetBitmapBits(numBytes, m_buf);
	CDC *cdc = GetDC();

	cdc->BitBlt(rect.left, rect.top, m_width, m_height, &m_cdc, 0, 0, SRCCOPY);

}
