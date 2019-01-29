/****************************************************************************
*	Operator Console - an extensible user interface for the Imatest IT 		*
*	library																	*
*	Copyright (C) 2019 Imatest LLC.											*
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

#include "atlstr.h"
#include "imatest_acquisition.h"

#include <algorithm>
#include <string>
#include <vector>

///
/// This class houses the information describing a acquisition device as returned by a call to list_devices()
///
class AcquisitionDeviceInfo
{
public:
	AcquisitionDeviceInfo();
	AcquisitionDeviceInfo(mwArray structObj);
	~AcquisitionDeviceInfo();

public:
	CString m_adaptorName; //!< The acquisition adaptor name (e.g. winvideo)
	CString m_deviceName;  //!< The device name
	int m_deviceID;        //!< The device ID to be used as the sourceID input for the acquire_image() call
	CString m_defaultFormat; //!< The default video format for the device that is used if no other format is specified in the acquire_image() call
	std::vector<CString> m_supportedFormats; //!< The supported video formats for the device
	bool m_deviceFileSupported; //!< Whether or not the device supports a configuration file (not currently supported by the Operator Console)

	bool isValid() const { return m_isInitialized; }

private:
	bool m_isInitialized;

	bool load(mwArray structObj);
	int extractDeviceID(mwArray structObj); //!< Returns the value of the "DeviceID" field of structObj
	CString extractDeviceName(mwArray structObj); //!< Returns the value of the "DeviceName" field of structObj
	CString extractAdaptorName(mwArray structObj); //!< Returns the value of the "Adaptor" field of structObj
	CString extractDefaultFormat(mwArray structObj); //!< Returns the value of the "DefaultFormat" field of structObj
	std::vector<CString> extractSupportedFormats(mwArray structObj); //!< Returns the values of the "SupportedFormats" field of structObj
	bool extractDeviceFileSupported(mwArray structObj); //!< Returns the value of the "DeviceFileSupported" field of structObj

	static bool hasField(mwArray structObj, const char* str); //!< A simple method for determining if a mwArray struct has a given field name
	static CString extractStringField(mwArray structObj, const char* fieldName); //!< A method for extract the value of a struct field that only contains a single string

};

