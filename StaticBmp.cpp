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
	m_buf = NULL;
	m_width = 0;
	m_height = 0;
	m_bmp = new CBitmap();
	m_dummy = NULL;
}

CStaticBmp::~CStaticBmp(void)
{

	CBitmap* old = m_cdc.SelectObject(m_dummy);
	VERIFY(m_bmp->DeleteObject());
	if (old != m_bmp)
		old->DeleteObject();
	if (m_bmp != NULL)
		delete m_bmp;
}

bool CStaticBmp::Init()
{
	CRect	rect;

	//
	// Initialize with the current CWnd width and height
	//
	GetWindowRect(&rect);
	m_width = rect.Width();
	m_height = rect.Height();

	return Init(rect.Width(), rect.Height());
}


bool CStaticBmp::Init(int width, int height) {

	CDC		*cdc = GetDC();
	bool	success;

	//
	// Create a memory drawing context and bitmap the size of the window
	//

	//CDC* p_cdc = m_cdc.FromHandle(m_cdc);
	//if (p_cdc != NULL || m_cdc.CreateCompatibleDC(cdc))
	if ( m_cdc.CreateCompatibleDC(cdc))
	{
		success = InitBitMap(cdc, width, height);
	}

	return success;
}

void CStaticBmp::Update(void *buf, int width, int height)
{
	m_buf = buf;

	if (m_width != width || m_height != height)
	{
		ReInitBitMap(width, height);
	}

	Invalidate();
}

bool CStaticBmp::ReInit()
{
	return ReInitBitMap(m_width, m_height);
}

bool CStaticBmp::InitBitMap(CDC* cdc, int width, int height)
{
	bool	success;

	m_width = width;
	m_height = height;

	if (m_bmp->CreateCompatibleBitmap(cdc, m_width, m_height))
	{
		m_dummy = m_cdc.SelectObject(m_bmp);
		success = true;
	}
	return success;
}

bool CStaticBmp::ReInitBitMap(int width, int height)
{
	bool success = false;
	CDC* cdc = GetDC();

	// Reinitialize m_bmp
	CBitmap* oldBitmap = m_cdc.SelectObject(m_dummy);
	if (oldBitmap != m_bmp) {
		VERIFY(oldBitmap->DeleteObject());
		delete oldBitmap;
	}

	if (m_bmp != NULL) {
		VERIFY(m_bmp->DeleteObject());
		delete m_bmp;
	}

	m_bmp = new CBitmap();

	success = InitBitMap(cdc, width, height);

	return success;
}

void CStaticBmp::Draw(const CWnd *parent)
{
	CRect	rect;
	DWORD	numBytes = m_width * m_height * 4;

	GetWindowRect(&rect);
	parent->ScreenToClient(&rect);
	m_bmp->SetBitmapBits(numBytes, m_buf);
	CDC *cdc = GetDC();

	cdc->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_cdc, 0, 0, m_width, m_height, SRCCOPY);

}
