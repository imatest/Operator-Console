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

#include "stdafx.h"
#include "escapi30\escapi.h"
#include "SimpleDirectShowAcq.h"
#include "ThreadControl.h"


int SimpleDirectShowAcq::max_string_size = 50;

SimpleDirectShowAcq::SimpleDirectShowAcq(void)
{
	m_width       = 0;
	m_height      = 0;
	m_numBytes    = 0;
	m_numPixels   = 0;
	m_buf         = NULL;
   m_cameraIndex = 0;
   m_device_names.clear();
}


SimpleDirectShowAcq::~SimpleDirectShowAcq(void)
{
}
///
/// @return Returns the value of m_inited after initialization, e.g. returns TRUE if the camera is successfully initialized
///
bool SimpleDirectShowAcq::Open()
{
	struct SimpleCapParams	params;
	bool					success = false;


	if (m_width == 0 || m_height == 0)	// these get set in Init()
	{
		m_logMsg = "Unable to Open camera:  width and height are not set.";
	}
	else
	{
		m_numCameras = setupESCAPI();

      m_device_names.clear();

		if (m_numCameras == -1)
		{
			m_logMsg.Format("Error initializing ESCAPI.  Make sure the escap.dll file is in the same directory as the executable.");
		}
		if (m_numCameras == -2)
		{
			m_logMsg.Format("Error initializing ESCAPI.  Wrong version of ESCAPI. Requires 3.0.");
		}
		else if (m_numCameras == 0)
		{
			m_logMsg.Format("%s: no cameras found.\n", __FUNCTION__);
		}
		else
		{
			params.mWidth     = m_width;
			params.mHeight    = m_height;
			params.mTargetBuf = (int *)m_buf;	// m_buf gets set in Init()

         this->refreshDeviceList();
			
			if (initCapture(m_cameraIndex, &params))
			{
				m_inited = true;	// assigning it here avoids an annoying compiler warning
			}
		}
	}

	return m_inited;
}


bool SimpleDirectShowAcq::refreshDeviceList() {

   bool success = true;
   m_device_names.clear();

   for (int iDevice = 0; iDevice < m_numCameras; ++iDevice) {

      char * buffer = new char[max_string_size + 1]();

      try {
         getCaptureDeviceName(iDevice, buffer, max_string_size);
         success &= true;
      } catch(...) {
         success &= false;
      }

      m_device_names.push_back(std::string(buffer));

      delete[] buffer;
   }

   return success;
}

bool SimpleDirectShowAcq::refreshNumCameras()
{
   bool success = false;
   int temp = 0;
   try {
      
      temp = countCaptureDevices();
      m_numCameras = temp;
      success = true;

   } catch (...) {
   }

   return success;
}

std::vector<std::string>  SimpleDirectShowAcq::getCameraNames() {

   if (m_inited) {
      this->refreshNumCameras();
      this->refreshDeviceList();
   }

   return m_device_names;

}

bool SimpleDirectShowAcq::setCameraIndex(int id) {
   bool success = false;

   if (id <= m_numCameras && m_cameraIndex != id) {
      
      success = this->Close();

      m_cameraIndex = id;

      success &= this->Open();
   }

   return success;
}

int SimpleDirectShowAcq::getCameraIndex() {

   return m_cameraIndex;

}


///
/// @return Returns TRUE upon completion
///
bool SimpleDirectShowAcq::Close()
{
	if (m_inited)
	{
		deinitCapture(m_cameraIndex);
	}

	return true;
}

///
/// @return Returns TRUE upon completion
///
bool SimpleDirectShowAcq::CaptureFrame()
{
	doCapture(m_cameraIndex);

	while (!isCaptureDone(m_cameraIndex))	// captures into m_buf
	{
		Sleep(1);
	}

	return true;
}
