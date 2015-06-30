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

///
/// OperatorConsole.cpp : Defines the class behaviors for the application.
///

#include "stdafx.h"
#include "Config.h"
#include "imatest_library.h"
#include "imatest_acquisition.h"
#include "OperatorConsole.h"
#include "OperatorConsoleDlg.h"
#include "Setup.h"
#include "PassFail.h"
#include "PasswordDialog.h"
#include "ImatestSourceIDs.h"
#include <sstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COperatorConsoleApp
BEGIN_MESSAGE_MAP(COperatorConsoleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)

END_MESSAGE_MAP()

///
/// Converts the entry<T>.value into a std::string to facilitate writing to the pass/fail file
///
template <typename T> const std::string writeEntValueString( const entry<T>& ent)
{
	stringstream ss;

	ss << ent.value;

	return ss.str();
}
///
/// Converts the entry<CString>.value into a std::string to facilitate writing to the pass/fail file
///
template <> const std::string writeEntValueString( const entry<CString>& ent)
{
	stringstream ss;

	ss << ent.value.GetString();

	return ss.str();
}
///
/// Converts the minMaxEntry<CString>.min_val and .max_val into a std::string to facilitate writing to the pass/fail file
///
template <typename T> const std::string writeMinMaxEntValueString(const minMaxEntry<T>& ent)
{
	stringstream ss;

	ss<< ent.min_val << " " << ent.max_val;
	return ss.str();
}

///
/// Converts the vecEntry<CString>.value into a single std::string to facilitate writing to the pass/fail file
///
template <typename T> const std::string writeVecEntValueString(const vecEntry<T>& ent)
{
	stringstream ss;

	for(std::size_t idx =0 ; idx < ent.value.size(); ++idx)
	{
		ss << ent.value[idx] << " ";
	}
	return ss.str();

}

///
/// Conditionally adds a particular key (variable name) and value to the std::vector<std::string> keys, vals if the particular key is in use by the pass/fail file
///
void addKeysAndValues(const std::string& candidateKey, const std::string& candidateVal, std::vector<std::string>& keys, std::vector<std::string>& vals, bool b_addKey)
{
	if (b_addKey)
	{
		keys.push_back(candidateKey);
		vals.push_back(candidateVal);
	}
}

///
/// Adds a data_type string and key string to the std::vector<std::string> data_types and keys, respectively
///
void addDataTypeAndKey(const std::string& candidateDT, const CString& candidateKey, std::vector<std::string>& data_types, std::vector<CString>& keys)
{
	data_types.push_back(candidateDT);
	keys.push_back(candidateKey);
}

/// COperatorConsoleApp construction

COperatorConsoleApp::COperatorConsoleApp()
{
   // support Restart Manager
   m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

   // Place all significant initialization in InitInstance
   m_acq               = NULL;
   m_test              = NULL;
   m_fileImage         = NULL;
   m_cameraImage       = NULL;
   m_jsonDlgThread     = NULL;
   m_config            = NULL;
   memset(&m_flags, 0, sizeof(m_flags));
   m_PFSettings.m_ini_file = _T(INI_FILENAME);
   m_PFSettings.m_ini_file.Remove('\n');
   m_PFSettings.m_ini_file.Remove('\r');
   m_password = _T(ADMIN_PASSWORD);
   m_passFailIsUnlocked = false; // Keep this initialized to false unless you want to completely disable password-protection for pass/fail settings!
   m_image_source = imatest_source;
   m_logFileName  = _T(LOG_FILENAME);
#if defined(STDIO_DEBUG)
   printf("Hello from constructor [stdout]\n");
   cout << "Hello from constructor [cout]" << endl;
#endif
}

COperatorConsoleApp::~COperatorConsoleApp()
{
	if (m_fileImage != NULL)
	{
		delete [] m_fileImage;
	}

	if (m_cameraImage != NULL)
	{
		delete [] m_cameraImage;
	}

}

/// The one and only COperatorConsoleApp object
COperatorConsoleApp theApp;




/// COperatorConsoleApp initialization

BOOL COperatorConsoleApp::InitInstance()
{
#if defined(STDIO_DEBUG)
	printf("Hello from %s [stdout]\n", __FUNCTION__);
	cout << "Hello from" << __FUNCTION__ << "[cout]" << endl;
#endif

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

	m_pMainWnd = NULL;

	InitOutput();	// this sets up a pipe for handling stdout results from the DLLs


#if defined(STDIO_DEBUG)
	fprintf(stdout, "Hello from stdout.\n");	// for debugging
	fprintf(stderr, "Hello from stderr.\n");	// for debugging
	cout << "Hello from cout." << endl;			// for debugging
#endif
	if (!InitLibs()) // InitLibs must be called before any calls to Imatest library functions
	{
		cerr << "Unable to initialize the Imatest library." << endl;
	}
  

	if (!ReadINISettings()) // if we're unable to read the ini file, we must generate image dimensions from user input before continuing
	{
		m_setup.width = 640;
		m_setup.height = 480;
		cout << "INI settings for Operator Console not found." << endl;
		cout << "Please click the 'Setup' button and fill in the" << endl;
		cout << "appropriate fields before proceeding." << endl;

	}
	if (!ReadPassFail())
	{

	}
	if (!Init())	// do our application-specific initialization
	{
		cout << "Application Initialization Failed" << endl;
		Quit();	// if init failed, clean up anything that was done in Init()
	}
	else
	{
      COperatorConsoleDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
			TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
		}

	}

	Quit(); // This was originally missing, but I think it should be here TDC
	CloseLibs();


	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


//////////////////////////////////////////////
///
/// Init         perform application-specific initialization
///
/// @params        none
///
/// @return value true if successful, false otherwise
///
/// Description
///    Init does the following things:
///    It checks all of the hard coded filenames to be sure that the files exist
///    It initializes the blemish and sfrplus file acquisition objects (loads the raw files into buffers)
///    It initializes the camera object
///    Allocates a buffer to hold a single image frame
///    Creates and starts the blemish and sfrplus threads
///    Creates and starts the modeless dialog thread
///    Initializes the MATLAB and ImatestIT libraries
///    Initializes the blemish and sfrplus modules
///
/// If any of the initialization steps fails, an appropriate message is displayed in an Alert Box.
/////////////////////////////////////////////
bool COperatorConsoleApp::Init()
{
	CString	errMsg;
	bool	success = false;

#ifdef IMATEST_CAMERA
	//m_width  = CAMERA_WIDTH;
	//m_height = CAMERA_HEIGHT;
#elif !defined FAKE_CAMERA
	m_width  = CAMERA_WIDTH;
	m_height = CAMERA_HEIGHT;
#else
	m_width  = RAW_WIDTH_BLEMISH;
	m_height = RAW_HEIGHT_BLEMISH;
#endif

	//
	// Initialize the Config object.  These values are used in the calls to blemish_shell() and sfrplus_shell().  
	// Right now they're all hard coded (they're defined in OperatorConsole.h)
	//
	m_configRAW.Init(INI_FILENAME, PROGRAMPATH, RAW_EXTENSION, FILE_ROOT, SERIAL_NUMBER, PART_NUMBER, 1);
	m_configRGB.Init(INI_FILENAME, PROGRAMPATH, RGB_EXTENSION, FILE_ROOT, SERIAL_NUMBER, PART_NUMBER, 3);



#if defined USE_RGB_DATA	
	m_config = &m_configRGB;
#else
	m_config = &m_configRAW;
#endif
	m_setup.ini_file = m_config->m_iniFilePathName;
	m_setup.program_path = m_config->m_programPath;
	m_setup.serial_number = m_config->m_serialNumber;
	m_setup.part_number = m_config->m_partNumber;
	if (!CheckFiles(errMsg))	// make sure that all of the hard coded files exist
	{
		success = false;
	}
	else if (!InitBlemishAcq())	// loads the raw blemish file into a buffer
	{
		errMsg = m_blemishAcq.GetInfo();
	}
	else if (!InitSFRplusAcq())	// loads the raw sfrplus file into a buffer
	{
		errMsg = m_sfrPlusAcq.GetInfo();
	}
	else if (!InitCamera())
	{
		errMsg = m_camera->GetInfo();
	}
	else if (!AllocateImageBuf())	// allocate a buffer large enough to hold an image [must be done after call to m_blemishAcq.Open()]
	{
		errMsg = "Unable to allocate image buffers.";
	}
	else if (!InitBlemishThread())
	{
		errMsg = "Unable to create Blemish thread.";
	}
	else if (!InitSFRplusThread())
	{
		errMsg = "Unable to create SFRplus thread.";
	}
	else if (!InitCameraThread())
	{
		errMsg = "Unable to create Capture thread.";
	}
	else if (!InitModelessThread())
	{
		errMsg = "Unable to create thread for modeless dialog.";
	}
	else
	{
		OnSetSFRplus(0, 0);	// the default test is set to SFRplus
		m_status = idle;

		success = true;
	}

	if (!errMsg.IsEmpty())
	{
		AfxMessageBox(errMsg);
	}
	return success;
}


bool COperatorConsoleApp::InitBlemishAcq()
{
	bool	success = false;

	if (m_blemishAcq.Init(RAW_WIDTH_BLEMISH, RAW_HEIGHT_BLEMISH, RAW_BLEMISH_IMAGE))
	{
		success = m_blemishAcq.Open();
	}

	return success;
}

bool COperatorConsoleApp::InitSFRplusAcq()
{
	bool	success = false;

	if (m_sfrPlusAcq.Init(RAW_WIDTH_SFRPLUS, RAW_HEIGHT_SFRPLUS, RAW_SFRPLUS_IMAGE))
	{
		success = m_sfrPlusAcq.Open();
	}

	return success;
}

bool COperatorConsoleApp::InitCamera()
{
	bool	success = false;

	if (m_image_source==imatest_source)
	{
		m_camera = &m_imatest_cam;
		if (m_camera->Init(m_setup.width, m_setup.height, 4))
		{
			success = m_camera->Open();
			m_camera->m_device_ID = m_setup.epiphan_deviceID;
			m_camera->m_source_ID = m_setup.sourceID;
			m_camera->m_ini_file.assign(m_setup.ini_file);
		}

	}
	else if (m_image_source==directshow_source)
	{
		m_camera = &m_directshow_cam;
		if (m_camera->Init(m_setup.width, m_setup.height, 4))
		{
			success = m_camera->Open();
			m_camera->m_device_ID = m_setup.epiphan_deviceID;
			m_camera->m_source_ID = m_setup.sourceID;
			m_camera->m_ini_file.assign(m_setup.ini_file);
		}

	}
   else if (m_image_source==file_source)
   {
      //    m_camera = &m_file_cam;
      //    m_camera = &m_file_cam;
      //success = m_camera->Open();

      cerr << "Error! Image file loading not yet implemented." << endl;
   }

	//#ifdef IMATEST_CAMERA
	//	if (m_camera.Init(m_setup.width, m_setup.height, 4))
	//	{
	//		success = m_camera.Open();
	//		m_camera.m_device_ID = m_setup.epiphan_deviceID;
	//		m_camera.m_source_ID = m_setup.sourceID;
	//		m_camera.m_ini_file.assign(m_setup.ini_file);
	//	}
	//#elif !defined FAKE_CAMERA
	//	if (m_camera.Init(CAMERA_WIDTH, CAMERA_HEIGHT, 4))
	//	{
	//		success = m_camera.Open();
	//	}
	//#else
	//	m_camera.Init(IMAGE_NAME);
	//	success = m_camera.Open();
	//#endif

	return success;
}

bool COperatorConsoleApp::InitBlemishThread()
{
#if defined USE_RGB_DATA

	m_blemish.Init(m_cameraImage, m_setup.width, m_setup.height, 1, m_config);

#else
	m_blemish.Init(m_fileImage, m_blemishAcq.GetWidth(), m_blemishAcq.GetHeight(), 1, m_config);
#endif

	m_flags.blemishThread = m_blemishControl.Init(MSG_BLEMISH_DONE, m_nThreadID, m_blemish.ThreadProc, &m_blemish);


	return m_flags.blemishThread;
}


bool COperatorConsoleApp::InitSFRplusThread()
{
#if defined USE_RGB_DATA
	m_sfrPlus.Init(m_cameraImage, m_setup.width, m_setup.height, 1, m_config);
#else
	m_sfrPlus.Init(m_fileImage, m_sfrPlusAcq.GetWidth(), m_sfrPlusAcq.GetHeight(), 1, m_config);
#endif
	m_flags.sfrplusThread = m_sfrPlusControl.Init(MSG_SFRPLUS_DONE, m_nThreadID, m_sfrPlus.ThreadProc, &m_sfrPlus);

	return m_flags.sfrplusThread;
}


bool COperatorConsoleApp::InitCameraThread()
{
	//m_flags.cameraThread = m_cameraControl.Init(MSG_FRAME_READY, m_nThreadID, m_camera.ThreadProc, &m_camera);
	m_flags.ImatestCameraThread = m_ImatestCameraControl.Init(MSG_FRAME_READY, m_nThreadID, m_imatest_cam.ThreadProc, &m_imatest_cam);
	m_flags.DirectshowCameraThread = m_DirectShowCameraControl.Init(MSG_FRAME_READY, m_nThreadID, m_directshow_cam.ThreadProc, &m_directshow_cam);
	if (m_image_source == imatest_source)
	{
		SendAppMessage(MSG_SET_IMATEST_CAM);
	}
	else if (m_image_source == directshow_source)
	{
		SendAppMessage(MSG_SET_DIRECTSHOW_CAM);
	}
	return (m_flags.ImatestCameraThread)&& (m_flags.DirectshowCameraThread);
}



bool COperatorConsoleApp::InitLibs()
{
	CString	str;

	m_flags.matlab = mclInitializeApplication(NULL, 0);	// try to initialize the MATLAB library

	if (!m_flags.matlab)
	{
		str = "Unable to initialize MATLAB library";
	}
	else
	{
		//
		// Try to initialize Blemish and SFRplus libraries
		//
		m_flags.imatestIT = imatest_libraryInitialize();

		if (!m_flags.imatestIT)
		{
			str = "Unable to initialize imatest library.";
		}

		m_flags.imatestAcq = imatest_acquisitionInitialize();

		if (!m_flags.imatestAcq)
		{
			str = "Unable to initialize imatest acquisition library.";
		}
	}

	if (!str.IsEmpty())
	{
		AfxMessageBox(str);
	}

	return m_flags.matlab && m_flags.imatestIT && m_flags.imatestAcq;
}

void COperatorConsoleApp::CloseLibs()
{
	if (m_flags.imatestIT)
	{
		imatest_libraryTerminate();
		m_flags.imatestIT = false;
	}

	if (m_flags.imatestAcq)
	{
		imatest_acquisitionTerminate();
		m_flags.imatestAcq = false;
	}

	if (m_flags.matlab)
	{
		mclTerminateApplication();	// terminate MATLAB runtime
		m_flags.matlab = false;
	}
}

bool COperatorConsoleApp::InitModelessThread()
{
	m_jsonDlgThread = (ModelessDialogThread *)AfxBeginThread(RUNTIME_CLASS(ModelessDialogThread));

	m_flags.modelessThread = (m_jsonDlgThread != NULL);

	return m_flags.modelessThread;
}

bool COperatorConsoleApp::InitOutput()
{
	m_flags.stdOut = false;
	m_flags.stdErr = false;

#if defined REDIRECT_STDIO
	m_stdoutThread.InitThread(STDOUT_BUFSIZE, MSG_STDOUT, m_nThreadID);
	m_stderrThread.InitThread(STDOUT_BUFSIZE, MSG_STDERR, m_nThreadID);
	m_flags.stdOut = true;
	m_flags.stdErr = true;
#endif

	return m_flags.stdOut && m_flags.stdErr;
}


bool COperatorConsoleApp::AllocateImageBuf()
{
	if (m_fileImage!=NULL)
	{
		delete [] m_fileImage; 
	}
	if (m_cameraImage!=NULL)
	{
		delete [] m_cameraImage; 
	}
   if (m_displayImage != NULL)
   {
      delete [] m_displayImage;
   }

	m_fileImage   = new char[m_blemishAcq.BytesPerFrame()];
	m_cameraImage = new char[m_camera->BytesPerFrame()];
   m_displayImage = new char[m_camera->GetHeight()*m_camera->GetWidth()*4];

	return m_fileImage != NULL && m_cameraImage != NULL && m_displayImage != NULL;
}


////////////////////////////////////
///
/// The threads and dialog communicate with the application object
/// by sending messages. This function pulls out the messages of
/// interest to us before they get intercepted by Windows or MFC.
///
///  @return value of TRUE means that we've processed the message
///	 @return values of FALSE means that Windows/MFC needs to deal with the message
///
//////////////////////////////////
BOOL COperatorConsoleApp::PreTranslateMessage(MSG* pMsg)
{
	BOOL	handled = FALSE;

	switch (pMsg->message)
	{
	case MSG_START:
		OnStart((WPARAM)0, (LPARAM)0);
		handled = TRUE;
		break;

	case MSG_STOP:
		OnStop((WPARAM)0, (LPARAM)0);
		handled = TRUE;
		break;

	case MSG_FRAME_READY:
		OnFrameReady(pMsg->wParam, pMsg->lParam);
		break;

	case MSG_RUN_TEST:
		OnRunTest((WPARAM)0, (LPARAM)0);
		handled = TRUE;
		break;

	case MSG_BLEMISH_DONE:
		OnBlemishDone(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_SFRPLUS_DONE:
		OnSFRplusDone(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_SET_BLEMISH:
		OnSetBlemish(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_SET_SFRPLUS:
		OnSetSFRplus(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_JSON:
		OnShowJSON(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_STDOUT:
		OnUpdateStdout();
		break;

	case MSG_STDERR:
		OnUpdateStderr();
		break;

	case MSG_SETUP:
		OnSetup(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_PASS_FAIL:
		OnPassFail(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_SET_IMATEST_CAM:
		OnSetImatestCamera(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;

	case MSG_SET_DIRECTSHOW_CAM:
		OnSetDirectshowCamera(pMsg->wParam, pMsg->lParam);
		handled = TRUE;
		break;
   }

	return handled;
}

void COperatorConsoleApp::OnSetBlemish(WPARAM wParam, LPARAM lParam)
{
	m_test = &m_blemishControl;
	m_acq  = &m_blemishAcq;
}

void COperatorConsoleApp::OnSetSFRplus(WPARAM wParam, LPARAM lParam)
{
	m_test = &m_sfrPlusControl;
	m_acq  = &m_sfrPlusAcq;
}


void COperatorConsoleApp::OnStart(UINT wParam, LONG lParam)	// the Start button was pressed in the dialog
{
	if (m_status != runningTest)
	{
		m_status = runningTest;
		SendAppMessage(MSG_RUN_TEST);	// send message to app to run test
	}
}

void COperatorConsoleApp::OnStop(UINT wParam, LONG lParam)
{
	m_status = idle;	// any test in progress will finish, but a new one won't be started
}

void COperatorConsoleApp::OnShowJSON(UINT wParam, LONG lParam)
{
	if (m_jsonDlgThread != NULL)
	{
		m_jsonDlgThread->SetText(m_results.json);
		m_jsonDlgThread->SetTitle(m_results.name);
		m_jsonDlgThread->Show(true);
	}
}

void COperatorConsoleApp::Quit()
{
	///
	/// This function gets called when the user wants to Quit the application (either by closing the window or
	/// pressing the Quit button).  Before we quit, we need to wait for the blemish and sfrplus threads to 
	/// finish.
	/// 
	/// We tell the threads to finish by sending a Quit message to them.  However, if a test is in progress,
	/// that thread won't process the Quit message until the test finished.  This can take a few seconds, so
	/// we post a Log message and turn on the Wait cursor until the threads finish.
	///
	CWaitCursor waitcursor;

	m_status = quitting;		// this tells us not to run another test

	if (m_flags.sfrplusThread)
	{
		m_sfrPlusControl.Quit();	// wait for the sfrplus thread to quit
		m_flags.sfrplusThread = false;
	}

	if (m_flags.blemishThread)
	{
		m_blemishControl.Quit();	// wait for the blemish thread to quit
		m_flags.blemishThread = false;
	}

	//if (m_flags.cameraThread)
	//{
	//	m_cameraControl.Quit();	// wait for the camera thread to quit
	//	m_flags.cameraThread = false;
	//}

	if (m_flags.DirectshowCameraThread)
	{
		m_DirectShowCameraControl.Quit(); // wait for the DirectShow camera thread to quit
		m_flags.DirectshowCameraThread = false;
	}

	if (m_flags.ImatestCameraThread)
	{
		m_ImatestCameraControl.Quit(); // wait for the Imatest library camera thread to quit
		m_flags.ImatestCameraThread = false;
	}

	if (m_flags.modelessThread)
	{
		::PostThreadMessage(m_jsonDlgThread->m_nThreadID, MSG_MODELESS_QUIT, 0, 0);	// tell the json dialog thread to quit
		::WaitForSingleObject(m_jsonDlgThread->m_hThread, INFINITE);				// wait for it to quit (thread gets deleted automatically)
		m_flags.modelessThread = false;
	}
#if defined(REDIRECT_STDIO)
	if (m_flags.stdOut)
	{
		m_stdoutThread.Quit();		// close the stdout pipe and wait for the thread to quit
		m_flags.stdOut = false;
	}
	if (m_flags.stdErr)
	{
		m_stderrThread.Quit();		// close the stderr pipe and wait for the thread to quit
		m_flags.stdErr = false;
	}
#endif
	// TDC 2014/08/07 Calling SaveLog() anywhere in Quit() will NOT copy the contents of 
	// c_log as c_log.GetWindowText returns an empty string instead of the contents of c_log. Additionally,
	// if you use SetSel to set to the entire edit box, the values it returns have nStart > nStop.
	//SaveLog(LOG_FILENAME); 
	
	
}

void COperatorConsoleApp::OnRunTest(WPARAM wParam, LPARAM lParam)
{
	CString	str;

#if !defined NO_LOG
	str.Format("%s started\n", GetTestName());
	((COperatorConsoleDlg *)m_pMainWnd)->LogMessage(str);
#endif

#if !defined USE_RGB_DATA
   m_acq->GetFrame((void *)m_fileImage);	// get the latest image from the Acquisition object (will eventually come from camera
#endif 

#ifdef IMATEST_CAMERA
   AllocateImageBuf();// Reallocate image and display buffers
   m_camera->GetFrame(m_cameraImage);
   m_camera->GetDisplayFrame(m_displayImage);
   m_cameraControl->Run();	// capture 1 frame from the camera (we'll run the test after the frame is captured)
#elif !defined START_TEST_FROM_FRAME_READY
   m_test->Run();							// tell the thread to run 1 test (it will send our thread a message when it's done)
#else
   m_cameraControl.Run();	// capture 1 frame from the camera (we'll run the test after the frame is captured)
#endif

}

void COperatorConsoleApp::OnFrameReady(WPARAM w, LPARAM l)
{
#if defined START_TEST_FROM_FRAME_READY
#ifdef USE_RGB_DATA
 
   AllocateImageBuf(); // Reallocate image and display buffers
	m_camera->GetFrame(m_cameraImage);
   m_camera->GetDisplayFrame(m_displayImage);
#else
   //m_camera->GetFrame(m_fileImage);
#endif

   int bytesPerColor = m_camera->BytesPerPixel()/m_camera->GetNumColors();
   if (m_test == &m_sfrPlusControl)
   {
      m_sfrPlus.Init(m_cameraImage, m_camera->GetWidth(), m_camera->GetHeight(), bytesPerColor, m_config);
   }
   else if (m_test == &m_blemishControl)
   {
      m_blemish.Init(m_cameraImage, m_camera->GetWidth(), m_camera->GetHeight(), bytesPerColor, m_config);
   }

	m_test->Run();	// tell the thread to run 1 test (it will send our thread a message when it's done)
  
#else
	((COperatorConsoleDlg *)m_pMainWnd)->UpdateImage(m_cameraImage);	// display it in the dialog
	m_cameraControl.Run();												// get another frame
#endif

}

void COperatorConsoleApp::OnBlemishDone(WPARAM wParam, LPARAM lParam)
{
	if (m_status != quitting)
	{
		UpdateResults(&m_blemish);	// right now blemish doesn't have any special results, so we can just call UpdateResults() directly

		if (m_status == runningTest)
		{
         if (m_setup.sourceID != SOURCE_File) {
            SendAppMessage(MSG_RUN_TEST);	// send message to app to run again
         } else {
            SendAppMessage(MSG_STOP); // Only run once for file acquisition
            SendAppMessage(IDC_STOP);
         }
		}
	}
}

void COperatorConsoleApp::OnSFRplusDone(WPARAM wParam, LPARAM lParam)
{
   if (m_status != quitting)
	{
		
      UpdateResultsSFRplus(&m_sfrPlus);

		if (m_status == runningTest)
		{
         if (m_setup.sourceID != SOURCE_File) {
            SendAppMessage(MSG_RUN_TEST);	// send message to app to run again
         } else {
            SendAppMessage(MSG_STOP); // Only run once for file acquisition
            SendAppMessage(IDC_STOP);
         }
		}
	}
}

void COperatorConsoleApp::OnUpdateStdout()
{
	GetStdoutMsg(m_stdoutThread, m_stdoutStr);
}

void COperatorConsoleApp::OnUpdateStderr()
{
	GetStdoutMsg(m_stderrThread, m_stderrStr);
}

void COperatorConsoleApp::GetStdoutMsg(StdoutThread &data, CString &str)
{
	COperatorConsoleDlg *dlg = (COperatorConsoleDlg *)m_pMainWnd;

	data.Get(str, true);			// get the message

	if (!str.IsEmpty())
	{
		str.Replace("\n", "\r\n");		// convert \n to \r\n
		dlg->LogMessage(str, false);	// copy the message to the log display in the dialog
	}
}

void COperatorConsoleApp::UpdateResultsSFRplus(ImageTest *test)
{
	COperatorConsoleDlg	*dlg = (COperatorConsoleDlg *)m_pMainWnd;
	const bool			*qso = 	m_sfrPlus.GetQSO(); // QSO overlay (tic-tac-toe)

	UpdateResults(test);
	dlg->UpdateQuadrants(qso);
}


void COperatorConsoleApp::UpdateResults(ImageTest *test)
{
	CString				str;
	bool				passed = test->Passed();
	COperatorConsoleDlg *dlg   = (COperatorConsoleDlg *)m_pMainWnd;


#if !defined NO_LOG
	str.Format("%s took %s sec\n", test->m_name, test->m_elapsedStr);
	dlg->LogMessage(str);
#endif

#if defined START_TEST_FROM_FRAME_READY
   dlg->UpdateImage(m_displayImage); // display the image in the dialog
#endif

	//
	// Copy strings from the test object into our application
	// object so that we can display them in the dialog.
	//
	GetResults(test);

	//
	// Update the dialog with the test results
	//
	dlg->LogMessage(m_results.log, false);			// update any log messages from the test
	dlg->UpdateStatus(passed, m_results.failInfo);	// status (pass/fail)
	dlg->UpdateSummary(m_results.summary);			// summary (results)
	dlg->UpdateFPS(test->m_elapsed);				// frame rate

	//
	// Update the values in the json string, replacing \n with \r\n so
	// that the line breaks will work when displayed in a dialog.
	// 
	m_results.json.Replace("\n", "\r\n");
}

void COperatorConsoleApp::GetResults(ImageTest *test)
{
	test->GetSummary(m_results.summary);	// the summary results of the test (these get displayed in the dialog)
	test->GetFailInfo(m_results.failInfo);	// reasons that image failed the test
	test->GetLog(m_results.log);			// log message(s)
	test->GetName(m_results.name);			// the name of the test that was run
	test->GetJSON(m_results.json);
}

void COperatorConsoleApp::LogTime()
{
	CTime	t   = CTime::GetCurrentTime();
	CString	str = t.Format("%c ");

	((COperatorConsoleDlg *)m_pMainWnd)->LogMessage(str);

}

BOOL COperatorConsoleApp::SendAppMessage(int msg)
{
	BOOL	result;

	result = ::PostThreadMessage(m_nThreadID, msg, 0, 0);

	return result;
}


bool COperatorConsoleApp::CheckFiles(CString &msg)
{
	const char	*files[] = {LOGO_NAME, IMAGE_NAME, RAW_BLEMISH_IMAGE, RAW_SFRPLUS_IMAGE, INI_FILENAME};
	int			numFiles = sizeof(files) / sizeof (*files);
	int			i;
	char		str[256];
	bool		success = true;


	msg.Empty();

	//
	// Try to open all of the hard-coded files.  
	//
	for (i = 0; i < numFiles; i++)
	{
		if (!FileExists(files[i], str, sizeof(str)))
		{
			msg.AppendFormat("%s\r\n", str);
			success = false;
		}
	}

	return success;
}

bool COperatorConsoleApp::FileExists(LPCTSTR filePathName, char *errorMsg, int len)
{
	bool			exists;
	CFile			file;
	CFileException	exception;

	//
	// Try to open the file, and if successful, close it.  If we could open it, then it exists
	//
	if (!file.Open(filePathName, CFile::modeRead | CFile::shareExclusive, NULL, &exception))
	{
		exception.GetErrorMessage(errorMsg, len);
		exists = false;
	}
	else
	{
		file.Close();
		exists = true;
	}

	return exists;
}

void COperatorConsoleApp::SaveLog(void)
{
	SaveLog(m_logFileName);
}


void COperatorConsoleApp::SaveLog(const CString& filePathName)
{
	UINT	len;
	CString	str;
	CFile	file(filePathName, CFile::modeWrite | CFile::modeCreate);
	

	//
	// Copy the contents of the log window into a string
	//
	if (m_pMainWnd){
		((COperatorConsoleDlg *)m_pMainWnd)->GetLog(str);
	} 
	
	//
	// str may have lines ending with \r\n or just \n.  We want all lines to end with \r\n, so first we need to 
	// convert any \r\n combinations to \n.  Then we can go back and replace all of the \n characters with \r\n.
	//
	str.Replace("\r\n", "\n");
	str.Replace("\n", "\r\n");
	len = (UINT)str.GetLength();

	//
	// Write the string to a file.  Note that the file has *not* been opened in TEXT mode.
	// This means that the string will be written to the file as-is.  There will be *no*
	// automatic replacing of \n with \r\n.  That's okay, because the text from the
	// log window already has \r\n in it.
	//
	file.Write(str, len);
	file.Close();
}


void COperatorConsoleApp::OnSetup(WPARAM wParam, LPARAM lParam)
{
	int oldWidth = m_setup.width;
	int oldHeight = m_setup.height;
	int oldSourceID = m_setup.sourceID;

	CSetup setup(NULL, m_setup);
	INT_PTR nRet = setup.DoModal();
	if (nRet==IDOK)
	{
		m_setup = setup.m_setup_settings;
		m_camera->m_device_ID = m_setup.epiphan_deviceID; // update the device_ID used by acquire_image
		m_camera->m_source_ID = m_setup.sourceID; // update the source_ID used by acquire_image
		m_camera->m_ini_file.assign(m_setup.ini_file); //TDC


		// transfer the configuration details to the other classes
		m_config->m_iniFilePathName = m_setup.ini_file;
		m_config->m_serialNumber = m_setup.serial_number;
		m_config->m_partNumber = m_setup.part_number;
		m_config->m_programPath = m_setup.program_path;

		m_blemish.m_iniFilePathName = m_config->m_iniFilePathName ;
		m_blemish.m_serialNumber = m_config->m_serialNumber;
		m_blemish.m_partNumber = m_config->m_partNumber;
		m_blemish.m_programPath = m_config->m_programPath;

		m_sfrPlus.m_iniFilePathName = m_config->m_iniFilePathName ;
		m_sfrPlus.m_serialNumber = m_config->m_serialNumber;
		m_sfrPlus.m_partNumber = m_config->m_partNumber;
		m_sfrPlus.m_programPath = m_config->m_programPath;

		WriteINISettings(); // store new settings
		if ( oldWidth != m_setup.width || oldHeight != m_setup.height 
			|| (oldSourceID != SOURCE_OpConsoleDirectShow) && (m_setup.sourceID == SOURCE_OpConsoleDirectShow) 
			|| (oldSourceID == SOURCE_OpConsoleDirectShow) && (m_setup.sourceID != SOURCE_OpConsoleDirectShow))
		{

			if (m_setup.sourceID != SOURCE_OpConsoleDirectShow)
			{
				m_image_source=imatest_source;
				if (oldSourceID == SOURCE_OpConsoleDirectShow)
				{
					SendAppMessage(MSG_SET_IMATEST_CAM);
				}
			}
			else 
			{
				m_image_source=directshow_source;
				if (oldSourceID != SOURCE_OpConsoleDirectShow)
				{
					SendAppMessage(MSG_SET_DIRECTSHOW_CAM);
				}
			}
			// image dimensions have changed, so we must reallocate
			try
			{
				if (!ReInit())
				{
					cout << "Unable to reinitialize." << endl;
				}
			}
			catch (std::exception & ex)
			{
				cout << "Error occurred during reinitialization." << endl;
				cerr << ex.what() << endl;
			}
		}
	}
}

///
/// This function reads in items for the setup dialog from imatest.ini
///

bool COperatorConsoleApp::ReadINISettings(void)
{
	bool result = false;
	mwArray vararginParam = mwArray(1,3,mxCELL_CLASS);
	mwArray readKeys = mwArray(1,5,mxCELL_CLASS);
	mwArray inifilename(INI_FILENAME);
	mwArray mode("read");
	mwArray section_ovt("ovt"),section_imatest("imatest"),section_op("op_console"),section;
	mwArray subsection_blank(""), subsection_current("current"),subsection;
	mwArray key_acquire("acquire"),key_width("width"),key_height("height"),key_bitdepth("bitdepth"),key_bayer("bayer_pattern"),key_omniregister("register_files"),key_epiphan_deviceid("deviceID");
	mwArray value_int("i"), value_string(""), value_double("d");
	mwArray default_0(0), default_emptystring("");

	// NOTE: the mwArray::Get function has input syntax Get(number of indexes, i1, i2,...in)

	// first read the 'acquire' key from [imatest]
	readKeys.Get(2,1,1).Set(section_imatest);
	readKeys.Get(2,1,2).Set(subsection_blank);
	readKeys.Get(2,1,3).Set(key_acquire);
	readKeys.Get(2,1,4).Set(value_int);
	readKeys.Get(2,1,5).Set(default_0);

	vararginParam.Get(2,1,1).Set(inifilename);
	vararginParam.Get(2,1,2).Set(mode);
	vararginParam.Get(2,1,3).Set(readKeys);
	mwArray readSett = mwArray(1,1,mxCELL_CLASS);
	int temp_source_id = m_setup.sourceID;
	try
	{
		inifile(1,readSett,vararginParam);
		temp_source_id = (int)readSett.Get(1,1).Get(1,1);
		m_setup.sourceID = temp_source_id;

	}
	catch (mwException& e)
	{
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
	}	

	if (m_setup.sourceID ==2)
	{
		section = section_ovt;
		subsection = subsection_current;
	}
	else
	{
		section = section_op;
		subsection = subsection_blank;
	}

	readKeys = mwArray(6,5,mxCELL_CLASS);
	// to read the Epiphan 'device_ID' key 
	readKeys.Get(2,1,1).Set(section);
	readKeys.Get(2,1,2).Set(subsection);
	readKeys.Get(2,1,3).Set(key_epiphan_deviceid);
	readKeys.Get(2,1,4).Set(value_int);
	readKeys.Get(2,1,5).Set(default_0);

	// to read the 'width' key 
	readKeys.Get(2,2,1).Set(section);
	readKeys.Get(2,2,2).Set(subsection);
	readKeys.Get(2,2,3).Set(key_width);
	readKeys.Get(2,2,4).Set(value_int);
	readKeys.Get(2,2,5).Set(default_0);

	// to read the 'height' key 
	readKeys.Get(2,3,1).Set(section);
	readKeys.Get(2,3,2).Set(subsection);
	readKeys.Get(2,3,3).Set(key_height);
	readKeys.Get(2,3,4).Set(value_int);
	readKeys.Get(2,3,5).Set(default_0);

	// to read the 'bitdepth' key 
	readKeys.Get(2,4,1).Set(section);
	readKeys.Get(2,4,2).Set(subsection);
	readKeys.Get(2,4,3).Set(key_bitdepth);
	readKeys.Get(2,4,4).Set(value_int);
	readKeys.Get(2,4,5).Set(default_0);

	// to read the 'bayer_pattern' key
	readKeys.Get(2,5,1).Set(section);
	readKeys.Get(2,5,2).Set(subsection);
	readKeys.Get(2,5,3).Set(key_bayer);
	readKeys.Get(2,5,4).Set(value_int);
	readKeys.Get(2,5,5).Set(default_0);

	// to read the 'register_files' key
	readKeys.Get(2,6,1).Set(section);
	readKeys.Get(2,6,2).Set(subsection);
	readKeys.Get(2,6,3).Set(key_omniregister);
	readKeys.Get(2,6,4).Set(value_string);
	readKeys.Get(2,6,5).Set(default_emptystring);

	vararginParam.Get(1,1).Set(inifilename);
	vararginParam.Get(1,2).Set(mode);
	vararginParam.Get(1,3).Set(readKeys);

	readSett = mwArray(1,6,mxCELL_CLASS);
	int temp_epiphan_deviceid = m_setup.epiphan_deviceID;
	int temp_width = m_setup.width;
	int temp_height = m_setup.height;
	int temp_bits_per_pixel = m_setup.bits_per_pixel;
	int temp_bayer = m_setup.bayer;
	CString temp_reg_file = m_setup.omnivision_reg_file;
	try
	{
		inifile(1,readSett,vararginParam);
		temp_epiphan_deviceid =		(int)readSett.Get(1,1).Get(1,1);
		temp_width =				(int)readSett.Get(1,1).Get(1,2);
		temp_height =				(int)readSett.Get(1,1).Get(1,3);
		temp_bits_per_pixel =		(int)readSett.Get(1,1).Get(1,4);
		temp_bayer =				(int)readSett.Get(1,1).Get(1,5);
		temp_reg_file =				readSett.Get(1,1).Get(1,6).ToString();

		// copy the values into the corresponding fields in m_setup
		m_setup.epiphan_deviceID =		temp_epiphan_deviceid;
		m_setup.width =					temp_width;
		m_setup.height =				temp_height;
		m_setup.bits_per_pixel =		temp_bits_per_pixel;
		m_setup.bayer =					temp_bayer;
		m_setup.omnivision_reg_file =	temp_reg_file; 

		// change the image source if needed
		if (m_setup.sourceID != SOURCE_OpConsoleDirectShow)
		{
         m_image_source=imatest_source;
			SendAppMessage(MSG_SET_IMATEST_CAM);
		}
		else
		{
			m_image_source=directshow_source;
			SendAppMessage(MSG_SET_DIRECTSHOW_CAM);
		}
	}
	catch (mwException& e)
	{
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();

	}	

	m_setup.omnivision_reg_file.Remove('\n'); // inifile() reads newline and carriage return characters into the string
	m_setup.omnivision_reg_file.Remove('\r'); // which causes the setup window to be unable to find the file. These characters must be removed.

	if (m_setup.omnivision_reg_file.Compare("[]")==0) // Matlab returns empty strings slightly differently when cells are involved...
	{
		m_setup.omnivision_reg_file="";
	}

	if ( m_setup.width > 0 && m_setup.height > 0) // we must have the width and height > 0 since we will be allocating buffers soon
	{
		result = true;
	}

	return result;
}
//
// This function writes various items used in the setup dialog to imatest.ini
// 
void COperatorConsoleApp::WriteINISettings(void)
{
	mwArray vararginParam = mwArray(1,4,mxCELL_CLASS);
	mwArray writeKeys = mwArray(7,4,mxCELL_CLASS);
	mwArray inifilename(m_config->m_iniFilePathName);
	mwArray mode("write"),style("plain");
	mwArray section_ovt("ovt"),section_imatest("imatest"),section_op("op_console"),section("");
	mwArray subsection_blank(""), subsection_current("current"),subsection("");
	mwArray key_acquire("acquire"),key_width("width"),key_height("height"),key_bitdepth("bitdepth");
	mwArray key_bayer("bayer_pattern"),key_omniregister("register_files"),key_epiphan_deviceid("deviceID");
	mwArray val_acquire(m_setup.sourceID), val_width(m_setup.width), val_height(m_setup.height), val_bitdepth(m_setup.bits_per_pixel);
	mwArray val_bayer(m_setup.bayer), val_omniregister(m_setup.omnivision_reg_file), val_epiphan_deviceid(m_setup.epiphan_deviceID);

	// NOTE: the mwArray::Get function has input syntax Get(number of indexes, i1, i2,...in)
	// first read the 'acquire' key from [imatest]
	writeKeys.Get(2,1,1).Set(section_imatest);
	writeKeys.Get(2,1,2).Set(subsection_blank);
	writeKeys.Get(2,1,3).Set(key_acquire);
	writeKeys.Get(2,1,4).Set(val_acquire);

	if (m_setup.sourceID == SOURCE_Omnivision) // Omnivision
	{
		section = section_ovt;
		subsection = subsection_current;
	}
	else
	{
		section = section_op;
		subsection = subsection_blank;
	}

	// to write the Epiphan 'device_ID' key 
	writeKeys.Get(2,2,1).Set(section);
	writeKeys.Get(2,2,2).Set(subsection);
	writeKeys.Get(2,2,3).Set(key_epiphan_deviceid);
	writeKeys.Get(2,2,4).Set(val_epiphan_deviceid);

	// to write the 'width' key 
	writeKeys.Get(2,3,1).Set(section);
	writeKeys.Get(2,3,2).Set(subsection);
	writeKeys.Get(2,3,3).Set(key_width);
	writeKeys.Get(2,3,4).Set(val_width);


	// to write the 'height' key 
	writeKeys.Get(2,4,1).Set(section);
	writeKeys.Get(2,4,2).Set(subsection);
	writeKeys.Get(2,4,3).Set(key_height);
	writeKeys.Get(2,4,4).Set(val_height);


	// to write the 'bitdepth' key 
	writeKeys.Get(2,5,1).Set(section);
	writeKeys.Get(2,5,2).Set(subsection);
	writeKeys.Get(2,5,3).Set(key_bitdepth);
	writeKeys.Get(2,5,4).Set(val_bitdepth);


	// to write the 'bayer_pattern' 
	writeKeys.Get(2,6,1).Set(section);
	writeKeys.Get(2,6,2).Set(subsection);
	writeKeys.Get(2,6,3).Set(key_bayer);
	writeKeys.Get(2,6,4).Set(val_bayer);


	// to write the 'register_files' 
	writeKeys.Get(2,7,1).Set(section);
	writeKeys.Get(2,7,2).Set(subsection);
	writeKeys.Get(2,7,3).Set(key_omniregister);
	writeKeys.Get(2,7,4).Set(val_omniregister);

	vararginParam.Get(2,1,1).Set(inifilename);
	vararginParam.Get(2,1,2).Set(mode);
	vararginParam.Get(2,1,3).Set(writeKeys);
	vararginParam.Get(2,1,4).Set(style);
	try 
	{
		inifile(vararginParam);
	}
	catch (mwException& e)
	{
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
	}
}

//
// This function allows for reallocation of the various image buffers when we change the image size
//
bool COperatorConsoleApp::ReInit(void)
{

	CString	errMsg;
	bool	success = false;

	m_sfrPlusControl.Quit();
	m_blemishControl.Quit();

	if (!CheckFiles(errMsg))	// make sure that all of the hard coded files exist
	{
		success = false;
	}
	else if (!InitBlemishAcq())	// loads the raw blemish file into a buffer
	{
		errMsg = m_blemishAcq.GetInfo();
	}
	else if (!InitSFRplusAcq())	// loads the raw sfrplus file into a buffer
	{
		errMsg = m_sfrPlusAcq.GetInfo();
	}
	else if (!InitCamera())
	{
		errMsg = m_camera->GetInfo();
	}
	else if (!AllocateImageBuf())	// allocate a buffer large enough to hold an image [must be done after call to m_blemishAcq.Open()]
	{
		errMsg = "Unable to allocate image buffers.";
	}
	else if (!InitBlemishThread())
	{
		errMsg = "Unable to create Blemish thread.";
	}
	else if (!InitSFRplusThread())
	{
		errMsg = "Unable to create SFRplus thread.";
	}
	else
	{
		OnSetSFRplus(0, 0);	// the default test is set to SFRplus
		m_status = idle;
		success = true;
		((COperatorConsoleDlg *)m_pMainWnd)->ReInitDialog();
	}


	if (!errMsg.IsEmpty())
	{
		AfxMessageBox(errMsg);
	}

	return success;

}


void COperatorConsoleApp::OnPassFail(WPARAM wParam, LPARAM lParam)
{

	if (!m_passFailIsUnlocked)
	{
		CPasswordDialog pwdDialog;
		INT_PTR nRetPwd = pwdDialog.DoModal();
		if (nRetPwd==IDOK && m_password.Compare(pwdDialog.getResponse())==0 ) // if the user enters the correct password and clicks 'OK'
		{
			m_passFailIsUnlocked = pwdDialog.getUnlockStatus();
			CPassFail passfail(m_PFSettings, NULL);
			INT_PTR nRet = passfail.DoModal();
			if (nRet==IDOK)
			{		
				if (!m_PFSettings.b_isReadOnly)
				{
					m_PFSettings = passfail.PFSettings;
					WritePassFail();
				}
				else
				{
					cout << "Pass/Fail file is read-only. No changes were saved." << endl;
				}
			}
		}
		else if (nRetPwd==IDOK && m_password.Compare(pwdDialog.getResponse())!=0 )
		{
			cout << "Incorrect password." << endl;
		}
	}
	else
	{
		CPassFail passfail(m_PFSettings, NULL);
		INT_PTR nRet = passfail.DoModal();
		if (nRet==IDOK)
		{		
			if (!m_PFSettings.b_isReadOnly)
			{
				m_PFSettings = passfail.PFSettings;
				WritePassFail();
			}
			else
			{
				cout << "Pass/Fail file is read-only. No changes were saved." << endl;
			}
		}
	}
}

bool COperatorConsoleApp::ReadPassFail(void)
{
	bool result=false;

	// First we must find the name of the Pass/Fail file as listed in imatest.ini
	mwArray vararginParam = mwArray(1,3,mxCELL_CLASS);
	mwArray readKeys = mwArray(1,5,mxCELL_CLASS);
	mwArray inifilename(m_PFSettings.m_ini_file.GetString());
	mwArray mode("read");
	mwArray section("api");
	mwArray subsection_blank("");
	mwArray key_passFail("passFail");
	mwArray value_int("i"), value_string(""), value_double("d");
	int badval = -123456;
	std::string badstring = "-123456%^&%%$**#";
	mwArray default_0(badval), default_emptystring(badstring.c_str()),default_dbl((double)badval);



	readKeys.Get(1,1).Set(section);
	readKeys.Get(1,2).Set(subsection_blank);
	readKeys.Get(1,3).Set(key_passFail);
	readKeys.Get(1,4).Set(value_string);
	readKeys.Get(1,5).Set(default_emptystring);

	vararginParam.Get(1,1).Set(inifilename);
	vararginParam.Get(1,2).Set(mode);
	vararginParam.Get(1,3).Set(readKeys);

	mwArray readSett = mwArray(1,1,mxCELL_CLASS);
	try
	{
		inifile(1,readSett,vararginParam);
	}
	catch (mwException& e)
	{
		cout << "Run Error! Unable to read Pass/Fail file" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
	}

	m_PFSettings.m_pass_fail_file.SetString(_T(readSett.Get(1,1).Get(1,1).ToString()));
	m_PFSettings.m_pass_fail_file.Remove('\n');
	m_PFSettings.m_pass_fail_file.Remove('\r');

	//Now we find the Pass/Fail file and check if it is read-only


	if ( GetFileAttributes(m_PFSettings.m_pass_fail_file) % 2 == 1) // this returns an odd number if and only if the file is read-only
	{
		m_PFSettings.b_isReadOnly = TRUE;
	}

	// Next we need to figure out which pass/fail modules are in the ini. We will just 'readAll', which returns the full list of sections.
	//[keys,sections,subsections] = INIFILE(fName,'readall')

	mwArray keys;
	mwArray sections;
	mwArray subsections;
	mwArray passFailFile(m_PFSettings.m_pass_fail_file.GetString());
	mwArray mode_readall("readall"),mode_read("read");

	mwArray varargout = mwArray(1,3,mxCELL_CLASS);	
	varargout.Get(1,1).Set(keys);
	varargout.Get(1,2).Set(sections);
	varargout.Get(1,3).Set(subsections);

	mwArray varargin = mwArray(1,2,mxCELL_CLASS);
	varargin.Get(1,1).Set(passFailFile);
	varargin.Get(1,2).Set(mode_readall);
	try
	{
		inifile(3, varargout,varargin);
	}
	catch (mwException& e)
	{
		cout << "Run Error! Unable to read Pass/Fail file" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
	} 

	std::size_t numSections = varargout.Get(1,2).NumberOfElements();
	for (std::size_t idx = 1; idx <= numSections; ++idx)
	{
		CString section = _T(varargout.Get(1,2).Get(1,idx).ToString());

		section.Remove('\n');
		section.Remove('\r');

		if (section.CompareNoCase(_T("SFRplus")) ==0)
		{
			m_PFSettings.sfrplus.b_enable = true;
		}
		else if(section.CompareNoCase(_T("Blemish"))==0)
		{
			m_PFSettings.blemish.b_enable = true;
		}
		else if(section.CompareNoCase(_T("OIS"))==0)
		{
			m_PFSettings.ois.b_enable = true;
		}
		else
		{
			m_PFSettings.other.b_enable = true;
			m_PFSettings.other.name = section;
		}
	}

	// Next we can read in the keys for the 'other' section from what we already have for which all values are strings


	if (m_PFSettings.other.b_enable)
	{
		mwArray dims = varargout.Get(1,1).GetDimensions();
		std::vector<int> vdims;
		std::size_t arr_dims = varargout.Get(1,1).NumberOfDimensions();

		vdims.resize(arr_dims);

		dims.GetData(&vdims[0], arr_dims);
		std::size_t numRows = (std::size_t)vdims[0];
		for (std::size_t idx=1; idx <=numRows; ++idx)
		{
			CString section = _T(varargout.Get(1,1).Get(2,idx,1).ToString());
			section.Remove('\n');
			section.Remove('\r');
			CString cstrkey;
			CString cstrval;
			entry<CString> ent;
			if (section.CompareNoCase(m_PFSettings.other.name) ==0)
			{
				cstrkey = _T(varargout.Get(1,1).Get(2,idx,3).ToString());
				cstrkey.Remove('\n');
				cstrkey.Remove('\r');
				cstrval = _T(varargout.Get(1,1).Get(2,idx,4).ToString()); 
				cstrval.Remove('\n');
				cstrval.Remove('\r');
				ent.name = cstrkey;
				ent.value = cstrval;
				ent.data_type = "";
				ent.description = _T("");
				m_PFSettings.other.ent_vec.push_back(ent);
			}
		}


	}

	// Now we get to read in the other sections in a manner that respects their data type since a few of the inputs are vectors.
	// Now is a good time to grab a cup of tea...

	if (m_PFSettings.blemish.b_enable)
	{

		readKeys = mwArray(m_PFSettings.blemish.numEntries,5,mxCELL_CLASS);
		varargin = mwArray(1,3,mxCELL_CLASS);
		mwArray section_Blemish("Blemish");


		std::vector<std::string> data_types;
		std::vector<CString> blem_keys;
		// the entry<T> corresponding to a given index in blem_keys will correspond to the same index + 1 in readKeys
		addDataTypeAndKey(m_PFSettings.blemish.Blemish_maximum_count.data_type, m_PFSettings.blemish.Blemish_maximum_count.group_name,data_types,blem_keys);										// 1
		addDataTypeAndKey(m_PFSettings.blemish.Blemish_size_pixels.data_type, m_PFSettings.blemish.Blemish_size_pixels.group_name,data_types,blem_keys);											// 2
		addDataTypeAndKey(m_PFSettings.blemish.Dead_pixels_max.data_type, m_PFSettings.blemish.Dead_pixels_max.name,data_types,blem_keys);															// 3
		addDataTypeAndKey(m_PFSettings.blemish.Dead_pixel_clusters_max.data_type, m_PFSettings.blemish.Dead_pixel_clusters_max.name,data_types,blem_keys);											// 4
		addDataTypeAndKey(m_PFSettings.blemish.Defective_pixels_max_count.data_type, m_PFSettings.blemish.Defective_pixels_max_count.name,data_types,blem_keys);									// 5
		addDataTypeAndKey(m_PFSettings.blemish.Hot_pixel_clusters_max.data_type, m_PFSettings.blemish.Hot_pixel_clusters_max.name,data_types,blem_keys);											// 6
		addDataTypeAndKey(m_PFSettings.blemish.Hot_pixels_max.data_type, m_PFSettings.blemish.Hot_pixels_max.name,data_types,blem_keys);															// 7
		addDataTypeAndKey(m_PFSettings.blemish.Optical_center_offset_max.data_type, m_PFSettings.blemish.Optical_center_offset_max.name,data_types,blem_keys);										// 8
		addDataTypeAndKey(m_PFSettings.blemish.Optical_center_offset_X_max.data_type, m_PFSettings.blemish.Optical_center_offset_X_max.name,data_types,blem_keys);									// 9
		addDataTypeAndKey(m_PFSettings.blemish.Optical_center_offset_Y_max.data_type, m_PFSettings.blemish.Optical_center_offset_Y_max.name,data_types,blem_keys);									//10
		addDataTypeAndKey(m_PFSettings.blemish.Relative_illumination_corner_diff_pct_max.data_type, m_PFSettings.blemish.Relative_illumination_corner_diff_pct_max.name,data_types,blem_keys);		//11
		addDataTypeAndKey(m_PFSettings.blemish.Relative_illumination_worst_corner_pct_min.data_type, m_PFSettings.blemish.Relative_illumination_worst_corner_pct_min.name,data_types,blem_keys);	//12
		addDataTypeAndKey(m_PFSettings.blemish.Uniformity_BoverG_corners_pct_max.data_type, m_PFSettings.blemish.Uniformity_BoverG_corners_pct_max.name,data_types,blem_keys);						//13
		addDataTypeAndKey(m_PFSettings.blemish.Uniformity_RoverG_corners_pct_max.data_type, m_PFSettings.blemish.Uniformity_RoverG_corners_pct_max.name,data_types,blem_keys);						//14
		// add the contents of blem_keys and data_types to readKeys
		for (std::size_t idx = 0; idx < m_PFSettings.blemish.numEntries; ++idx)
		{
			mwArray key(blem_keys[idx].GetString());
			mwArray value(data_types[idx].c_str());
			mwArray default_ret;

			if (data_types[idx].compare("i")==0)
			{
				default_ret = default_0;
			}
			else if( data_types[idx].compare("d")==0)
			{
				default_ret = default_dbl;
			}
			else
			{
				default_ret = default_emptystring;
			}

			readKeys.Get(2,idx+1,1).Set(section_Blemish);
			readKeys.Get(2,idx+1,2).Set(subsection_blank);
			readKeys.Get(2,idx+1,3).Set(key);
			readKeys.Get(2,idx+1,4).Set(value);
			readKeys.Get(2,idx+1,5).Set(default_ret);
		}

		varargin.Get(1,1).Set(passFailFile);
		varargin.Get(1,2).Set(mode_read);
		varargin.Get(1,3).Set(readKeys);
		readSett = mwArray(1,m_PFSettings.blemish.numEntries,mxCELL_CLASS);
		try
		{
			inifile(1,readSett,varargin);
		}
		catch (mwException& e)
		{
			cout << "Run Error! Unable to read Blemish keys from Pass/Fail file" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
		}

		// now to copy the values into m_PFSettings.blemish

		std::vector<int> intVecBuf;
		std::size_t vecSize;
		vecSize = readSett.Get(1,1).Get(1,1).NumberOfElements();
		intVecBuf.resize(vecSize);
		readSett.Get(1,1).Get(1,1).GetData(&intVecBuf[0], vecSize);
		m_PFSettings.blemish.Blemish_maximum_count.value.resize(vecSize);
		if (intVecBuf[0] != badval)
		{
			for ( std::size_t idx=0; idx < vecSize; ++idx)
			{
				m_PFSettings.blemish.Blemish_maximum_count.value[idx] = intVecBuf[idx];
			}
			m_PFSettings.blemish.Blemish_maximum_count.b_isUsed = true;
		}

		vecSize = readSett.Get(1,1).Get(1,2).NumberOfElements();
		intVecBuf.clear();
		intVecBuf.resize(vecSize);
		m_PFSettings.blemish.Blemish_size_pixels.value.resize(vecSize);
		readSett.Get(1,1).Get(1,2).GetData(&intVecBuf[0], vecSize);
		if (intVecBuf[0] != badval)
		{
			for ( std::size_t idx=0; idx < vecSize; ++idx)
			{
				m_PFSettings.blemish.Blemish_size_pixels.value[idx] = intVecBuf[idx];
			}
			m_PFSettings.blemish.Blemish_size_pixels.b_isUsed = true;
		}

		int intBuf = 0;
		double dblBuf = 0.0;

		readSett.Get(1,1).Get(1,3).GetData(&intBuf, 1);
		m_PFSettings.blemish.Dead_pixels_max.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,4).GetData(&intBuf, 1);
		m_PFSettings.blemish.Dead_pixel_clusters_max.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,5).GetData(&intBuf, 1);
		m_PFSettings.blemish.Defective_pixels_max_count.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,6).GetData(&intBuf, 1);
		m_PFSettings.blemish.Hot_pixel_clusters_max.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,7).GetData(&intBuf, 1);
		m_PFSettings.blemish.Hot_pixels_max.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,8).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Optical_center_offset_max.assign_value(dblBuf,badval);

		readSett.Get(1,1).Get(1,9).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Optical_center_offset_X_max.assign_value(dblBuf,badval);

		readSett.Get(1,1).Get(1,10).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Optical_center_offset_Y_max.assign_value(dblBuf,badval);

		readSett.Get(1,1).Get(1,11).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Relative_illumination_corner_diff_pct_max.assign_value(dblBuf,badval);

		readSett.Get(1,1).Get(1,12).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Relative_illumination_worst_corner_pct_min.assign_value(dblBuf,badval);

		readSett.Get(1,1).Get(1,13).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Uniformity_BoverG_corners_pct_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,14).GetData(&dblBuf, 1);
		m_PFSettings.blemish.Uniformity_RoverG_corners_pct_max.assign_value(dblBuf,(double)badval);


	}

	if ( m_PFSettings.sfrplus.b_enable)
	{


		readKeys = mwArray(m_PFSettings.sfrplus.numEntries,5,mxCELL_CLASS);
		varargin = mwArray(1,3,mxCELL_CLASS);
		mwArray section_sfr(m_PFSettings.sfrplus.name.GetString());

		std::vector<std::string> data_types;
		std::vector<CString> sfr_keys;
		// the entry<T> corresponding to a given index in sfr_keys will correspond to the same index + 1 in readKeys
		addDataTypeAndKey(m_PFSettings.sfrplus.All_Edge_IDs_detected.data_type,m_PFSettings.sfrplus.All_Edge_IDs_detected.name,data_types,sfr_keys);														// 1
		addDataTypeAndKey(m_PFSettings.sfrplus.Bayer_decode.data_type,m_PFSettings.sfrplus.Bayer_decode.name,data_types,sfr_keys);																			// 2
		addDataTypeAndKey(m_PFSettings.sfrplus.Chart_mean_pixel_level_bounds.data_type,m_PFSettings.sfrplus.Chart_mean_pixel_level_bounds.group_name,data_types,sfr_keys);									// 3
		addDataTypeAndKey(m_PFSettings.sfrplus.Chart_radial_pixel_shift_max.data_type,m_PFSettings.sfrplus.Chart_radial_pixel_shift_max.name,data_types,sfr_keys);											// 4
		addDataTypeAndKey(m_PFSettings.sfrplus.Color_expected_detected.data_type,m_PFSettings.sfrplus.Color_expected_detected.name,data_types,sfr_keys);													// 5
		addDataTypeAndKey(m_PFSettings.sfrplus.DeltaE_00_mean_max.data_type,m_PFSettings.sfrplus.DeltaE_00_mean_max.name,data_types,sfr_keys);
		addDataTypeAndKey(m_PFSettings.sfrplus.Convergence_angle_max.data_type,m_PFSettings.sfrplus.Convergence_angle_max.name,data_types,sfr_keys);														// 6
		addDataTypeAndKey(m_PFSettings.sfrplus.FOV_degrees_diagonal_min.data_type,m_PFSettings.sfrplus.FOV_degrees_diagonal_min.name,data_types,sfr_keys);													// 7
		addDataTypeAndKey(m_PFSettings.sfrplus.High_pixel_saturation_fraction_max.data_type,m_PFSettings.sfrplus.High_pixel_saturation_fraction_max.name,data_types,sfr_keys);								// 8
		addDataTypeAndKey(m_PFSettings.sfrplus.Horizontal_bars_OK_min.data_type,m_PFSettings.sfrplus.Horizontal_bars_OK_min.name,data_types,sfr_keys);														// 9
		addDataTypeAndKey(m_PFSettings.sfrplus.Low_pixel_saturation_fraction_max.data_type,m_PFSettings.sfrplus.Low_pixel_saturation_fraction_max.name,data_types,sfr_keys);								//10
		addDataTypeAndKey(m_PFSettings.sfrplus.Mirrored_chart.data_type,m_PFSettings.sfrplus.Mirrored_chart.name,data_types,sfr_keys);																		//11
		addDataTypeAndKey(m_PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.data_type,m_PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.name,data_types,sfr_keys);											//12
		addDataTypeAndKey(m_PFSettings.sfrplus.MTF50P_ratio_min.data_type,m_PFSettings.sfrplus.MTF50P_ratio_min.name,data_types,sfr_keys);																	//13
		addDataTypeAndKey(m_PFSettings.sfrplus.passfail_ini_file_date.data_type,m_PFSettings.sfrplus.passfail_ini_file_date.name,data_types,sfr_keys);														//14
		addDataTypeAndKey(m_PFSettings.sfrplus.Rotation_degrees_max.data_type,m_PFSettings.sfrplus.Rotation_degrees_max.name,data_types,sfr_keys);															//15
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_1_center_mean_min.data_type,m_PFSettings.sfrplus.Secondary_readout_1_center_mean_min.name,data_types,sfr_keys);							//16
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.data_type,m_PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.name,data_types,sfr_keys);								//17
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_1_outer_min_min.data_type,m_PFSettings.sfrplus.Secondary_readout_1_outer_min_min.name,data_types,sfr_keys);								//18
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.data_type,m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name,data_types,sfr_keys);			//19 
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.data_type,m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name,data_types,sfr_keys);	//20
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_2_center_mean_min.data_type,m_PFSettings.sfrplus.Secondary_readout_2_center_mean_min.name,data_types,sfr_keys);							//21
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.data_type,m_PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.name,data_types,sfr_keys);								//22
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_2_outer_min_min.data_type,m_PFSettings.sfrplus.Secondary_readout_2_outer_min_min.name,data_types,sfr_keys);								//23
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.data_type,m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name,data_types,sfr_keys);			//24
		addDataTypeAndKey(m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.data_type,m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name,data_types,sfr_keys);	//25
		addDataTypeAndKey(m_PFSettings.sfrplus.Stepchart_expected_detected.data_type,m_PFSettings.sfrplus.Stepchart_expected_detected.name,data_types,sfr_keys);											//26
		addDataTypeAndKey(m_PFSettings.sfrplus.upside_down.data_type,m_PFSettings.sfrplus.upside_down.name,data_types,sfr_keys);																			//27

		// add the contents of sfr_keys and data_types to readKeys
		for (std::size_t idx = 0; idx < m_PFSettings.sfrplus.numEntries; ++idx)
		{
			mwArray key(sfr_keys[idx].GetString());
			mwArray value(data_types[idx].c_str());
			mwArray default_ret;

			if (data_types[idx].compare("i")==0)
			{
				default_ret = default_0;
			}
			else if(data_types[idx].compare("d")==0)
			{
				default_ret = default_dbl;
			}
			else
			{
				default_ret = default_emptystring;
			}
			readKeys.Get(2,idx+1,1).Set(section_sfr);
			readKeys.Get(2,idx+1,2).Set(subsection_blank);
			readKeys.Get(2,idx+1,3).Set(key);
			readKeys.Get(2,idx+1,4).Set(value);
			readKeys.Get(2,idx+1,5).Set(default_ret);
		}

		int intBuf = 0;
		double dblBuf = 0.0;
		std::vector<int> intVecBuf;
		std::vector<double> dblVecBuf;
		varargin.Get(1,1).Set(passFailFile);
		varargin.Get(1,2).Set(mode_read);
		varargin.Get(1,3).Set(readKeys);
		readSett = mwArray(1,m_PFSettings.sfrplus.numEntries,mxCELL_CLASS);

		try
		{
			inifile(1,readSett,varargin);
		}
		catch (mwException& e)
		{
			cout << "Run Error! Unable to read SFRplus keys from Pass/Fail file" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
		}
		// copy the values read from file to the appropriate entries in m_PFSettings.sfrplus
		readSett.Get(1,1).Get(1,1).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.All_Edge_IDs_detected.assign_value( intBuf,badval);

		readSett.Get(1,1).Get(1,2).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.Bayer_decode.assign_value( intBuf,badval);

		std::size_t vecSize = readSett.Get(1,1).Get(1,3).NumberOfElements();
		dblVecBuf.resize(vecSize);
		readSett.Get(1,1).Get(1,3).GetData(&dblVecBuf[0], vecSize);
		m_PFSettings.sfrplus.Chart_mean_pixel_level_bounds.assign_value(dblVecBuf,(double)badval);

		readSett.Get(1,1).Get(1,4).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Chart_radial_pixel_shift_max.assign_value( dblBuf,badval);

		readSett.Get(1,1).Get(1,5).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.Color_expected_detected.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Convergence_angle_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.DeltaE_00_mean_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,7).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.FOV_degrees_diagonal_min.assign_value(dblBuf,badval);

		readSett.Get(1,1).Get(1,8).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.High_pixel_saturation_fraction_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,9).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.Horizontal_bars_OK_min.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,10).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Low_pixel_saturation_fraction_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,11).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.Mirrored_chart.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,12).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,13).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.MTF50P_ratio_min.assign_value(dblBuf,(double)badval);

		m_PFSettings.sfrplus.passfail_ini_file_date.value = _T(readSett.Get(1,1).Get(1,14).ToString());
		m_PFSettings.sfrplus.passfail_ini_file_date.value.Remove('\n');
		m_PFSettings.sfrplus.passfail_ini_file_date.value.Remove('\r');
		if (m_PFSettings.sfrplus.passfail_ini_file_date.value.Compare(_T(badstring.c_str())) != 0)
		{
			m_PFSettings.sfrplus.passfail_ini_file_date.b_isUsed = true;
			if (m_PFSettings.sfrplus.passfail_ini_file_date.value.Compare(_T("[]")) == 0)
			{
				m_PFSettings.sfrplus.passfail_ini_file_date.value = _T("");
			}
		}
		else
		{
			m_PFSettings.sfrplus.passfail_ini_file_date.b_isUsed = false;
		}

		readSett.Get(1,1).Get(1,15).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Rotation_degrees_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,16).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_1_center_mean_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,17).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,18).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_1_outer_min_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,19).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,20).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,21).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_2_center_mean_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,22).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,23).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_2_outer_min_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,24).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,25).GetData(&dblBuf, 1);
		m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.assign_value(dblBuf,(double)badval);

		readSett.Get(1,1).Get(1,26).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.Stepchart_expected_detected.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,27).GetData(&intBuf, 1);
		m_PFSettings.sfrplus.upside_down.assign_value(intBuf,badval);

	}

	if (m_PFSettings.ois.b_enable)
	{

		readKeys = mwArray(m_PFSettings.ois.numEntries,5,mxCELL_CLASS);
		varargin = mwArray(1,3,mxCELL_CLASS);
		mwArray section_ois(m_PFSettings.ois.name.GetString());

		std::vector<std::string> data_types;
		std::vector<CString> ois_keys;

		addDataTypeAndKey(m_PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.data_type,m_PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.name,data_types,ois_keys);	// 1
		addDataTypeAndKey(m_PFSettings.ois.R_improve_ALL_dB_min.data_type,m_PFSettings.ois.R_improve_ALL_dB_min.name,data_types,ois_keys);										// 2
		addDataTypeAndKey(m_PFSettings.ois.R_improve_H_dB_min.data_type,m_PFSettings.ois.R_improve_H_dB_min.name,data_types,ois_keys);											// 3
		addDataTypeAndKey(m_PFSettings.ois.R_improve_V_dB_min.data_type,m_PFSettings.ois.R_improve_V_dB_min.name,data_types,ois_keys);											// 4
		// add the contents of ois_keys and data_types to readKeys
		for (std::size_t idx = 0; idx < m_PFSettings.ois.numEntries; ++idx)
		{
			mwArray key(ois_keys[idx].GetString());
			mwArray value(data_types[idx].c_str());
			mwArray default_ret;

			if (data_types[idx].compare("i")==0)
			{
				default_ret = default_0;
			}
			else if(data_types[idx].compare("d")==0)
			{
				default_ret = default_dbl;
			}
			else
			{
				default_ret = default_emptystring;
			}
			readKeys.Get(2,idx+1,1).Set(section_ois);
			readKeys.Get(2,idx+1,2).Set(subsection_blank);
			readKeys.Get(2,idx+1,3).Set(key);
			readKeys.Get(2,idx+1,4).Set(value);
			readKeys.Get(2,idx+1,5).Set(default_ret);
		}

		int intBuf = 0;
		double dblBuf = 0.0;
		std::vector<int> intVecBuf;

		varargin.Get(1,1).Set(passFailFile);
		varargin.Get(1,2).Set(mode_read);
		varargin.Get(1,3).Set(readKeys);
		readSett = mwArray(1,m_PFSettings.ois.numEntries,mxCELL_CLASS);

		try
		{
			inifile(1,readSett,varargin);
		}
		catch (mwException& e)
		{
			cout << "Run Error!" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
		}
		// copy the values read from file to the appropriate entries in m_PFSettings.ois
		readSett.Get(1,1).Get(1,1).GetData(&intBuf, 1);
		m_PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,2).GetData(&intBuf, 1);
		m_PFSettings.ois.R_improve_ALL_dB_min.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,3).GetData(&intBuf, 1);
		m_PFSettings.ois.R_improve_H_dB_min.assign_value(intBuf,badval);

		readSett.Get(1,1).Get(1,4).GetData(&intBuf, 1);
		m_PFSettings.ois.R_improve_V_dB_min.assign_value(intBuf,badval);
	}

	return result;
}


bool COperatorConsoleApp::WritePassFail(void)
{
	bool result = false;

	mwArray varargin = mwArray(1,4,mxCELL_CLASS);

	mwArray inifilename(m_PFSettings.m_ini_file.GetString());
	mwArray passfailfilename(m_PFSettings.m_pass_fail_file.GetString());
	mwArray mode("write");
	mwArray style("plain");
	mwArray subsection_blank("");
	mwArray value_int("i"), value_string(""), value_double("d");


	if (m_PFSettings.blemish.b_enable)
	{		
		mwArray section(m_PFSettings.blemish.name.GetString());
		std::vector<std::string> keys;
		std::vector<std::string> vals;

		addKeysAndValues(m_PFSettings.blemish.Blemish_maximum_count.group_name.GetString(), writeVecEntValueString(m_PFSettings.blemish.Blemish_maximum_count), keys, vals, m_PFSettings.blemish.Blemish_maximum_count.b_isUsed);				// 1
		addKeysAndValues(m_PFSettings.blemish.Blemish_size_pixels.group_name.GetString(),writeVecEntValueString(m_PFSettings.blemish.Blemish_size_pixels),keys,vals,m_PFSettings.blemish.Blemish_size_pixels.b_isUsed);						// 2
		addKeysAndValues(m_PFSettings.blemish.Dead_pixels_max.name.GetString(),writeEntValueString(m_PFSettings.blemish.Dead_pixels_max), keys, vals, m_PFSettings.blemish.Dead_pixels_max.b_isUsed);											// 3
		addKeysAndValues(m_PFSettings.blemish.Dead_pixel_clusters_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Dead_pixel_clusters_max), keys, vals, m_PFSettings.blemish.Dead_pixel_clusters_max.b_isUsed);				// 4
		addKeysAndValues(m_PFSettings.blemish.Defective_pixels_max_count.name.GetString(), writeEntValueString(m_PFSettings.blemish.Defective_pixels_max_count), keys, vals, m_PFSettings.blemish.Defective_pixels_max_count.b_isUsed);		// 5
		addKeysAndValues(m_PFSettings.blemish.Hot_pixel_clusters_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Hot_pixel_clusters_max), keys, vals, m_PFSettings.blemish.Hot_pixel_clusters_max.b_isUsed);					// 6
		addKeysAndValues(m_PFSettings.blemish.Hot_pixels_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Hot_pixels_max), keys, vals, m_PFSettings.blemish.Hot_pixels_max.b_isUsed);											// 7
		addKeysAndValues(m_PFSettings.blemish.Optical_center_offset_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Optical_center_offset_max), keys, vals, m_PFSettings.blemish.Optical_center_offset_max.b_isUsed);			// 8
		addKeysAndValues(m_PFSettings.blemish.Optical_center_offset_X_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Optical_center_offset_X_max), keys, vals, m_PFSettings.blemish.Optical_center_offset_X_max.b_isUsed);	// 9
		addKeysAndValues(m_PFSettings.blemish.Optical_center_offset_Y_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Optical_center_offset_Y_max), keys, vals, m_PFSettings.blemish.Optical_center_offset_Y_max.b_isUsed);	//10
		addKeysAndValues(m_PFSettings.blemish.Relative_illumination_corner_diff_pct_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Relative_illumination_corner_diff_pct_max), keys, vals, m_PFSettings.blemish.Relative_illumination_corner_diff_pct_max.b_isUsed);		//11
		addKeysAndValues(m_PFSettings.blemish.Relative_illumination_worst_corner_pct_min.name.GetString(), writeEntValueString(m_PFSettings.blemish.Relative_illumination_worst_corner_pct_min), keys, vals, m_PFSettings.blemish.Relative_illumination_worst_corner_pct_min.b_isUsed);	//12 
		addKeysAndValues(m_PFSettings.blemish.Uniformity_BoverG_corners_pct_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Uniformity_BoverG_corners_pct_max), keys, vals, m_PFSettings.blemish.Uniformity_BoverG_corners_pct_max.b_isUsed);								//13
		addKeysAndValues(m_PFSettings.blemish.Uniformity_RoverG_corners_pct_max.name.GetString(), writeEntValueString(m_PFSettings.blemish.Uniformity_RoverG_corners_pct_max), keys, vals, m_PFSettings.blemish.Uniformity_RoverG_corners_pct_max.b_isUsed);								//14
		// add the contents of keys and vals to writeKeys
		mwArray writeKeys = mwArray(keys.size(),4,mxCELL_CLASS);
		for (std::size_t idx=0; idx < keys.size(); ++idx)
		{
			mwArray key(keys[idx].c_str());
			mwArray val(vals[idx].c_str());
			writeKeys.Get(2,idx+1,1).Set(section);
			writeKeys.Get(2,idx+1,2).Set(subsection_blank);
			writeKeys.Get(2,idx+1,3).Set(key);
			writeKeys.Get(2,idx+1,4).Set(val);
		}

		varargin.Get(1,1).Set(passfailfilename);
		varargin.Get(1,2).Set(mode);
		varargin.Get(1,3).Set(writeKeys);
		varargin.Get(1,4).Set(style);

		try
		{
			inifile(varargin);
			result = true;
		}
		catch (mwException& e)
		{
			cout << "Run Error! Unable to write to Pass/Fail file" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
			result = false;
		}

	}

	if (m_PFSettings.ois.b_enable)
	{
		mwArray section(m_PFSettings.ois.name.GetString());
		std::vector<std::string> keys;
		std::vector<std::string> vals;

		addKeysAndValues(m_PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.name.GetString(), writeEntValueString(m_PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min), keys, vals, m_PFSettings.ois.L_MTF50_delta2_gain_summary_all_dB_min.b_isUsed);// 1
		addKeysAndValues(m_PFSettings.ois.R_improve_ALL_dB_min.name.GetString(), writeEntValueString(m_PFSettings.ois.R_improve_ALL_dB_min), keys, vals, m_PFSettings.ois.R_improve_ALL_dB_min.b_isUsed);	// 2
		addKeysAndValues(m_PFSettings.ois.R_improve_H_dB_min.name.GetString(), writeEntValueString(m_PFSettings.ois.R_improve_H_dB_min), keys, vals, m_PFSettings.ois.R_improve_H_dB_min.b_isUsed);		// 3
		addKeysAndValues(m_PFSettings.ois.R_improve_V_dB_min.name.GetString(), writeEntValueString(m_PFSettings.ois.R_improve_V_dB_min), keys, vals, m_PFSettings.ois.R_improve_V_dB_min.b_isUsed);		// 4

		mwArray writeKeys = mwArray(keys.size(),4,mxCELL_CLASS);
		// add the contents of keys and vals to writeKeys
		for (std::size_t idx=0; idx < keys.size(); ++idx)
		{
			mwArray key(keys[idx].c_str());
			mwArray val(vals[idx].c_str());
			writeKeys.Get(2,idx+1,1).Set(section);
			writeKeys.Get(2,idx+1,2).Set(subsection_blank);
			writeKeys.Get(2,idx+1,3).Set(key);
			writeKeys.Get(2,idx+1,4).Set(val);
		}

		varargin.Get(1,1).Set(passfailfilename);
		varargin.Get(1,2).Set(mode);
		varargin.Get(1,3).Set(writeKeys);
		varargin.Get(1,4).Set(style);

		try
		{
			inifile(varargin);
			result = true;
		}
		catch (mwException& e)
		{
			cout << "Run Error writing OIS keys to Pass/Fail file!" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
			result = false;
		}

	}

	if (m_PFSettings.other.b_enable)
	{
		mwArray writeKeys = mwArray(m_PFSettings.other.ent_vec.size(),4,mxCELL_CLASS);
		mwArray section(m_PFSettings.other.name.GetString());
		// add the contents of ent_vec to writeKeys
		for (std::size_t idx=0; idx < m_PFSettings.other.ent_vec.size(); ++idx)
		{
			mwArray key(m_PFSettings.other.ent_vec[idx].name.GetString());
			mwArray val(writeEntValueString(m_PFSettings.other.ent_vec[idx]).c_str());
			writeKeys.Get(2,idx+1,1).Set(section);
			writeKeys.Get(2,idx+1,2).Set(subsection_blank);
			writeKeys.Get(2,idx+1,3).Set(key);
			writeKeys.Get(2,idx+1,4).Set(val);
		}

		varargin.Get(1,1).Set(passfailfilename);
		varargin.Get(1,2).Set(mode);
		varargin.Get(1,3).Set(writeKeys);
		varargin.Get(1,4).Set(style);

		try
		{
			inifile(varargin);
			result = true;
		}
		catch (mwException& e)
		{
			cout << "Run Error writing Blemish keys to Pass/Fail file!" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
			result = false;
		}

	}

	if ( m_PFSettings.sfrplus.b_enable)
	{		
		mwArray section(m_PFSettings.sfrplus.name.GetString());
		std::vector<std::string> keys;
		std::vector<std::string> vals;

		addKeysAndValues(m_PFSettings.sfrplus.All_Edge_IDs_detected.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.All_Edge_IDs_detected), keys, vals, m_PFSettings.sfrplus.All_Edge_IDs_detected.b_isUsed);												// 1
		addKeysAndValues(m_PFSettings.sfrplus.Bayer_decode.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Bayer_decode), keys, vals, m_PFSettings.sfrplus.Bayer_decode.b_isUsed);																			// 2
		addKeysAndValues(m_PFSettings.sfrplus.Chart_mean_pixel_level_bounds.group_name.GetString(), writeMinMaxEntValueString(m_PFSettings.sfrplus.Chart_mean_pixel_level_bounds), keys, vals, m_PFSettings.sfrplus.Chart_mean_pixel_level_bounds.b_isUsed);			// 3
		addKeysAndValues(m_PFSettings.sfrplus.Chart_radial_pixel_shift_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Chart_radial_pixel_shift_max), keys, vals, m_PFSettings.sfrplus.Chart_radial_pixel_shift_max.b_isUsed);							// 4
		addKeysAndValues(m_PFSettings.sfrplus.Color_expected_detected.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Color_expected_detected), keys, vals, m_PFSettings.sfrplus.Color_expected_detected.b_isUsed);										// 5
		addKeysAndValues(m_PFSettings.sfrplus.DeltaE_00_mean_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.DeltaE_00_mean_max), keys, vals, m_PFSettings.sfrplus.DeltaE_00_mean_max.b_isUsed);										// 6
		addKeysAndValues(m_PFSettings.sfrplus.Convergence_angle_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Convergence_angle_max), keys, vals, m_PFSettings.sfrplus.Convergence_angle_max.b_isUsed);												// 7
		addKeysAndValues(m_PFSettings.sfrplus.FOV_degrees_diagonal_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.FOV_degrees_diagonal_min), keys, vals, m_PFSettings.sfrplus.FOV_degrees_diagonal_min.b_isUsed);										// 8
		addKeysAndValues(m_PFSettings.sfrplus.High_pixel_saturation_fraction_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.High_pixel_saturation_fraction_max), keys, vals, m_PFSettings.sfrplus.High_pixel_saturation_fraction_max.b_isUsed);		// 9
		addKeysAndValues(m_PFSettings.sfrplus.Horizontal_bars_OK_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Horizontal_bars_OK_min), keys, vals, m_PFSettings.sfrplus.Horizontal_bars_OK_min.b_isUsed);											// 10
		addKeysAndValues(m_PFSettings.sfrplus.Low_pixel_saturation_fraction_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Low_pixel_saturation_fraction_max), keys, vals, m_PFSettings.sfrplus.Low_pixel_saturation_fraction_max.b_isUsed);			//11
		addKeysAndValues(m_PFSettings.sfrplus.Mirrored_chart.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Mirrored_chart), keys, vals, m_PFSettings.sfrplus.Mirrored_chart.b_isUsed);																	//12
		addKeysAndValues(m_PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.MTF50P_CP_weighted_mean_min), keys, vals, m_PFSettings.sfrplus.MTF50P_CP_weighted_mean_min.b_isUsed);							//13
		addKeysAndValues(m_PFSettings.sfrplus.MTF50P_ratio_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.MTF50P_ratio_min), keys, vals, m_PFSettings.sfrplus.MTF50P_ratio_min.b_isUsed);																//14
		addKeysAndValues(m_PFSettings.sfrplus.passfail_ini_file_date.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.passfail_ini_file_date), keys, vals, m_PFSettings.sfrplus.passfail_ini_file_date.b_isUsed);											//15
		addKeysAndValues(m_PFSettings.sfrplus.Rotation_degrees_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Rotation_degrees_max), keys, vals, m_PFSettings.sfrplus.Rotation_degrees_max.b_isUsed);													//16
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_1_center_mean_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_1_center_mean_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_1_center_mean_min.b_isUsed);	//17
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_1_outer_mean_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_1_outer_mean_min.b_isUsed);		//18
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_1_outer_min_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_1_outer_min_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_1_outer_min_min.b_isUsed);			//19
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max), keys, vals, m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.b_isUsed);			//20
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.b_isUsed);//21
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_2_center_mean_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_2_center_mean_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_2_center_mean_min.b_isUsed);	//22
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_2_outer_mean_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_2_outer_mean_min.b_isUsed);		//23
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_2_outer_min_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_2_outer_min_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_2_outer_min_min.b_isUsed);			//24
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max), keys, vals, m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.b_isUsed);			//25
		addKeysAndValues(m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min), keys, vals, m_PFSettings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.b_isUsed);//26
		addKeysAndValues(m_PFSettings.sfrplus.Stepchart_expected_detected.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.Stepchart_expected_detected), keys, vals, m_PFSettings.sfrplus.Stepchart_expected_detected.b_isUsed);							//27
		addKeysAndValues(m_PFSettings.sfrplus.upside_down.name.GetString(), writeEntValueString(m_PFSettings.sfrplus.upside_down), keys, vals, m_PFSettings.sfrplus.upside_down.b_isUsed);																			//28

		mwArray writeKeys = mwArray(keys.size(),4,mxCELL_CLASS);
		// add the contents of keys and vals to writeKeys
		for (std::size_t idx=0; idx < keys.size(); ++idx)
		{
			mwArray key(keys[idx].c_str());
			mwArray val(vals[idx].c_str());
			writeKeys.Get(2,idx+1,1).Set(section);
			writeKeys.Get(2,idx+1,2).Set(subsection_blank);
			writeKeys.Get(2,idx+1,3).Set(key);
			writeKeys.Get(2,idx+1,4).Set(val);
		}

		varargin.Get(1,1).Set(passfailfilename);
		varargin.Get(1,2).Set(mode);
		varargin.Get(1,3).Set(writeKeys);
		varargin.Get(1,4).Set(style);

		try
		{
			inifile(varargin);
			result = true;
		}
		catch (mwException& e)
		{
			cout << "Run Error writing SFRplus keys to Pass/Fail file!" << endl;
			cerr << e.what() << endl;
			e.print_stack_trace();
			result = false;
		}
	}

	return result;
}


void COperatorConsoleApp::OnSetImatestCamera(WPARAM wParam, LPARAM lParam)
{
	m_camera = &m_imatest_cam;
	m_cameraControl = &m_ImatestCameraControl;

}


void COperatorConsoleApp::OnSetDirectshowCamera(WPARAM wParam, LPARAM lParam)
{
	m_camera = &m_directshow_cam;
	m_cameraControl = &m_DirectShowCameraControl;

}
