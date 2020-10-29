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

// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "OperatorConsole.h"
#include "Setup.h"
#include "afxdialogex.h"
#include <algorithm>
#include "ImatestSourceIDs.h"
#include "logger_preferences.h"



// CSetup dialog

IMPLEMENT_DYNAMIC(CSetup, CDialogEx)

CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetup::IDD, pParent), m_logger("Setup"), m_fileLogHandler(LOGGER_LEVEL, LOG_FILE_NAME)
{
	m_setup_settings.width = 0;
	m_setup_settings.height = 0;
	m_setup_settings.bits_per_pixel = 0;
	m_setup_settings.sourceID = 0;
	m_setup_settings.epiphan_deviceID = 0;
	m_setup_settings.bayer = 0;

	m_logger.AddHandler(&m_fileLogHandler);
	FINEST_LOG(m_logger, "Calling CSetup()");
}


CSetup::CSetup(CWnd* pParent, const setup_settings& input_settings)
	: CDialogEx(CSetup::IDD, pParent), m_logger("Setup"), m_fileLogHandler(LOGGER_LEVEL, LOG_FILE_NAME)
{
	m_setup_settings = input_settings;
	m_logger.AddHandler(&m_fileLogHandler);
	FINEST_LOG(m_logger, "Calling CSetup()");
}

CSetup::~CSetup()
{
	FINEST_LOG(m_logger, "Calling ~CSetup()");
}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIDTH, c_setup_width);
	DDX_Control(pDX, IDC_HEIGHT, c_setup_height);
	DDX_Control(pDX, IDC_DEVICE_LIST, c_device_list);
	DDX_Control(pDX, IDC_DEVICEID, c_combo_deviceid);
	DDX_Control(pDX, IDC_BITSPERPIXEL, c_combo_bits_per_pixel);
	DDX_Control(pDX, IDC_COMBO_BAYER, c_combo_bayer);
	DDX_Control(pDX, IDC_EDIT_BROWSE, c_edit_browse);
	DDX_Control(pDX, IDC_EDIT_PARTNUMBER, c_edit_part_number);
	DDX_Control(pDX, IDC_EDIT_SERIALNUMBER, c_edit_serial_number);
	DDX_Control(pDX, IDC_EDITBROWSE_INIFILE, c_editbrowse_inifile);
	DDX_Control(pDX, IDC_EDITBROWSE_PROGPATH, c_editbrowse_prog_path);
	DDX_Control(pDX, IDC_COMBO_VIDEO_FORMAT, c_combo_video_format);
}


BEGIN_MESSAGE_MAP(CSetup, CDialogEx)
	ON_EN_CHANGE(IDC_WIDTH, &CSetup::OnChangeWidth)
	ON_EN_CHANGE(IDC_HEIGHT, &CSetup::OnChangeHeight)
	ON_LBN_SELCHANGE(IDC_DEVICE_LIST, &CSetup::OnLbnSelchangeDeviceList)
	ON_CBN_SELCHANGE(IDC_DEVICEID, &CSetup::OnSelchangeDeviceid)
	ON_CBN_SELCHANGE(IDC_BITSPERPIXEL, &CSetup::OnSelchangeBitsperpixel)
	ON_CBN_SELCHANGE(IDC_COMBO_BAYER, &CSetup::OnSelchangeComboBayer)
	ON_EN_CHANGE(IDC_EDIT_BROWSE, &CSetup::OnChangeEditBrowse)
	ON_EN_CHANGE(IDC_EDITBROWSE_PROGPATH, &CSetup::OnChangeEditbrowseProgpath)
	ON_EN_CHANGE(IDC_EDITBROWSE_INIFILE, &CSetup::OnChangeEditbrowseInifile)
	ON_EN_CHANGE(IDC_EDIT_PARTNUMBER, &CSetup::OnChangeEditPartnumber)
	ON_EN_CHANGE(IDC_EDIT_SERIALNUMBER, &CSetup::OnChangeEditSerialnumber)
	ON_CBN_SELCHANGE(IDC_DIRECTSHOWID, &CSetup::OnCbnSelchangeDirectshowid)
	ON_CBN_SELCHANGE(IDC_COMBO_VIDEO_FORMAT, &CSetup::OnCbnSelchangeComboVideoFormat)
END_MESSAGE_MAP()


// CSetup message handlers


void CSetup::OnChangeWidth()
{
	FINEST_LOG(m_logger, "Entering OnChangeWidth()");
	
	CString str;
	int value = 0;
	c_setup_width.GetWindowTextA(str);
	value = (int)strtol(str.GetString(), NULL, 10);

	if (value > 0 || value == 0)
	{
		m_setup_settings.width = value;
	}
	else if (value < 0)
	{
		m_setup_settings.width = -value;
		str.Empty();
		str.Format(_T("%d"), m_setup_settings.width);
		c_setup_width.SetWindowTextA(str);
	}

	FINEST_LOG(m_logger, "Exiting OnChangeWidth()");
}


void CSetup::OnChangeHeight()
{
	FINEST_LOG(m_logger, "Entering OnChangeHeight()");
	
	CString str;
	int value = 0;
	c_setup_height.GetWindowTextA(str);
	value = (int)strtol(str.GetString(), NULL, 10);
	if (value > 0 || value == 0)
	{
		m_setup_settings.height = value;
	}
	else if (value < 0)
	{
		m_setup_settings.height = -value;
		str.Empty();
		str.Format(_T("%d"), m_setup_settings.height);
		c_setup_height.SetWindowTextA(str);
	}

	FINEST_LOG(m_logger, "Exiting OnChangeHeight()");
}


BOOL CSetup::OnInitDialog()
{
	FINEST_LOG(m_logger, "Entering OnInitDialog()");
	
	CDialogEx::OnInitDialog();
	CString strheight, strwidth;

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	c_combo_deviceid.ShowWindow(SW_HIDE);

	strheight.Format(_T("%d"), m_setup_settings.height);
	strwidth.Format(_T("%d"), m_setup_settings.width);
	c_setup_height.SetWindowTextA(strheight);
	c_setup_width.SetWindowTextA(strwidth);

	// fill the list of devices
	for (std::size_t i1 = 0; i1 < m_setup_settings.device_list.size(); ++i1)
	{
		c_device_list.AddString(m_setup_settings.device_list[i1]);
	}
	for (size_t i1 = 0; i1 < m_setup_settings.deviceInfos.size(); ++i1) {
		c_device_list.AddString(m_setup_settings.deviceInfos[i1].m_deviceName);
	}

	// fill the list of Bayer patterns
	for (std::size_t i1 = 0; i1 < m_setup_settings.bayer_list.size(); ++i1)
	{
		c_combo_bayer.AddString(m_setup_settings.bayer_list[i1]);
	}

	// fill the list of allowed bits per pixel (Omnivision only)
	for (std::size_t j1 = 0; j1 < m_setup_settings.allowed_bits_per_pixel.size(); ++j1)
	{
		CString str;
		str.Format(_T("%d"), m_setup_settings.allowed_bits_per_pixel[j1]);
		c_combo_bits_per_pixel.AddString(str);
	}

	// fill the combo-box of Epiphan device IDs
	for (std::size_t j1 = 0; j1 < m_setup_settings.epiphan_deviceID_list.size(); ++j1)
	{
		c_combo_deviceid.AddString(m_setup_settings.epiphan_deviceID_list[j1]);
	}

	// TODO: Delete this code related to directshow
	// file the combo-box of DirectShow device names
	//for (std::size_t j1 = 0; j1 < m_setup_settings.directshow_device_names.size(); ++j1)
	//{
	//	c_combo_directshow_id.AddString(m_setup_settings.directshow_device_names[j1]);
	//}

	//if (m_setup_settings.directshow_device_names.size() > 0)
	//	c_combo_directshow_id.SetCurSel(m_setup_settings.directshow_deviceID);


	////////////////////////////////////////////////////////////////////////////////////
	// Select the stored value for the device and hide/show objects accordingly
	////////////////////////////////////////////////////////////////////////////////////

	// Recall: 
	//device_list[0] = _T("Aptina DevWare");
	//device_list[1] = _T("Omnivision OVTA");
	//device_list[2] = _T("Toshiba ImaTuning");
	//device_list[3] = _T("STM Conduit");
	//device_list[4] = _T("Graphin EasyLab");
	//device_list[5] = _T("Epiphan");	
	//device_list[6] = _T("DirectShow camera");
	if (m_setup_settings.sourceID >= 0)
	{
		CString str;

		switch (m_setup_settings.sourceID)
		{
		case SOURCE_Aptina:
			str.SetString(m_setup_settings.device_list[0]); // Aptina DevWare
			ShowNormalElements();
			break;
		case SOURCE_Omnivision:
			str.SetString(m_setup_settings.device_list[1]); // Omnivision OVTA
			ShowOmnivisionElements();
			break;
		case SOURCE_Toshiba:
			str.SetString(m_setup_settings.device_list[2]); // Toshiba ImaTuning
			ShowNormalElements();
			break;
		case SOURCE_STM:
			str.SetString(m_setup_settings.device_list[3]); // STM Conduit
			ShowNormalElements();
			break;
		case SOURCE_Graphin:
			str.SetString(m_setup_settings.device_list[4]); // Graphin EasyLab
			ShowNormalElements();
			break;
		case SOURCE_Epiphan:
			str.SetString(m_setup_settings.device_list[5]); // Epiphan
			ShowEpiphanElements();
			break;
		// TODO: Delete
		//case SOURCE_OpConsoleDirectShow:
		//	str.SetString(m_setup_settings.device_list[6]); // DirectShow camera
		//	ShowDirectShowElements();
		//	break;
		case SOURCE_File:
			str.SetString(m_setup_settings.device_list[7]); // Load an image file
			ShowNormalElements();
			break;
		default:
			if (m_setup_settings.sourceID >= 128 && m_setup_settings.deviceInfos.size() > 0) {
				int sourceID = m_setup_settings.sourceID;
				auto selection = std::find_if(m_setup_settings.deviceInfos.begin(), m_setup_settings.deviceInfos.end(), [sourceID](AcquisitionDeviceInfo& x) {return x.m_deviceID == sourceID; });
				if (selection != m_setup_settings.deviceInfos.end()) {
					str.SetString(selection->m_deviceName);
					ShowDynamicDeviceElements(*selection);

				}
				else {
					str.SetString(m_setup_settings.device_list[0]); // Default: Aptina DevWare
					ShowNormalElements();
				}
			}
			else {
				str.SetString(m_setup_settings.device_list[0]); // Default: Aptina DevWare
				ShowNormalElements();
			}
		}

		c_device_list.SelectString(-1, str); // select and highlight the previously stored entry

	}

	if (m_setup_settings.bayer > 0) // select the Bayer pattern item using the stored value
	{
		c_combo_bayer.SetCurSel(m_setup_settings.bayer - 1);
	}
	else
	{
		c_combo_bayer.SetCurSel(0);
		m_setup_settings.bayer = 1;
	}

	if (m_setup_settings.bits_per_pixel > 0) // select the bits per pixel item using the stored value
	{
		CString str;
		str.Format(_T("%d"), m_setup_settings.bits_per_pixel);

		int idx;
		idx = c_combo_bits_per_pixel.FindString(-1, str);

		if (idx != CB_ERR)
		{
			c_combo_bits_per_pixel.SetCurSel(idx);
		}
		else
		{
			c_combo_bits_per_pixel.SetCurSel(0);
		}
	}
	else
	{
		c_combo_bits_per_pixel.SetCurSel(0);
		m_setup_settings.bits_per_pixel = 8;
	}

	// enable (or disable) the various edit-browse controls
	CString devstr;
	devstr.Format(_T("%d"), m_setup_settings.epiphan_deviceID);
	int idx = c_combo_deviceid.FindString(-1, devstr);
	c_combo_deviceid.SetCurSel(idx);

	c_edit_browse.EnableFileBrowseButton();
	c_edit_browse.SetWindowTextA(m_setup_settings.omnivision_reg_file);

	c_editbrowse_inifile.EnableFileBrowseButton();
	c_editbrowse_inifile.SetWindowTextA(m_setup_settings.ini_file);

	//c_editbrowse_prog_path.EnableFolderBrowseButton(); // the program path is a read-only display for now
	c_editbrowse_prog_path.EnableBrowseButton(false);
	c_editbrowse_prog_path.SetWindowTextA(m_setup_settings.program_path);
	c_editbrowse_prog_path.SetReadOnly();

	// load the part number and serial number into their respective edit boxes
	c_edit_part_number.SetWindowTextA(m_setup_settings.part_number);

	c_edit_serial_number.SetWindowTextA(m_setup_settings.serial_number);
	
	FINEST_LOG(m_logger, "Exiting OnInitDialog()");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSetup::OnLbnSelchangeDeviceList()
{
	FINEST_LOG(m_logger, "Entering OnLbnSelchangeDeviceList()");
	
	int nIndex;
	CString str;
	nIndex = c_device_list.GetCurSel();
	c_device_list.GetText(nIndex, str);
	m_setup_settings.device_name = str;

	if (str.Compare(m_setup_settings.device_list[0]) == 0) // Aptina DevWare
	{
		m_setup_settings.sourceID = SOURCE_Aptina;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[1]) == 0) // Omnivision OVTA
	{
		m_setup_settings.sourceID = SOURCE_Omnivision;
		ShowOmnivisionElements();
	}
	else if (str.Compare(m_setup_settings.device_list[2]) == 0) // Toshiba ImaTuning
	{
		m_setup_settings.sourceID = SOURCE_Toshiba;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[3]) == 0) // STM Conduit
	{
		m_setup_settings.sourceID = SOURCE_STM;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[4]) == 0) // Graphin EasyLab
	{
		m_setup_settings.sourceID = SOURCE_Graphin;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[5]) == 0) // Epiphan
	{
		m_setup_settings.sourceID = SOURCE_Epiphan;
		ShowEpiphanElements();
	}
	//TODO: Delete
	//else if (str.Compare(m_setup_settings.device_list[6]) == 0) // DirectShow camera
	//{
	//	m_setup_settings.sourceID = SOURCE_OpConsoleDirectShow;
	//	ShowDirectShowElements();
	//}
	else if (str.Compare(m_setup_settings.device_list[7]) == 0) // Image File
	{
		m_setup_settings.sourceID = SOURCE_File;
		ShowNormalElements();
	}
	else
	{

		auto selection = std::find_if(m_setup_settings.deviceInfos.begin(), m_setup_settings.deviceInfos.end(), [str](AcquisitionDeviceInfo x) { return str.Compare(x.m_deviceName) == 0; });
		if (selection != m_setup_settings.deviceInfos.end()) {
			m_setup_settings.sourceID = selection->m_deviceID;
			fprintf(stdout, "%S selected with sourceID %d", selection->m_deviceName, selection->m_deviceID);
			ShowDynamicDeviceElements(*selection);
		}
		else {
			m_setup_settings.sourceID = SOURCE_Aptina;
			fprintf(stderr, "Error: Unknown device selection. The program will revert to Aptina DevWare");
			ShowNormalElements();
		}


	}

	FINEST_LOG(m_logger, "Exiting OnLbnSelchangeDeviceList()");
}

void CSetup::OnSelchangeDeviceid()
{
	FINEST_LOG(m_logger, "Entering OnSelchangeDeviceid()");
	
	CString str;

	c_combo_deviceid.GetWindowTextA(str);

	if (str.CompareNoCase(m_setup_settings.epiphan_deviceID_list[0]) == 0)
	{
		m_setup_settings.epiphan_deviceID = 0;
	}
	else if (str.CompareNoCase(m_setup_settings.epiphan_deviceID_list[1]) == 0)
	{
		m_setup_settings.epiphan_deviceID = 1;
	}
	FINEST_LOG(m_logger, "Exiting OnSelchangeDeviceid()");
}

int CSetup::GetDeviceID(void) const
{
	return m_setup_settings.epiphan_deviceID;
}

int CSetup::GetBitsPerPixel(void) const
{
	return m_setup_settings.bits_per_pixel;
}


void CSetup::OnSelchangeBitsperpixel()
{
	FINEST_LOG(m_logger, "Entering OnSelchangeBitsperpixel()");
	CString str;
	int value = 0;

	c_combo_bits_per_pixel.GetWindowTextA(str);
	value = (int)strtol(str.GetString(), NULL, 10);
	// double check that the found value is allowed.
	std::vector<int>::iterator itr = std::find(m_setup_settings.allowed_bits_per_pixel.begin(), m_setup_settings.allowed_bits_per_pixel.end(), value);

	if (itr != m_setup_settings.allowed_bits_per_pixel.end())
	{
		m_setup_settings.bits_per_pixel = value;
	}
	else
	{
		m_setup_settings.bits_per_pixel = 8;
	}

	FINEST_LOG(m_logger, "Exiting OnSelchangeBitsperpixel()");
}


void CSetup::OnSelchangeComboBayer()
{
	FINEST_LOG(m_logger, "Entering OnSelchangeComboBayer()");
	
	CString str;
	int value = 0;
	c_combo_bayer.GetWindowTextA(str);

	if (str.CompareNoCase(m_setup_settings.bayer_list[0]) == 0) // Red in R1C1 (RG/GB)
	{
		m_setup_settings.bayer = 1;
	}
	else if (str.CompareNoCase(m_setup_settings.bayer_list[1]) == 0) // Red in R1C2 (GR/BG)
	{
		m_setup_settings.bayer = 2;
	}
	else if (str.CompareNoCase(m_setup_settings.bayer_list[2]) == 0) // Red in R2C1 (GB/RG)
	{
		m_setup_settings.bayer = 3;
	}
	else if (str.CompareNoCase(m_setup_settings.bayer_list[3]) == 0) // Red in R2C2 (BG/GR)
	{
		m_setup_settings.bayer = 4;
	}
	else
	{
		m_setup_settings.bayer = 0;
	}

	FINEST_LOG(m_logger, "Exiting OnSelchangeComboBayer()");
}


void CSetup::OnChangeEditBrowse()
{
	FINEST_LOG(m_logger, "Entering OnChangeEditBrowse()");
	
	CFile theFile;
	CFileStatus status;
	CString FileName;
	c_edit_browse.GetWindowTextA(FileName);
	FileName.Remove('\n'); // remove these characters, just in case, as they cause the file search to fail
	FileName.Remove('\r');

	if (FileName.Compare(_T("")) != 0) // Check for an empty string
	{
		if (CFile::GetStatus(FileName, status)) // if we find the file, propagate the filename
		{
			m_setup_settings.omnivision_reg_file.SetString(FileName);
		}
		else
		{
			fprintf(stderr, "Error: Omnivision register settings file not found. The program will revert to the previous file.\n");
		}
	}
	else // allow the user to not supply a register file after previously supplying one
	{
		m_setup_settings.omnivision_reg_file.SetString(FileName);
	}

	FINEST_LOG(m_logger, "Exiting OnChangeEditBrowse()");
}


void CSetup::OnChangeEditbrowseProgpath()
{
	FINEST_LOG(m_logger, "Entering OnChangeEditbrowseProgpath()");
	
	CFile theFile;
	CFileStatus status;
	CString PathName;
	c_editbrowse_prog_path.GetWindowTextA(PathName);
	PathName.Remove('\n'); // remove these characters, just in case, as they cause the folder search to fail
	PathName.Remove('\r');
	if (PathName.Compare(_T("")) != 0) // Check for an empty string
	{
		if (CFile::GetStatus(PathName, status)) // if we find the folder, propagate the path name
		{
			m_setup_settings.program_path.SetString(PathName);
		}
		else
		{
			fprintf(stderr, "Program path not found. The program will revert to the previous path.\n");
		}
	}

	FINEST_LOG(m_logger, "Exiting OnChangeEditbrowseProgpath()");
}


void CSetup::OnChangeEditbrowseInifile()
{
	FINEST_LOG(m_logger, "Entering OnChangeEditbrowseInifile()");
	
	CFile theFile;
	CFileStatus status;
	CString FileName;
	c_editbrowse_inifile.GetWindowTextA(FileName);
	FileName.Remove('\n'); // remove these characters, just in, case as they cause the file search to fail
	FileName.Remove('\r');

	if (FileName.Compare(_T("")) != 0) // Check for an empty string
	{
		if (CFile::GetStatus(FileName, status)) // if we find the file, propagate the filename
		{
			m_setup_settings.ini_file.SetString(FileName);
		}
		else
		{
			fprintf(stderr, "Error: INI file not found. The program will revert to the previous file.\n");
		}
	}
	else
	{
		c_editbrowse_inifile.SetWindowTextA(m_setup_settings.ini_file);
	}

	FINEST_LOG(m_logger, "Exiting OnChangeEditbrowseInifile()");
}


void CSetup::OnChangeEditPartnumber()
{
	FINEST_LOG(m_logger, "Entering OnChangeEditPartnumber()");
	c_edit_part_number.GetWindowTextA(m_setup_settings.part_number);
	FINEST_LOG(m_logger, "Exiting OnChangeEditPartnumber()");
}


void CSetup::OnChangeEditSerialnumber()
{
	FINEST_LOG(m_logger, "Entering OnChangeEditSerialnumber()");
	c_edit_serial_number.GetWindowTextA(m_setup_settings.serial_number);
	FINEST_LOG(m_logger, "Exiting OnChangeEditSerialnumber()");
}

//
// This function hides the dialog elements that are only for
// Epiphan or Omnivision
//
void CSetup::ShowNormalElements(void)
{
	FINEST_LOG(m_logger, "Entering ShowNormalElements()");
	
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);
	CWnd* pVideoFormatLabel = GetDlgItem(IDC_STATIC_VIDEO_FORMAT);

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	pBitsPerPixel->ShowWindow(SW_HIDE);

	c_combo_deviceid.ShowWindow(SW_HIDE);
	pDeviceID->ShowWindow(SW_HIDE);

	c_combo_bayer.ShowWindow(SW_HIDE);
	pBayer->ShowWindow(SW_HIDE);

	c_edit_browse.ShowWindow(SW_HIDE);
	pEditBrowse->ShowWindow(SW_HIDE);

	c_combo_video_format.ShowWindow(SW_HIDE);
	pVideoFormatLabel->ShowWindow(SW_HIDE);

	FINEST_LOG(m_logger, "Exiting ShowNormalElements()");
}

//
// This function shows the dialog elements that are meant only 
// for Epiphan and hides those meant only for Omnivision
//
void CSetup::ShowEpiphanElements(void)
{
	FINEST_LOG(m_logger, "Entering ShowEpiphanElements()");
	
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);
	CWnd* pVideoFormatLabel = GetDlgItem(IDC_STATIC_VIDEO_FORMAT);

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	pBitsPerPixel->ShowWindow(SW_HIDE);

	c_combo_deviceid.ShowWindow(SW_SHOW);
	pDeviceID->ShowWindow(SW_SHOW);

	c_combo_bayer.ShowWindow(SW_HIDE);
	pBayer->ShowWindow(SW_HIDE);

	c_edit_browse.ShowWindow(SW_HIDE);
	pEditBrowse->ShowWindow(SW_HIDE);

	c_combo_video_format.ShowWindow(SW_HIDE);
	pVideoFormatLabel->ShowWindow(SW_HIDE);

	FINEST_LOG(m_logger, "Exiting ShowEpiphanElements()");
}

// TODO: Delete ShowDirectShowElements
// This function shows the dialog elements that are meant only 
// for Epiphan and hides those meant only for Omnivision
//
//void CSetup::ShowDirectShowElements(void)
//{
//	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
//	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
//	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
//	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);
//	CWnd* pDirectShowLabel = GetDlgItem(IDC_STATIC_DIRECTSHOWLABEL);
//	CWnd* pVideoFormatLabel = GetDlgItem(IDC_STATIC_VIDEO_FORMAT);
//
//	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
//	pBitsPerPixel->ShowWindow(SW_HIDE);
//
//	c_combo_deviceid.ShowWindow(SW_HIDE);
//	pDeviceID->ShowWindow(SW_HIDE);
//
//	c_combo_bayer.ShowWindow(SW_HIDE);
//	pBayer->ShowWindow(SW_HIDE);
//
//	c_edit_browse.ShowWindow(SW_HIDE);
//	pEditBrowse->ShowWindow(SW_HIDE);
//
//	c_combo_video_format.ShowWindow(SW_HIDE);
//	pVideoFormatLabel->ShowWindow(SW_HIDE);
//}

void CSetup::ShowDynamicDeviceElements(const AcquisitionDeviceInfo & device)
{
	FINEST_LOG(m_logger, "Entering ShowDynamicDeviceElements()");
	
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);
	CWnd* pVideoFormatLabel = GetDlgItem(IDC_STATIC_VIDEO_FORMAT);

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	pBitsPerPixel->ShowWindow(SW_HIDE);

	c_combo_deviceid.ShowWindow(SW_HIDE);
	pDeviceID->ShowWindow(SW_HIDE);

	c_combo_bayer.ShowWindow(SW_HIDE);
	pBayer->ShowWindow(SW_HIDE);

	c_edit_browse.ShowWindow(SW_HIDE);
	pEditBrowse->ShowWindow(SW_HIDE);


	UpdateVideoFormatDropdown(device);
	c_combo_video_format.ShowWindow(SW_SHOW);
	pVideoFormatLabel->ShowWindow(SW_SHOW);

	FINEST_LOG(m_logger, "Exiting ShowDynamicDeviceElements()");
}

void CSetup::UpdateVideoFormatDropdown(const AcquisitionDeviceInfo & device)
{
	FINEST_LOG(m_logger, "Entering UpdateVideoFormatDropdown()");
	
	c_combo_video_format.ResetContent();
	for (auto iFormat = device.m_supportedFormats.begin(); iFormat != device.m_supportedFormats.end(); ++iFormat)
		c_combo_video_format.AddString(_T(*iFormat));

	CString formatString;
	if (m_setup_settings.video_format.IsEmpty()) {
		// Set the selection to the default video format
		formatString = device.m_defaultFormat;
	}
	else {
		formatString = m_setup_settings.video_format;
	}

	int cursorSelection = c_combo_video_format.FindStringExact(0, formatString);
	cursorSelection = cursorSelection >= 0 ? cursorSelection : 0;
	c_combo_video_format.SetCurSel(cursorSelection);

	FINEST_LOG(m_logger, "Exiting UpdateVideoFormatDropdown()");
}


//
// This function shows the dialog elements that are meant only 
// for Omnivision and hides those meant only for Epiphan
//
void CSetup::ShowOmnivisionElements(void)
{
	FINEST_LOG(m_logger, "Entering ShowOmnivisionElements()");
	
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);
	CWnd* pVideoFormatLabel = GetDlgItem(IDC_STATIC_VIDEO_FORMAT);

	c_combo_bits_per_pixel.ShowWindow(SW_SHOW);
	pBitsPerPixel->ShowWindow(SW_SHOW);

	c_combo_deviceid.ShowWindow(SW_HIDE);
	pDeviceID->ShowWindow(SW_HIDE);

	c_combo_bayer.ShowWindow(SW_SHOW);
	pBayer->ShowWindow(SW_SHOW);

	c_edit_browse.ShowWindow(SW_SHOW);
	pEditBrowse->ShowWindow(SW_SHOW);

	c_combo_video_format.ShowWindow(SW_HIDE);
	pVideoFormatLabel->ShowWindow(SW_HIDE);

	FINEST_LOG(m_logger, "Exiting ShowOmnivisionElements()");
}

//
// This function shows all the dialog elements that should
// only be shown for either Omnivision or Epiphan
//
void CSetup::ShowAllElements(void)
{
	FINEST_LOG(m_logger, "Entering ShowAllElements()");
	
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);
	CWnd* pVideoFormatLabel = GetDlgItem(IDC_STATIC_VIDEO_FORMAT);

	c_combo_bits_per_pixel.ShowWindow(SW_SHOW);
	pBitsPerPixel->ShowWindow(SW_SHOW);

	c_combo_deviceid.ShowWindow(SW_SHOW);
	pDeviceID->ShowWindow(SW_SHOW);

	c_combo_bayer.ShowWindow(SW_SHOW);
	pBayer->ShowWindow(SW_SHOW);

	c_edit_browse.ShowWindow(SW_SHOW);
	pEditBrowse->ShowWindow(SW_SHOW);

	c_combo_video_format.ShowWindow(SW_SHOW);
	pVideoFormatLabel->ShowWindow(SW_SHOW);

	FINEST_LOG(m_logger, "Exiting ShowAllElements()");
}

// TODO: Delete OnCbnSelchangeDirectshowid
void CSetup::OnCbnSelchangeDirectshowid()
{

	m_setup_settings.directshow_deviceID = c_combo_directshow_id.GetCurSel();

}


void CSetup::OnCbnSelchangeComboVideoFormat()
{
	FINEST_LOG(m_logger, "Entering OnCbnSelchangeComboVideoFormat()");
	
	int index = c_combo_video_format.GetCurSel();
	CString selection = "";

	c_combo_video_format.GetLBText(index, selection);
	if (!selection.IsEmpty()) {
		m_setup_settings.video_format = selection;
	}

	FINEST_LOG(m_logger, "Exiting OnCbnSelchangeComboVideoFormat()");
}
