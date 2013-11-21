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
#include "afxmt.h"

///
/// a buffer class where all access (read or write) to the buffer is protected by critical sections
///
class CriticalBuf : public CCriticalSection
{
public:
	CriticalBuf(void);
	~CriticalBuf(void);

	void	Get(void *buf);
	void	Set(void *buf);
	bool	Init(unsigned int bufLen);

private:
	byte			*m_buf;		//!< pointer to the buffer
	unsigned int	m_bufLen;	//!< length of m_buf in bytes
};

