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

#include "AcquisitionDeviceInfo.h"



AcquisitionDeviceInfo::AcquisitionDeviceInfo()
{
}

AcquisitionDeviceInfo::AcquisitionDeviceInfo(mwArray structObj)
{
	m_isInitialized = load(structObj);
}


AcquisitionDeviceInfo::~AcquisitionDeviceInfo()
{
}

bool AcquisitionDeviceInfo::load(mwArray structObj)
{
	bool success = false;
	assert(structObj.ClassID() == mxSTRUCT_CLASS);

	if (structObj.ClassID() == mxSTRUCT_CLASS) {
		m_deviceName = extractDeviceName(structObj);
		m_deviceID = extractDeviceID(structObj);
		m_defaultFormat = extractDefaultFormat(structObj);
		m_supportedFormats = extractSupportedFormats(structObj);
		m_deviceFileSupported = extractDeviceFileSupported(structObj);
		m_adaptorName = extractAdaptorName(structObj);
		
		
		success = true;
	}
	return success;
}

int AcquisitionDeviceInfo::extractDeviceID(mwArray structObj)
{
	mxInt16 deviceID = -1;

	std::string fieldName = "DeviceID";

	if (hasField(structObj, fieldName.c_str())) {
		mwArray deviceIDField = structObj.Get(fieldName.c_str(), 1, 1);
		assert(deviceIDField.ClassID() == mxINT16_CLASS);
		if (deviceIDField.NumberOfElements() >= 0) {
			deviceID = deviceIDField(1);
			assert(deviceID >= 128);
		}
	}

	return (int)deviceID;
}

CString AcquisitionDeviceInfo::extractDeviceName(mwArray structObj)
{
	return extractStringField(structObj, "DeviceName");
}

CString AcquisitionDeviceInfo::extractAdaptorName(mwArray structObj)
{
	return extractStringField(structObj, "Adaptor");
}

CString AcquisitionDeviceInfo::extractDefaultFormat(mwArray structObj)
{
	return extractStringField(structObj, "DefaultFormat");
}

std::vector<CString> AcquisitionDeviceInfo::extractSupportedFormats(mwArray structObj)
{
	std::vector<CString> formats;

	std::string fieldName = "SupportedFormats";
	if (hasField(structObj, fieldName.c_str())) {
		mwArray field = structObj.Get(fieldName.c_str(), 1, 1);

		size_t numFormats = field.NumberOfElements();
		if (numFormats > 0 && field.ClassID() == mxCELL_CLASS) {
			assert(field.Get(1, 1).ClassID() == mxCHAR_CLASS);

			formats.resize(numFormats);
			
			mwString formatString;
			for (int idx = 1; idx <= numFormats; ++idx) {
				formatString = field.Get(1, idx).ToString();
				formats[idx - 1] = CString((const char*)formatString);
			}
			
		}

	}

	return formats;
}

bool AcquisitionDeviceInfo::extractDeviceFileSupported(mwArray structObj)
{
	assert(structObj.ClassID() == mxSTRUCT_CLASS);

	bool isSupported = false;
	std::string fieldName = "DeviceFileSupported";

	if (hasField(structObj, fieldName.c_str())) {
		mwArray field = structObj.Get(fieldName.c_str(), 1, 1);
		assert(field.ClassID() == mxLOGICAL_CLASS);

		if (field.ClassID() == mxLOGICAL_CLASS && field.NumberOfElements() > 0) {
			isSupported = field(1);
		}
	}

	return isSupported;
}

bool AcquisitionDeviceInfo::hasField(mwArray structObj, const char * str)
{
	bool result = false;

	if (structObj.ClassID() == mxSTRUCT_CLASS) {
		int numFields = structObj.NumberOfFields();

		if (numFields > 0) {
			std::vector<mwString> fields((size_t)numFields);
			for (int idx = 1; idx <= numFields; ++idx) {
				fields[idx - 1] = mwString(structObj.GetFieldName(idx));
			}

			std::vector<mwString>::iterator item = std::find(fields.begin(), fields.end(), mwString(str));

			result = item != fields.end();
		}
	}
	
	return result;
}

CString AcquisitionDeviceInfo::extractStringField(mwArray structObj, const char * fieldName)
{
	assert(structObj.ClassID() == mxSTRUCT_CLASS);

	mwString value = "";

	if (hasField(structObj, fieldName)) {
		mwArray field = structObj.Get(fieldName, 1, 1);
		assert(field.ClassID() == mxCHAR_CLASS);

		if (field.ClassID() == mxCHAR_CLASS) {
			value = field.ToString();
		}
	}

	return CString((const char*)value);
}
