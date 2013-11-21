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


// CSetup dialog

IMPLEMENT_DYNAMIC(CSetup, CDialogEx)

	CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetup::IDD, pParent)
{
	m_setup_settings.width = 0;
	m_setup_settings.height = 0;
	m_setup_settings.bits_per_pixel = 0;
	m_setup_settings.sourceID = 0;
	m_setup_settings.epiphan_deviceID = 0;
	m_setup_settings.bayer = 0;
}


CSetup::CSetup(CWnd* pParent , const setup_settings& input_settings)
	: CDialogEx(CSetup::IDD, pParent)
{
	m_setup_settings = input_settings;
}

CSetup::~CSetup()
{
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
END_MESSAGE_MAP()


// CSetup message handlers


void CSetup::OnChangeWidth()
{	
	CString str;
	int value = 0;
	c_setup_width.GetWindowTextA(str);
	value = (int) strtol(str.GetString(),NULL,10);	

	if ( value > 0 || value ==0)
	{
		m_setup_settings.width = value;
	}
	else if (value < 0)
	{
		m_setup_settings.width = -value;
		str.Empty();
		str.Format(_T("%d"),m_setup_settings.width);
		c_setup_width.SetWindowTextA(str);
	}
}


void CSetup::OnChangeHeight()
{
	CString str;
	int value = 0;
	c_setup_height.GetWindowTextA(str);
	value = (int) strtol(str.GetString(),NULL,10);
	if ( value > 0 || value ==0)
	{
		m_setup_settings.height = value;
	}
	else if (value < 0)
	{
		m_setup_settings.height = -value;
		str.Empty();
		str.Format(_T("%d"),m_setup_settings.height);
		c_setup_height.SetWindowTextA(str);
	}
}


BOOL CSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strheight, strwidth;

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	c_combo_deviceid.ShowWindow(SW_HIDE);

	strheight.Format(_T("%d"),m_setup_settings.height);
	strwidth.Format(_T("%d"),m_setup_settings.width);
	c_setup_height.SetWindowTextA(strheight);
	c_setup_width.SetWindowTextA(strwidth);



	// fill the list of devices
	for (std::size_t i1 = 0; i1 < m_setup_settings.device_list.size(); ++i1)
	{
		c_device_list.AddString(m_setup_settings.device_list[i1]);
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
		str.Format(_T("%d"), m_setup_settings.allowed_bits_per_pixel[j1] );
		c_combo_bits_per_pixel.AddString(str);
	}

	// fill the combo-box of Epiphan device IDs
	for ( std::size_t j1 = 0; j1 < m_setup_settings.epiphan_deviceID_list.size(); ++j1)
	{
		c_combo_deviceid.AddString(m_setup_settings.epiphan_deviceID_list[j1]);
	}

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

	if (m_setup_settings.sourceID != 0)
	{
		CString str;

		switch (m_setup_settings.sourceID)
		{
		case 1:
			str.SetString(m_setup_settings.device_list[0]); // Aptina DevWare
			ShowNormalElements();
			break;
		case 2:
			str.SetString(m_setup_settings.device_list[1]); // Omnivision OVTA
			ShowOmnivisionElements();
			break;
		case 3:
			str.SetString(m_setup_settings.device_list[2]); // Toshiba ImaTuning
			ShowNormalElements();
			break;
		case 4:
			str.SetString(m_setup_settings.device_list[3]); // STM Conduit
			ShowNormalElements();
			break;
		case 5:
			str.SetString(m_setup_settings.device_list[4]); // Graphin EasyLab
			ShowNormalElements();
			break;
		case 6:
			str.SetString(m_setup_settings.device_list[5]); // Epiphan
			ShowEpiphanElements();
			break;
		default:
			str.SetString(m_setup_settings.device_list[0]); // Default: Aptina DevWare
			ShowNormalElements();
		}

		c_device_list.SelectString(-1,str); // select and highlight the previously stored entry

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

	if ( m_setup_settings.bits_per_pixel > 0) // select the bits per pixel item using the stored value
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
	int idx = c_combo_deviceid.FindString(-1,devstr);	
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSetup::OnLbnSelchangeDeviceList()
{

	int nIndex;
	CString str;
	nIndex = c_device_list.GetCurSel();
	c_device_list.GetText(nIndex,str);

	if (str.Compare(m_setup_settings.device_list[0]) ==0) // Aptina DevWare
	{
		m_setup_settings.sourceID = 1;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[1]) ==0) // Omnivision OVTA
	{
		m_setup_settings.sourceID = 2;
		ShowOmnivisionElements();
	}
	else if (str.Compare(m_setup_settings.device_list[2]) ==0) // Toshiba ImaTuning
	{
		m_setup_settings.sourceID = 3;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[3]) ==0) // STM Conduit
	{
		m_setup_settings.sourceID = 4;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[4]) ==0) // Graphin EasyLab
	{
		m_setup_settings.sourceID = 5;
		ShowNormalElements();
	}
	else if (str.Compare(m_setup_settings.device_list[5]) ==0) // Epiphan
	{
		m_setup_settings.sourceID = 6;
		ShowEpiphanElements();
	}
	else
	{
		m_setup_settings.sourceID = 1;
		fprintf(stderr,"Error: Unknown device selection. The program will revert to Aptina DevWare");
		ShowNormalElements();
	}
}

void CSetup::OnSelchangeDeviceid()
{

	CString str;

	c_combo_deviceid.GetWindowTextA(str);

	if (str.CompareNoCase(m_setup_settings.epiphan_deviceID_list[0])==0)
	{
		m_setup_settings.epiphan_deviceID = 0;
	}
	else if (str.CompareNoCase(m_setup_settings.epiphan_deviceID_list[1])==0)
	{
		m_setup_settings.epiphan_deviceID = 1;
	}

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
	CString str;
	int value = 0;

	c_combo_bits_per_pixel.GetWindowTextA(str);
	value = (int) strtol(str.GetString(),NULL,10);
	// double check that the found value is allowed.
	std::vector<int>::iterator itr = std::find(m_setup_settings.allowed_bits_per_pixel.begin(), m_setup_settings.allowed_bits_per_pixel.end(), value);

	if ( itr != m_setup_settings.allowed_bits_per_pixel.end())
	{
		m_setup_settings.bits_per_pixel = value;
	}
	else
	{
		m_setup_settings.bits_per_pixel = 8;
	}
}


void CSetup::OnSelchangeComboBayer()
{
	CString str;
	int value = 0;
	c_combo_bayer.GetWindowTextA(str);

	if (str.CompareNoCase(m_setup_settings.bayer_list[0])==0) // Red in R1C1 (RG/GB)
	{
		m_setup_settings.bayer = 1;
	}
	else if (str.CompareNoCase(m_setup_settings.bayer_list[1])==0) // Red in R1C2 (GR/BG)
	{
		m_setup_settings.bayer = 2;
	}
	else if (str.CompareNoCase(m_setup_settings.bayer_list[2])==0) // Red in R2C1 (GB/RG)
	{
		m_setup_settings.bayer = 3;
	}
	else if (str.CompareNoCase(m_setup_settings.bayer_list[3])==0) // Red in R2C2 (BG/GR)
	{
		m_setup_settings.bayer = 4;
	}
	else
	{
		m_setup_settings.bayer = 0;
	}
}


void CSetup::OnChangeEditBrowse()
{
	CFile theFile;
	CFileStatus status;
	CString FileName;
	c_edit_browse.GetWindowTextA(FileName);
	FileName.Remove('\n'); // remove these characters, just in case, as they cause the file search to fail
	FileName.Remove('\r');

	if ( FileName.Compare(_T("")) != 0) // Check for an empty string
	{
		if (CFile::GetStatus( FileName, status )) // if we find the file, propagate the filename
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

}


void CSetup::OnChangeEditbrowseProgpath()
{
	CFile theFile;
	CFileStatus status;
	CString PathName;
	c_editbrowse_prog_path.GetWindowTextA(PathName);
	PathName.Remove('\n'); // remove these characters, just in case, as they cause the folder search to fail
	PathName.Remove('\r');
	if ( PathName.Compare(_T("")) != 0) // Check for an empty string
	{
		if (CFile::GetStatus( PathName, status )) // if we find the folder, propagate the path name
		{
			m_setup_settings.program_path.SetString(PathName);
		}
		else
		{
			fprintf(stderr, "Program path not found. The program will revert to the previous path.\n");
		}	
	}
}


void CSetup::OnChangeEditbrowseInifile()
{
	CFile theFile;
	CFileStatus status;
	CString FileName;
	c_editbrowse_inifile.GetWindowTextA(FileName);
	FileName.Remove('\n'); // remove these characters, just in, case as they cause the file search to fail
	FileName.Remove('\r');

	if ( FileName.Compare(_T("")) != 0) // Check for an empty string
	{
		if (CFile::GetStatus( FileName, status )) // if we find the file, propagate the filename
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
}


void CSetup::OnChangeEditPartnumber()
{
	c_edit_part_number.GetWindowTextA(m_setup_settings.part_number);
}


void CSetup::OnChangeEditSerialnumber()
{
	c_edit_serial_number.GetWindowTextA(m_setup_settings.serial_number);
}

//
// This function hides the dialog elements that are only for
// Epiphan or Omnivision
//
void CSetup::ShowNormalElements(void)
{
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	pBitsPerPixel->ShowWindow(SW_HIDE);
	c_combo_deviceid.ShowWindow(SW_HIDE);
	pDeviceID ->ShowWindow(SW_HIDE);
	pBayer ->ShowWindow(SW_HIDE);
	c_combo_bayer.ShowWindow(SW_HIDE);
	pEditBrowse -> ShowWindow(SW_HIDE);
	c_edit_browse.ShowWindow(SW_HIDE);
}

//
// This function shows the dialog elements that are meant only 
// for Epiphan and hides those meant only for Omnivision
//
void CSetup::ShowEpiphanElements(void)
{
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);

	c_combo_bits_per_pixel.ShowWindow(SW_HIDE);
	pBitsPerPixel->ShowWindow(SW_HIDE);
	c_combo_deviceid.ShowWindow(SW_SHOW);
	pDeviceID ->ShowWindow(SW_SHOW);
	pBayer ->ShowWindow(SW_HIDE);
	c_combo_bayer.ShowWindow(SW_HIDE);
	pEditBrowse -> ShowWindow(SW_HIDE);
	c_edit_browse.ShowWindow(SW_HIDE);
}

//
// This function shows the dialog elements that are meant only 
// for Omnivision and hides those meant only for Epiphan
//
void CSetup::ShowOmnivisionElements(void)
{
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);

	c_combo_bits_per_pixel.ShowWindow(SW_SHOW);
	pBitsPerPixel->ShowWindow(SW_SHOW);
	c_combo_deviceid.ShowWindow(SW_HIDE);
	pDeviceID ->ShowWindow(SW_HIDE);
	pBayer ->ShowWindow(SW_SHOW);
	c_combo_bayer.ShowWindow(SW_SHOW);
	pEditBrowse -> ShowWindow(SW_SHOW);
	c_edit_browse.ShowWindow(SW_SHOW);
}

//
// This function shows all the dialog elements that should
// only be shown for either Omnivision or Epiphan
//
void CSetup::ShowAllElements(void)
{
	CWnd* pDeviceID = GetDlgItem(IDC_STATIC_DEVICEID);
	CWnd* pBitsPerPixel = GetDlgItem(IDC_STATIC_BITSPERPIXEL);
	CWnd* pBayer = GetDlgItem(IDC_STATIC_BAYER);
	CWnd* pEditBrowse = GetDlgItem(IDC_STATIC_BROWSE);

	c_combo_bits_per_pixel.ShowWindow(SW_SHOW);
	pBitsPerPixel->ShowWindow(SW_SHOW);
	c_combo_deviceid.ShowWindow(SW_SHOW);
	pDeviceID ->ShowWindow(SW_SHOW);
	pBayer ->ShowWindow(SW_SHOW);
	c_combo_bayer.ShowWindow(SW_SHOW);
	pEditBrowse -> ShowWindow(SW_SHOW);
	c_edit_browse.ShowWindow(SW_SHOW);
}
