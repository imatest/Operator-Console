#include "logger.h"

CLogRecord::CLogRecord(LEVEL level, CString loggerName,
	CString message, CString sourceFile, DWORD sourceLine)
{
	if (level == LEVEL_OFF || level == LEVEL_ALL)
	{
		ASSERT(FALSE);
	}
	m_level = level;
	m_loggerName = loggerName;
	m_message = message;
	ftime(&m_timeStamp);
	m_sourceFile = sourceFile;
	m_sourceLine = sourceLine;
	m_threadID = GetCurrentThreadId();
	m_processID = GetCurrentProcessId();
}

CString CLogRecord::GetFormatedMessage(void)
{
	CString	result;

	char	tempBuffer[256];

	result += "\nLogger name: ";
	result += m_loggerName + "\n";
	result += "Level: ";
	result += levelNames[m_level];
	result += "\n";
	struct tm*	tempTm = localtime(&m_timeStamp.time);
	strftime(tempBuffer, 255, "%d/%m/%Y %H:%M:%S.", tempTm);
	result += "Time: ";
	result += tempBuffer;
	result += itoa((int) m_timeStamp.millitm, tempBuffer, 10);
	result += "\n";
	result += "Sequence: ";
	result += itoa(m_sequenceNumber, tempBuffer, 10);
	result += "\n";
	result += "Source file: ";
	result += m_sourceFile + "\n";
	result += "Source line: ";
	result += itoa(m_sourceLine, tempBuffer, 10);
	result += "\n";
	result += "Thread ID: ";
	result += itoa(m_threadID, tempBuffer, 10);
	result += "\n";
	result += "Process ID: ";
	result += itoa(m_processID, tempBuffer, 10);
	result += "\n";
	result += "Message: ";
	result += m_message + "\n";

	return result;
}

CFileLogHandler::CFileLogHandler(LEVEL level, CString fileName,
	DWORD maxLength, BOOL console) : CLogHandler(level)
{
	m_fileName = fileName;
	m_maxLength = maxLength;
	m_console = console;
	m_currentFileCount = 0;
};

BOOL CFileLogHandler::Publish(CLogRecord& record)
{
	if (GetLevel() == LEVEL_OFF)
	{
		return TRUE;
	}
	if (record.GetLevel() < GetLevel())
	{
		return TRUE;
	}
	record.SetSequenceNumber(m_sequenceNumber++);
	int	fileHandler;
	fileHandler = m_console ? 2 : -1;

	if (fileHandler < 0)
	{
		while (1)
		{
			CString	str;
			CTime	temp = CTime::GetCurrentTime();
			str.Format("%s_%d_%d_%d.log", m_fileName,
					temp.GetYear(), temp.GetDay(),
					m_currentFileCount);
			if (fileHandler < 0)
			{
				fileHandler = _sopen(str,
								_O_CREAT |
								_O_APPEND |
								_O_BINARY |
								_O_WRONLY,
								_SH_DENYNO, _S_IREAD |
								_S_IWRITE);
			}
			if (fileHandler < 0)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			struct _stat	statistics;
			if (_fstat(fileHandler, &statistics) < 0)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			if ((DWORD) statistics.st_size >= m_maxLength)
			{
				_close(fileHandler);
				fileHandler = -1;
				m_currentFileCount++;
			}
			else
			{
				break;
			}
		}
	}
	CString	buffer = record.GetFormatedMessage();
	if (_write(fileHandler, buffer.GetBuffer(buffer.GetLength()),
			buffer.GetLength()) != buffer.GetLength())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (fileHandler != 2)
	{
		_close(fileHandler);
	}
	return TRUE;
}

CWndLogHandler::CWndLogHandler(LEVEL level, CString title) : CLogHandler(level)
{
	m_title = title;
	m_windowThread = INVALID_HANDLE_VALUE;
	m_hWnd = NULL;
	m_listbox = NULL;
	DWORD					ID;

	INITCOMMONCONTROLSEX	InitCtrls;
	InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
	InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	BOOL	bRet = InitCommonControlsEx(&InitCtrls);
	if (bRet != TRUE)
	{
		ASSERT(FALSE);
		return;
	}


	m_windowThread = CreateThread(NULL, 0, WindowThread, this, 0,
						&ID);
	if (m_windowThread == INVALID_HANDLE_VALUE)
	{
		ASSERT(FALSE);
	}
}

BOOL CWndLogHandler::Publish(CLogRecord& record)
{
	if (GetLevel() == LEVEL_OFF)
	{
		return TRUE;
	}
	if (record.GetLevel() < GetLevel())
	{
		return TRUE;
	}
	record.SetSequenceNumber(m_sequenceNumber++);

	char	tempBuffer[256];
	LV_ITEM	item;
	item.mask = LVIF_TEXT;
	item.iItem = ListView_GetItemCount(m_listbox);


	CString	str = record.GetLoggerName();
	item.iSubItem = 0;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_InsertItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str = levelNames[record.GetLevel()];
	item.iSubItem = 1;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	timeb		tempTimeb = record.GetTimeStamp();
	struct tm*	tempTm = localtime(&tempTimeb.time);
	strftime(tempBuffer, 255, "%Y/%m/%d %H:%M:%S.", tempTm);
	str = tempBuffer;
	str += itoa((int) tempTimeb.millitm, tempBuffer, 10);
	item.iSubItem = 2;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str.Format("%u", record.GetSequenceNumber());
	item.iSubItem = 3;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str = record.GetSourceFile();
	item.iSubItem = 4;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str.Format("%u", record.GetSourceLine());
	item.iSubItem = 5;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str.Format("%u", record.GetThreadID());
	item.iSubItem = 6;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str.Format("%u", record.GetProcessID());
	item.iSubItem = 7;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	str = record.GetMessage();
	item.iSubItem = 8;
	item.pszText = str.GetBuffer(str.GetLength());
	if (ListView_SetItem(m_listbox, &item) < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CWndLogHandler::RegisterLoggerWindow(void)
{
	char		szWindowClass[] = "WndLogHandler";
	WNDCLASSEX	wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC) WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = AfxGetInstanceHandle();
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	unsigned short temp = RegisterClassEx(&wcex);

	if (temp == 0)
	{
		return GetLastError() == 1410 ? TRUE : FALSE; //GetLastError==1410 - class already registered
	}
	else
	{
		return TRUE;
	}
}

LRESULT CALLBACK CWndLogHandler::WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD WINAPI CWndLogHandler::WindowThread(void* p)
{
	CWndLogHandler*	wndLogHandler = (CWndLogHandler*) p;
	MSG				msg;
	char			szWindowClass[] = "WndLogHandler";

	if (wndLogHandler->RegisterLoggerWindow() != TRUE)
	{
		ASSERT(FALSE);
		return -1;
	}



	wndLogHandler->m_hWnd = CreateWindow(szWindowClass,
								wndLogHandler->m_title,
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT,
								0, NULL, NULL,
								AfxGetInstanceHandle(), NULL);
	if (!wndLogHandler->m_hWnd)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ShowWindow(wndLogHandler->m_hWnd, SW_SHOW);
	UpdateWindow(wndLogHandler->m_hWnd);



	CRect	rect;
	GetClientRect(wndLogHandler->m_hWnd, &rect);

	wndLogHandler->m_listbox = CreateWindow(WC_LISTVIEW,
								"Messages",
								WS_VISIBLE |
								WS_CHILD |
								WS_BORDER |
								LVS_REPORT |
								LVS_EDITLABELS |
								WS_EX_CLIENTEDGE,
								0, 0, rect.Width(),
								rect.Height(),
								wndLogHandler->m_hWnd, NULL,
								AfxGetInstanceHandle(), NULL);
	if (!wndLogHandler->m_listbox)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ShowWindow(wndLogHandler->m_listbox, SW_SHOW);
	UpdateWindow(wndLogHandler->m_listbox);

	SendMessage(wndLogHandler->m_listbox,
		LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

	char		columns[][80] =
	{
		"Logger name", "Level", "Time", "Sequence number",
		"Source file", "Source line", "Thread ID", "Process ID",
		"Message"
	};

	LV_COLUMN	col;
	for (int i = 0; i < 9; i++)
	{
		col.mask = LVCF_FMT |
			LVCF_WIDTH |
			LVCF_TEXT |
			LVCF_SUBITEM;
		col.fmt = LVCFMT_CENTER; // left-align column
		col.cx = 75; // width of column in pixels
		col.pszText = columns[i];
		col.iSubItem = i;
		if (ListView_InsertColumn(wndLogHandler->m_listbox, i,
				&col) == -1)
		{
			ASSERT(FALSE);
			return -1;
		}
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

CSocketLogHandler::CSocketLogHandler(LEVEL level, CString host,
	unsigned short port, BOOL keepAlive) : CLogHandler(level)
{
	WSADATA	temp;
	WSAStartup(0x0000, &temp);
	WSAStartup(temp.wHighVersion, &temp);

	m_host = host;
	m_port = port;
	m_keepAlive = keepAlive;
	m_connectionRefused = false;

	m_connection = INVALID_SOCKET;
	memset(&m_loggerAddress, 0, sizeof(sockaddr_in));

	m_loggerAddress.sin_family = AF_INET;
	m_loggerAddress.sin_port = htons(m_port);

	hostent*resolvedHost;
	resolvedHost = gethostbyname(m_host);
	if (resolvedHost != NULL)
	{
		memcpy(&m_loggerAddress.sin_addr.S_un.S_addr,
			resolvedHost->h_addr_list[0], 4);
	}
	else
	{
		int	i = WSAGetLastError();
	}
}

CSocketLogHandler::~CSocketLogHandler()
{
	shutdown(m_connection, 0x02);
	closesocket(m_connection);
}

BOOL CSocketLogHandler::Publish(CLogRecord& record)
{
	if (GetLevel() == LEVEL_OFF)
	{
		return TRUE;
	}
	if (record.GetLevel() < GetLevel())
	{
		return TRUE;
	}
	record.SetSequenceNumber(m_sequenceNumber++);
	if (m_connectionRefused)
	{
		return false;
	}

	CString	str = record.GetFormatedMessage();
	int		sendResult = send(m_connection,
							str.GetBuffer(str.GetLength()),
							str.GetLength(), 0);
	if (sendResult != str.GetLength())
	{
		if ((WSAGetLastError() == WSAENOTCONN) ||
			(WSAGetLastError() == WSAENOTSOCK))
		{
			if (GetConnection())
			{
				sendResult = send(m_connection,
								str.GetBuffer(str.GetLength()),
								str.GetLength(), 0);
				if (sendResult != str.GetLength())
				{
					return FALSE;
				}
				else
				{
					if (m_keepAlive == FALSE)
					{
						closesocket(m_connection);
					}
					return true;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (m_keepAlive == FALSE)
		{
			closesocket(m_connection);
		}
		return TRUE;
	}
}

BOOL CSocketLogHandler::GetConnection(void)
{
	m_connection = socket(AF_INET, SOCK_STREAM, 0);
	if (m_connection != INVALID_SOCKET)
	{
		if (connect(m_connection, (sockaddr *) &m_loggerAddress,
				sizeof(sockaddr)) != 0)
		{
			if (WSAGetLastError() == WSAECONNREFUSED)
			{
				m_connectionRefused = TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CLogger::CLogger(CString loggerName)
{
	m_loggerName = loggerName;
	for (int i = 0; i < MAX_LOG_HANDLERS_NUMBER; i++)
	{
		m_logHandlers[i] = NULL;
	}
}

BOOL CLogger::log(LEVEL level, CString sourceFile,
	DWORD sourceLine, CString message)
{
	CLogRecord	record(level, this->m_loggerName, message,
		sourceFile, sourceLine);
	for (int i = 0; i < MAX_LOG_HANDLERS_NUMBER; i++)
	{
		if (m_logHandlers[i] != NULL)
		{
			if (m_logHandlers[i]->Publish(record) != TRUE)
			{
				ASSERT(FALSE);
			}
		}
	}
	return TRUE;
}

void CLogger::AddHandler(CLogHandler* logHandler)
{
	int	i;
	for (i = 0; i < MAX_LOG_HANDLERS_NUMBER; i++)
	{
		if (m_logHandlers[i] == logHandler)
		{
			return;
		}
	}
	for (i = 0; i < MAX_LOG_HANDLERS_NUMBER; i++)
	{
		if (m_logHandlers[i] == NULL)
		{
			m_logHandlers[i] = logHandler;
			return;
		}
	}
}

CLogHandler* CLogger::GetHandlerAt(DWORD index)
{
	if (index >= MAX_LOG_HANDLERS_NUMBER)
	{
		return NULL;
	}
	return m_logHandlers[index];
}

CLogHandler* CLogger::RemoveHandlerAt(DWORD index)
{
	if (index >= MAX_LOG_HANDLERS_NUMBER)
	{
		return NULL;
	}
	CLogHandler*result = m_logHandlers[index];
	m_logHandlers[index] = NULL;
	return result;
}