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

// OperatorConsole.h : main header file for the Operator Console application
//

#pragma once

#include "stdafx.h"

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <string>       // std::string
#include <iostream>     // std::cout, endl
#include <sstream>      // std::stringstream

#include "BlemishTest.h"
#include "Config.h"
#include "FileAcq.h"
#include "FileAcquisition.h"
#include "ImageAcquisition.h"
#include "ImageTest.h"
#include "ImatestLibAcq.h"
#include "ModelessDialogThread.h"
#include "SFRplusTest.h"
#include "StderrRedirect.h"
#include "StderrThread.h"
#include "StdoutRedirect.h"
#include "StdoutThread.h"
#include "TextDlg.h"
#include "ThreadControl.h"
#include "setup_settings.h"
#include "PassFailSettings.h"
#include "OperatorConsoleDlg.h"
#include "op_console_preferences.h"


//
// These are messages that get sent to our application object
//
#define MSG_START				(WM_APP + 0x10)	// gets sent when Start button is clicked in dialog
#define MSG_STOP				(WM_APP + 0x11)	// gets sent when Stop  button is clicked in dialog
#define MSG_RUN_TEST			(WM_APP + 0x12) // gets sent to start running a test
#define MSG_BLEMISH_DONE		(WM_APP + 0x13)	// gets sent from blemish thread when the blemish test has finished
#define MSG_SFRPLUS_DONE		(WM_APP + 0x14)	// gets sent from SFRplus thread when the SFRplus test has finished
#define MSG_SET_BLEMISH			(WM_APP + 0x15)	// gets sent from dialog when "Blemish" radio button is pressed
#define MSG_SET_SFRPLUS			(WM_APP + 0x16)	// gets sent from dialog when "SFRplus" radio button is pressed
#define MSG_JSON				(WM_APP + 0x17)	// gets sent from dialog when user wants to see the JSON output from the latest test
#define MSG_STDOUT				(WM_APP + 0x18)	// gets sent from StdoutThread when console output is available
#define MSG_STDERR				(WM_APP + 0x19)	// gets sent from StderrThread when console output is available
#define MSG_FRAME_READY			(WM_APP + 0x20)	// gets sent from capture thread when a frame is ready for display
#define MSG_SETUP				(WM_APP + 0x21)	// gets sent when the Setup button is clicked in dialog
#define MSG_PASS_FAIL			(WM_APP + 0x22) // gets sent when the 'Set Pass/Fail' button is clicked in dialog
#define MSG_SET_IMATEST_CAM		(WM_APP + 0x23) // gets sent when one of the Imatest library camera devices is selected


typedef enum AppStatus
{
	runningTest,	// an image test is running
	idle,			// no test is running
	quitting		// the user pressed Quit or closed the dialog window, but we need to wait for threads to finish	
} AppStatus;

///
/// A struct for holding all results (including JSON) of a test
///
typedef struct TestResults
{
	CString	name;		//!< the name of the test that was run
	CString	summary;	//!< the summary results of the test
	CString	json;		//!< holds the JSON output of a test
	CString	failInfo;	//!< reasons that image failed the test
	CString	log;		//!< log message(s)
} TestResults;

///
/// A struct for holding the various flags used throughout the application
///
typedef struct AppFlags
{
	unsigned int	blemishThread:1;
	unsigned int	sfrplusThread:1;
	unsigned int	cameraThread:1;
	unsigned int	modelessThread:1;
	unsigned int	matlab:1;			//!< MATLAB lib has been initialized
	unsigned int	imatestIT:1;		//!< imatestIT lib has been initialized
	unsigned int    imatestAcq:1;		//!< Imatest IT acquisition lib has been initialized
	unsigned int	stdOut:1;			//!< stdout pipe is open
	unsigned int	stdErr:1;			//!< stderr pipe is open
	unsigned int	ImatestCameraThread:1;
} AppFlags;

///
/// An enum type for indicating the image source
///
typedef enum image_source_t{
	imatest_source,	//!< image acquisition using the Imatest library
	file_source		//!< loading a file for the image source
};

#ifdef INI_SEPARATE_PARAMS
void MW_CALL_CONV inifile(int nargout, mwArray& varargout, const mwArray& varargin);

void MW_CALL_CONV inifile(int nargout, mwArray& varargout);

void MW_CALL_CONV inifile(const mwArray& varargin);
#endif


// COperatorConsoleApp:
// See OperatorConsole.cpp for the implementation of this class
//

///
/// The main class for the Operator Console Application. This class interfaces with the display
/// (controlled by the COperatorConsoleDlg class) through a messaging-passing interface. Therefore,
/// all data relevant to the operation of the program are owned by COperatorConsoleApp and not 
/// COperatorConsoleDlg.
/// 
class COperatorConsoleApp : public CWinApp
{
public:
	COperatorConsoleApp();
	~COperatorConsoleApp();
	void Quit();

	// Overrides
public:
	BOOL				InitInstance();
	static bool			FileExists(LPCTSTR filePathName, char *errorMsg, int len);
	int					GetImageWidth()  {return m_setup.width;}
	int					GetImageHeight() {return m_setup.height;}
	bool				ReadINISettings(void);					//!< This function reads in items for the setup dialog from imatest.ini
	void				WriteINISettings(void);					//!< This function writes items for the setup dialog to imatest.ini
	bool				ReInit(void);							//!< This function allows for reallocation of the various image buffers when we change the image size
	bool				ReadPassFail(void);						//!< This function reads in the pass/fail variable values as in the pass/fail file listed in the imatest INI file
	bool				WritePassFail(void);					//!< This function writes the pass/fail variables to the pass/fail file listed in the imatest INI file

protected:
	bool				AllocateImageBuf();
	bool                AllocateCameraImageBuf(unsigned int numBytes);
	bool				CheckFiles(CString &msg);
	void				CloseLibs();								//!< Function that closes the Imatest library. Note that once closed, the library cannot be reinitialized.
	void				GetResults(ImageTest *test);
	void				GetStdoutMsg(StdoutThread &data, CString &str);
	const char		    *GetTestName() { return (m_testControl == NULL) ? "" : ((ImageTest *)m_testControl->m_data)->m_name;}
	bool				Init();
	bool				InitBlemishAcq();
	bool				InitBlemishThread();
	bool				InitCamera();
	bool				InitCameraThread();
	bool				InitLibs();									//!< Function that initializes the Imatest library. Must be called before any Imatest library functions are used.
	bool				InitModelessThread();
	bool				InitOutput();
	bool				InitSFRplusAcq();
	bool				InitSFRplusThread();
	bool				LoadConfig();
	void				MakeHandles();
	void				OnBlemishDone(WPARAM wParam, LPARAM lParam);//!< Called after MSG_BLEMISH_DONE is received 
	void				OnFrameReady(WPARAM wParam, LPARAM lParam); //!< Called after MSG_FRAME_READY is received
	void				OnPassFail(WPARAM wParam, LPARAM lParam);	//!< Called after MSG_PASS_FAIL is received
	void				OnRunTest(WPARAM wParam, LPARAM lParam);	//!< Called after MSG_RUN_TEST is received
	void				OnSFRplusDone(WPARAM wParam, LPARAM lParam);//!< Called after MSG_SFRPLUS_DONE is received
	void				OnSetBlemish(WPARAM wParam, LPARAM lParam);	//!< Called after MSG_SET_BLEMISH is received
	void				OnSetSFRplus(WPARAM wParam, LPARAM lParam);	//!< Called after MSG_SET_SFRPLUS is received
	void				OnSetImatestCamera(WPARAM wParam, LPARAM lParam);
	void				OnShowJSON(WPARAM wParam, LPARAM lParam);		//!< Called after MSG_JSON is received
	void				OnStop(WPARAM wParam, LPARAM lParam);		//!< Called after MSG_STOP is received
	void				OnStart(WPARAM wParam, LPARAM lParam);		//!< Called after MSG_START is received
	void				OnSetup(WPARAM wParam, LPARAM lParam);		//!< Called after MSG_SETUP is received
	void				OnUpdateStderr();							//!< Called after MSG_STDERR is received
	void				OnUpdateStdout();							//!< Called after MSG_STDOUT is received
	bool				SaveConfig();
	void				SaveLog(const CString& filePathName);
	bool AllocateFileImageBuf(unsigned int numBytes);
	BOOL				PreTranslateMessage(MSG* pMsg);				//!< Intercepts messages of interest before Windows or MFC.
	BOOL				SendAppMessage(int msg);
	void				LogTime();
	void				UpdateResults(ImageTest *test);
	void				UpdateResultsSFRplus(ImageTest *test);
	void				UpdateStderr();
	void				UpdateStdout();

public:

	// Implementation
	void				SaveLog(void);			//!< A public function to save the contents of the log CEdit to m_logFileName.

protected:
	AppStatus				m_status;
	Config					m_configRAW;		//!< for using raw files:  will eventually come from a dialog (uses hard coded values for now)
	Config					m_configRGB;		//!< for using rgb data:   will eventually come from a dialog (uses hard coded values for now)
	Config					*m_config;			//!< pointer to current config struct to use
	char					*m_fileImage;		//!< buffer to hold a single image frame from the file   (will eventually be combined with m_cameraImage)
	char					*m_cameraImage;		//!< buffer to hold a single image frame from the camera (will eventually be combined with m_fileImage)
	FileAcquisition			m_blemishAcq;		//!< raw data for blemish tests (no longer needed, now that live capture is working)
	FileAcquisition			m_sfrPlusAcq;		//!< raw data for SFRplus tests (no longer needed, now that live capture is working)
	ImageAcquisition		*m_acq;				//!< pointer to current acquisition object being used
//#if defined IMATEST_CAMERA
//	ImatestLibAcq			m_camera;			//!< live acquisition using Imatest acquire_image()
//#elif !defined FAKE_CAMERA
//	SimpleDirectShowAcq		m_camera;			// live acquisition using a camera
//#else
//	FileAcq					m_camera;			// acquisition comes from a file, with data in RGB format (array of RGBQUAD structs)
//#endif
	ImatestLibAcq			m_imatest_cam;		//!< live acquisition using Imatest acquire_image()	
	//FileAcq					m_file_cam;			//!< acquisition comes from a file, with data in RGB format (array of RGBQUAD structs)
	ImageAcquisition		*m_camera;	

	int						m_width;
	int						m_height;
	size_t                  m_cameraImageBufLen;
	
	setup_settings			m_setup;			//!< this contains settings from/for the setup dialog
	BlemishTest				m_blemish;			//!< this will run the blemish tests
	SFRplusTest				m_sfrPlus;			//!< this will run the SFRplus tests
	ThreadControl			m_blemishControl;	//!< this is the thread control for running Blemish tests
	ThreadControl			m_sfrPlusControl;	//!< this is the thread control for running SFRplus tests
	ThreadControl			m_ImatestCameraControl;
	ThreadControl			m_DirectShowCameraControl;
	ThreadControl			*m_cameraControl;	//!< points to the current image source's  control thread
	//ThreadControl			m_cameraControl;	//!< this captures images from the acquisition source
	ImageTest			    *m_test;			//!< the current test being run (either &m_blemishControl or &sfrPlusControl)
	ThreadControl           *m_testControl;
	ModelessDialogThread	*m_jsonDlgThread;
	TestResults				m_results;
	StdoutThread			m_stdoutThread;
	StderrThread			m_stderrThread;
	CString					m_stdoutStr;		//!< our copy of the contents of m_stdout
	CString					m_stderrStr;		//!< our copy of the contents of m_stderr
	AppFlags				m_flags;

	DECLARE_MESSAGE_MAP()

public:
	CPassFailSettings m_PFSettings;				//!< Contains the pass/fail criteria given by the pass/fail file listed in imatest.ini
private:
	CString m_password;							//!< This contains the administrator password
	bool m_passFailIsUnlocked;					//!< Indicates whether the administrator has chosen to unlock access to pass/fail settings for the duration that the executable runs
	image_source_t m_image_source;
	CString m_logFileName;						//!< Contains the file name for the log file (log file is filled with the contents of the log edit box upon app closure).
};

extern COperatorConsoleApp theApp;
