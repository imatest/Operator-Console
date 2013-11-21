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
///
/// A class used to hold a bitmap image for display
///
class CStaticBmp : public CStatic
{
public:
	CStaticBmp(void);
	~CStaticBmp(void);

	void Draw(const CWnd *parent);
	bool Init();
	void Update(void *buf);

protected:
	CDC				m_cdc;			//!< memory drawing context to hold image
	CBitmap			m_bmp;			//!< bitmap for m_cdc
	int				m_width;
	int				m_height;
	void			*m_buf;
};

