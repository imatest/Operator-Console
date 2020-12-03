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

#ifndef GUARD_SETUP_SETTINGS_H
#define GUARD_SETUP_SETTINGS_H
#include "stdafx.h"
#include "afxstr.h"
#include "AcquisitionDeviceInfo.h"
#include <vector>

#define IMATEST_INI_SECTION "imatest"
#define OP_CONSOLE_SECTION "op_console"
#define INI_FILE_PATH_KEY "ini_file_path"
#define PART_NUMBER_KEY "part_number"
#define DEVICE_NAME_KEY "device_name"
#define SERIAL_NUMBER_KEY "serial_number"

///
/// A struct that contains the various data controlled by the setup dialog.
///
class setup_settings {

public:
	
	int						width;					//!< image width in pixels
	int						height;					//!< image height in pixels
	int						bits_per_pixel;			//!< Used for Omnivision only: number of bits per pixel (8,10,12 only) 
	int						epiphan_deviceID;		//!< Used for Epiphan only: the device ID (0 or 1 only) 
	int						sourceID;				//!< sourceID for acquire_image() 
													/**< Only takes values 1 through 6. 1 => Aptina DevWare, 2 => Omnivision, 3 => Toshiba, 4 => STM Conduit, 5 => Graphin EasyLab, 6 => Epiphan*/
	int						bayer;					//!< Used for Omnivision only: Bayer pattern index
													/**< Only takes values 1 through 4.  1 => Red in R1C1, 2=> Red in R1C2, 3=> Red in R2C1, 4=> Red in R2C2 */
	int                     directshow_deviceID;  //!< Used for DirectShow only: the device ID 
	CString					omnivision_reg_file;	//!< [OPTIONAL INPUT] Used for Omnivision only: a fully-qualified filename (including full path) for a file containing camera register settings 
	CString					part_number;			//!< [OPTIONAL INPUT] The user supplied camera part number to be used in the JSON output 
	CString					serial_number;			//!< [OPTIONAL INPUT] The user supplied camera part serial to be used in the JSON output 
	CString					ini_file;				//!< The fully-qualified file name (including full path) for the Imatest INI file
	CString                 op_console_ini_file;    //!< The 
	CString					program_path;			//!< The Imatest IT program path
	CString                 video_format;           //!< The video format for dynamically detected devices
	CString                 device_name;            
	std::vector<CString>	epiphan_deviceID_list;	//!< The list of allowed Epiphan device IDs used to fill the c_combo_deviceid CComboBox in Setup.h/cpp
	std::vector<CString>	device_list;			//!< The list of allowed device names used to fill the c_device_list CListBox in Setup.h/cpp
	std::vector<CString>	bayer_list;				//!< The list of allowed Bayer patterns used to fill the c_combo_bayer CComboBox in Setup.h/cpp
	std::vector<int>		allowed_bits_per_pixel; //!< The list of allowed bits-per-pixels used to fill the c_combo_bits_per_pixel CComboBox in Setup.h/cpp
	std::vector<AcquisitionDeviceInfo> deviceInfos;
	///
	/// default constructor
	///
	setup_settings();

	static void addStringWriteRequest(mwArray& writeKeys, mwSize rowIndex, const char* section, const char* subsection, const char* key, const char* value) noexcept;
	void writeOpConsoleKeys();
	static void addStringReadRequest(mwArray& readKeys, mwSize rowIndex, const char* section, const char* subsection, const char* key, const char* default_value) noexcept;
	void readOpConsoleKeys();

private:
	static CString getImatestINIFolderPath();
	
	};



#endif