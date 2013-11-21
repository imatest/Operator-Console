
// AppThread.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AppThread.h"
#include "OperatorConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// AppThread
BEGIN_MESSAGE_MAP(AppThread, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// AppThread construction

AppThread::AppThread()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}

AppThread::~AppThread()
{
}


// The one and only AppThread object

AppThread theApp;

// AppThread initialization


BOOL AppThread::InitInstance()
{
#if 0
	//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	AfxInitRichEdit();	// this must be called once before displaying a dialog that uses RichEdit control

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
#else
	CWinApp::InitInstance();
#endif

	CreatePipes();

	COperatorConsoleApp *thread = (COperatorConsoleApp *)AfxBeginThread(RUNTIME_CLASS(COperatorConsoleApp), 0, CREATE_SUSPENDED);
//	thread->m_appInstance = hInstance;
	thread->ResumeThread();
	::WaitForSingleObject(thread->m_hThread, INFINITE);

	// Delete the shell manager created above.
#if 0
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
#endif

	return FALSE;
}

void AppThread::CreatePipes() 
{ 
	SECURITY_ATTRIBUTES saAttr; 
 
	saAttr.nLength              = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle       = TRUE;
	saAttr.lpSecurityDescriptor = NULL; 

	// Create a pipe for the child process's STDOUT. 
	// Ensure the read handle to the pipe for STDOUT is not inherited.

	CreatePipe(&m_stdout_Rd, &m_stdout_Wr, &saAttr, 0);
	SetHandleInformation(m_stdout_Rd, HANDLE_FLAG_INHERIT, 0);

	CreatePipe(&m_stderr_Rd, &m_stderr_Wr, &saAttr, 0);
	SetHandleInformation(m_stderr_Rd, HANDLE_FLAG_INHERIT, 0);

	SetStdHandle(STD_OUTPUT_HANDLE, m_stdout_Wr);
	SetStdHandle(STD_ERROR_HANDLE,  m_stderr_Wr);
} 
