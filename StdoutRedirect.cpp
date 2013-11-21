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

#include <windows.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#include "StdoutRedirect.h"

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

#define READ_FD 0
#define WRITE_FD 1

#define CHECK(a) if ((a)!= 0) return -1;


StdoutRedirect::~StdoutRedirect()
{
#if 0
	_close(fdStdOut);
    _close(fdStdOutPipe[WRITE_FD]);
    _close(fdStdOutPipe[READ_FD]);
#else
	Close();
#endif
}

StdoutRedirect::StdoutRedirect()
{
	isOpen = false;
	file = stdout;
}

#if 0
StdoutRedirect::StdoutRedirect(int bufferSize)
{
	file = stdout
	Init(bufferSize);
}
#endif

void StdoutRedirect::Close()
{
	if (isOpen)
	{
		_close(fdStdOut);
		_close(fdStdOutPipe[WRITE_FD]);
		_close(fdStdOutPipe[READ_FD]);
		isOpen = false;
	}
}

int StdoutRedirect::Init(int bufferSize)
{
	int error = _pipe(fdStdOutPipe, bufferSize, O_TEXT);

	if (!error)
    {
		fdStdOut = _dup(_fileno(file));
		isOpen   = true;
	}

	return error;

}

int StdoutRedirect::Start()
{
#if 0
	fflush( file );
#endif
	CHECK(_dup2(fdStdOutPipe[WRITE_FD], _fileno(file)));
    ios::sync_with_stdio();
    setvbuf( file, NULL, _IONBF, 0 ); // absolutely needed
    return 0;
}

int StdoutRedirect::Stop()
{
    CHECK(_dup2(fdStdOut, _fileno(file)));
    ios::sync_with_stdio();
    return 0;
}

int StdoutRedirect::GetBuffer(char *buffer, int size)
{
	int nOutRead = _read(fdStdOutPipe[READ_FD], buffer, size);	// this will block until there is data to read (could cause thread to hang)
    buffer[nOutRead] = '\0';

    return nOutRead;
}
