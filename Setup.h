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
#include "afxwin.h"
#include "setup_settings.h"
#include "afxeditbrowsectrl.h"
//
// CSetup dialog
//

///
/// The class underlying the setup dialog. This dialog allows the user to select between different 
/// camera sources (which must already be functioning) and input the necessary data, e.g. image width in pixels.
/// Additionally, the user can supply the location of the ini file to be used with Imatest as well as the 
/// camera part number and serial number.
///
class CSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CSetup)

public:
	CSetup(CWnd* pParent = NULL);   // standard constructor
	CSetup(CWnd* pParent, const setup_settings& input_settings);   // non-standard constructor
	virtual ~CSetup();

	// Dialog Data
	enum { IDD = IDD_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit c_setup_width;						//!< The CEdit box for image width (in pixels)
	CEdit c_setup_height;						//!< The CEdit box for image height (in pixels)
	CListBox c_device_list;						//!< The CListBox containing the names of supported devices (Aptina DevWare, Omnivision, etc.)
	CComboBox c_combo_deviceid;					//!< The CCombobox drop-down list containing the two valid device IDs (only visible for Epiphan)
	CComboBox c_combo_bits_per_pixel;			//!< The CCombobox drop-down list containing the valid bits-per-pixel values (only visible for Omnivision)
	CComboBox c_combo_bayer;					//!< The CCombobox drop-down list containing the valid Bayer pattern indexes (only visible for Omnivision)
	CMFCEditBrowseCtrl c_edit_browse;			//!< [OPTIONAL INPUT] The MFCEditBrowse that allows the user to navigate the file system to locate an Omnivision register file (only visible for Omnivision)
	CEdit c_edit_part_number;					//!< [OPTIONAL INPUT] The CEdit box for the camera part number to be reported in the JSON output
	CEdit c_edit_serial_number;					//!< [OPTIONAL INPUT] The CEdit box for the camera serial number to be reported in the JSON output
	CMFCEditBrowseCtrl c_editbrowse_inifile;	//!< The MFCEditBrowse that allows the user to navigate the file system to locate an Imatest INI file
	CMFCEditBrowseCtrl c_editbrowse_prog_path;	//!< [Currently set to read-only] The MFCEditBrowse that allows the user to navigate the file system to locate the Imatest program path
	CComboBox c_combo_directshow_id;            //!< The CCombobox drop-down list containing the two valid device IDs (only visible for DirectShow)
	CComboBox c_combo_video_format;

	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWidth();
	afx_msg void OnChangeHeight();
	afx_msg void OnLbnSelchangeDeviceList();
	afx_msg void OnSelchangeDeviceid();
	afx_msg void OnSelchangeBitsperpixel();
	afx_msg void OnSelchangeComboBayer();
	afx_msg void OnChangeEditBrowse();
	afx_msg void OnChangeEditbrowseProgpath();
	afx_msg void OnChangeEditbrowseInifile();
	afx_msg void OnChangeEditPartnumber();
	afx_msg void OnChangeEditSerialnumber();

	int GetDeviceID(void) const;
	int GetBitsPerPixel(void) const;

public:
	setup_settings m_setup_settings;	//!< this contains settings for the setup dialog including the strings to fill the various CcomboBoxes and CListBoxes
										//!< and is fed back to the OperatorConsoleApp through a copy operation

private:
	void ShowOmnivisionElements(void);  //!< This function shows dialog elements that are meant only for Omnivision and hides those meant for Epiphan
	void ShowNormalElements(void);		//!< This function hides dialog elements that are meant only for Omnivision or Epiphan
	void ShowEpiphanElements(void);		//!< This function shows dialog elements that are meant only for Epiphan and hides those meant for Omnivision
	void ShowAllElements(void);			//!< This function shows dialog elements that are meant only for Omnivision or Epiphan
	void ShowDirectShowElements(void);
	void ShowDynamicDeviceElements(const AcquisitionDeviceInfo& device);
	void UpdateVideoFormatDropdown(const AcquisitionDeviceInfo& device);
public:
	afx_msg void OnCbnSelchangeDeviceid2();
	afx_msg void OnStnClickedStaticDeviceid2();
	afx_msg void OnCbnSelchangeDirectshowid();
	afx_msg void OnCbnSelchangeComboVideoFormat();
};
