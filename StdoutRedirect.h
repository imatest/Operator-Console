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

#include <stdio.h>
///
/// Defines functions for redirecting stdout through a buffered pipe
///
class StdoutRedirect
{
    public:
#if 0
		StdoutRedirect(int bufferSize);
#endif
		StdoutRedirect();
        virtual ~StdoutRedirect();
	
		void	Close();
		int		Init(int bufferSize);
        int		Start();
        int		Stop();
        int		GetBuffer(char *buffer, int size);

	protected:
		FILE	*file;	//!< stdout (subclasses can set this to something else, e.g. stderr)

	private:
		int		fdStdOutPipe[2];
        int		fdStdOut;
		bool	isOpen;
};
