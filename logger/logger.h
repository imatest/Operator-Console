/*
 *  Created by Gavriloaie Eugen-Andrei
 *  e-mail: gavriloaie_andrei@yahoo.com
 *  
 *  Send me a mail telling me that you are
 *  using this classe if you can and want.
 *
 *  ANY observations will be appreciated
 *  This code was downloaded from https://www.codeguru.com/cpp/v-s/debug/logging/article.php/c4425/A-Logger-Makes-Your-Life-Easier.htm
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <afxwin.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <afxsock.h>
#include <sys/timeb.h>

#define MAX_LOG_HANDLERS_NUMBER 128


/*
This is a copy-paste from the java documentation.
It describes the use of logging levels

---------------------------------------------------------------
SEVERE
is a message level indicating a serious failure. 
In general SEVERE messages should describe events that are of
considerable importance and which will prevent normal program 
execution. They should be reasonably intelligible to end users 
and to system administrators.
---------------------------------------------------------------
WARNING
is a message level indicating a potential problem. 
In general WARNING messages should describe events that will
be of interest to end users or system managers, or which
indicate potential problems.
---------------------------------------------------------------
INFO
is a message level for informational messages. 
Typically INFO messages will be written to the console or its 
equivalent. So the INFO level should only be used for
reasonably significant messages that will make sense to end
users and system admins.
---------------------------------------------------------------
CONFIG
is a message level for static configuration messages.
CONFIG messages are intended to provide a variety of static
configuration information, to assist in debugging problems
that may be associated with particular configurations.
For example, CONFIG message might include the CPU type, the
graphics depth, the GUI look-and-feel, etc.
---------------------------------------------------------------
FINE
is a message level providing tracing information. 
All of FINE, FINER, and FINEST are intended for relatively
detailed tracing. The exact meaning of the three levels will
vary between subsystems, but in general, FINEST should be used
for the most voluminous detailed output, FINER for somewhat
less detailed output, and FINE for the lowest volume (and most
important) messages. 
In general the FINE level should be used for information that
will be broadly interesting to developers who do not have a
specialized interest in the specific subsystem. 
FINE messages might include things like minor (recoverable)
failures. Issues indicating potential performance problems are
also worth logging as FINE.
---------------------------------------------------------------
FINER
indicates a fairly detailed tracing message. By default
logging calls for entering, returning, or throwing an
exception are traced at this level.
---------------------------------------------------------------
FINEST
public static final Level FINESTFINEST indicates a highly
detailed tracing message.
---------------------------------------------------------------
*/

typedef enum _level_
{
	LEVEL_ALL,
	LEVEL_FINEST,
	LEVEL_FINER,
	LEVEL_FINE,
	LEVEL_CONFIG,
	LEVEL_INFO,
	LEVEL_WARNING,
	LEVEL_SEVERE,
	LEVEL_OFF
}																					LEVEL;

static char	levelNames[][8] =
{
	"ALL", "FINEST", "FINER", "FINE", "CONFIG", "INFO",
	"WARNING", "SEVERE", "OFF"
};

class CLogRecord
{
private:
	LEVEL	m_level;
	CString	m_loggerName;
	CString	m_message;
	timeb	m_timeStamp;
	DWORD	m_sequenceNumber;
	CString	m_sourceFile;
	DWORD	m_sourceLine;
	DWORD	m_threadID;
	DWORD	m_processID;
public:
	CLogRecord(LEVEL level, CString loggerName, CString message,
		CString sourceFile, DWORD sourceLine);

	LEVEL GetLevel()
	{
		return m_level;
	};

	CString GetLoggerName()
	{
		return m_loggerName;
	};

	CString GetMessage()
	{
		return m_message;
	};

	CString GetFormatedMessage();

	timeb GetTimeStamp()
	{
		return m_timeStamp;
	};

	DWORD GetSequenceNumber()
	{
		return m_sequenceNumber;
	};

	void SetSequenceNumber(DWORD sequenceNumber)
	{
		m_sequenceNumber = sequenceNumber;
	};

	CString GetSourceFile()
	{
		return m_sourceFile;
	};

	DWORD GetSourceLine()
	{
		return m_sourceLine;
	};

	DWORD GetThreadID()
	{
		return m_threadID;
	};

	DWORD GetProcessID()
	{
		return m_processID;
	};
};

class CLogHandler
{
private:
	LEVEL	m_currentLevel;
protected:
	DWORD	m_sequenceNumber;
public:
	CLogHandler(LEVEL level)
	{
		m_sequenceNumber = 0;
		m_currentLevel = level;
	};

	void SetLevel(LEVEL level)
	{
		m_currentLevel = level;
	};

	LEVEL GetLevel(void)
	{
		return m_currentLevel;
	};

	virtual BOOL Publish(CLogRecord& record) = 0;
};

class CFileLogHandler : public CLogHandler
{
private:
	CString	m_fileName;
	DWORD	m_maxLength;
	BOOL	m_console;
	DWORD	m_currentFileCount;
public:
	CFileLogHandler(LEVEL level, CString fileName,
		DWORD maxLength = 1048576, BOOL console = FALSE);
	BOOL Publish(CLogRecord& record);
};

class CWndLogHandler : public CLogHandler
{
private:
	HANDLE	m_windowThread;
	HWND	m_hWnd;
	HWND	m_listbox;
	CString	m_title;
private:
	BOOL RegisterLoggerWindow(void);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
		WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI WindowThread(void*);
public:
	CWndLogHandler(LEVEL level, CString title);
	BOOL Publish(CLogRecord& record);
};

class CSocketLogHandler : public CLogHandler
{
private:
	CString			m_host;
	unsigned short	m_port;
	BOOL			m_keepAlive;
	sockaddr_in		m_loggerAddress;
	SOCKET			m_connection;
	BOOL			m_connectionRefused;
private:
	BOOL GetConnection(void);
public:
	CSocketLogHandler(LEVEL level, CString host,
		unsigned short port, BOOL keepAlive = TRUE);
	~CSocketLogHandler();
	BOOL Publish(CLogRecord& record);
};

class CLogger
{
private:
	CString		m_loggerName;
	CLogHandler*m_logHandlers[MAX_LOG_HANDLERS_NUMBER];
public:
	CLogger(CString loggerName);
	BOOL log(LEVEL level, CString sourceFile, DWORD sourceLine,
		CString message);
	void AddHandler(CLogHandler* logHandler);
	CLogHandler* GetHandlerAt(DWORD index);
	CLogHandler* RemoveHandlerAt(DWORD index);
};


#define FINEST_LOG(logger,message) \
{\
  (logger).log(LEVEL_FINEST,__FILE__,__LINE__,message);\
};

#define FINER_LOG(logger,message) \
{\
  (logger).log(LEVEL_FINER,__FILE__,__LINE__,message);\
};

#define FINE_LOG(logger,message) \
{\
  (logger).log(LEVEL_FINE,__FILE__,__LINE__,message);\
};

#define CONFIG_LOG(logger,message) \
{\
  (logger).log(LEVEL_CONFIG,__FILE__,__LINE__,message);\
};

#define INFO_LOG(logger,message) \
{\
  (logger).log(LEVEL_INFO,__FILE__,__LINE__,message);\
};

#define WARNING_LOG(logger,message) \
{\
  (logger).log(LEVEL_WARNING,__FILE__,__LINE__,message);\
};

#define SEVERE_LOG(logger,message) \
{\
  (logger).log(LEVEL_SEVERE,__FILE__,__LINE__,message);\
};

#endif //ifndef LOGGER_H