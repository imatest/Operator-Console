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
#include <string>

///
/// A null-terminated string class for which all access is controlled by critical sections
///
class CriticalCString :	public CCriticalSection
{
public:
	CriticalCString(void);
	virtual ~CriticalCString(void);
	bool	Append(CString &str);
	bool	Append(const char *str, int len);
	void	Clear();
	void	Get(CString &str, bool clear=false);	//!< copy m_string into str, then optionally clear m_string
	void	Set(CString &str);
	void	Set(std::string &str);
	void	Set(const char *str);
	void	Set(const char *str, int len);

private:
	CString	m_string;
};

