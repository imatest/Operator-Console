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
#include "imatest_library.h"
#include "ImatestLibAcq.h"
#include "ThreadControl.h"
#include "imatest_acquisition.h"
#include "ImatestSourceIDs.h"
#include <iostream>

using namespace std;

#define IMAQ_WEBCAM_SOURCE_ID 128
#define EPIPHAN_SOURCE_ID 6
#define EPIPHAN_DEVICE_ID 0.0

// This is the source id for the first Image acquisition toolkit device
// const unsigned short isource_id = IMAQ_WEBCAM_SOURCE_ID;
const unsigned short isource_id = EPIPHAN_SOURCE_ID;

ImatestLibAcq::ImatestLibAcq(void)
{
   m_width     = 0;
   m_height    = 0;
   m_numBytes  = 0;
   m_numPixels = 0;
   m_buf       = NULL;
   m_source_ID = 0;
   m_device_ID = 0;
   m_ini_file = "";
}


ImatestLibAcq::~ImatestLibAcq(void)
{
}

bool ImatestLibAcq::Open()
{
   //struct SimpleCapParams	params;
   bool					success = false;

   m_inited = true;

   if (m_width == 0 || m_height == 0)	// these get set in Init()
   {
      m_logMsg = "Unable to Open camera:  width and height are not set.";
   }
   else
   {
   }

   return m_inited;
}



bool ImatestLibAcq::Close()
{
   if (m_inited)
   {
      //deinitCapture(m_cameraIndex);
   }

   return true;
}

bool ImatestLibAcq::CaptureFrame()
{

   // This required parameter will hold return data.
   mwArray out;
   //mwArray nover;

   // Load our file in
   //raw_pixels_char = load_raw_file(BLEMISH_RAW_FILE, &pixel_count);
   mwArray im_orig, vstr;

   mwArray source_id((mxDouble)m_source_ID);

   mwArray toRGBrows =  mwArray(1,1,mxLOGICAL_CLASS);
   toRGBrows=true;// by being TRUE we call for the image to be in column major format

   mwArray deviceID((mxDouble)m_device_ID);// select which source we are using

   mwArray ini_file(m_ini_file.c_str());	// the Omnivision part of acquire_image needs to read from 
   // the correct imatest.ini file
   //const double varArgIn[] = {1.0, 1.0};   
   mwArray vararginParam = mwArray(1,2,mxCELL_CLASS);
   vararginParam.Get(1,1).Set(toRGBrows);		

   if ( m_source_ID == SOURCE_Omnivision || m_source_ID == SOURCE_File)
   {
      vararginParam.Get(1,2).Set(ini_file);				// Path to INI file
   }
   else
   {
      vararginParam.Get(1,2).Set(deviceID);				// 16bit RAW data
   }

   try 
   {
      acquire_image(2, im_orig, vstr, source_id, vararginParam);
      m_ncolors = 4; // We get an RGBA image back, so 4 colors
   }
   catch (mwException& e)
   {
      cout << "Run Error!" << endl;
      cerr << e.what() << endl;
      e.print_stack_trace();
   }

   // Now that we have an image, we need to copy the image separately into the data buffer (m_buf) and the display
   // buffer. For display on the monitor, we must rescale the pixel values such that they will 'fit' within the restriction
   // of 1 byte per color. The image returned from acquire_image() in the variable im_orig() is a 1D m x n x 4 array of
   // doubles.

   mwSize numElements = im_orig.NumberOfElements();

   // Copy the data stored in im_orig to a local buffer
   double* buffer = new double[numElements];
   im_orig.GetData((mxDouble*)buffer, numElements);

   // Find the maximum value so that we can at least guess
   // at the bit depth of the data.
   int maxVal = 0;
   for (size_t idx = 0; idx < numElements; ++idx) 
   {
      if ( buffer[idx] > maxVal)
         maxVal = static_cast<int>(buffer[idx]);
   }

   // From the maximum value guess at the bit-depth and determine the scale factor ( = (2^8 - 1)/(2^x - 1))
   double scaleFactor = 1.0;
   int bytesPerColor = 1;
   if (maxVal < 256)
   {
      scaleFactor = 1.0; 
      bytesPerColor = 1;
   }
   else 
   {
      int exponent = static_cast<int>(std::ceil(std::log(maxVal)/std::log(2))); // 
      scaleFactor = 255.0/(pow(2.0,exponent) - 1);

      if (exponent <= 16)
      {
         bytesPerColor = 2;
      }
      else if (exponent <= 32)
      {
         bytesPerColor = 4;
      }
      else if (exponent <= 64)
      {
         bytesPerColor = 8;
      }
   }
   
   // Copy the data directly from im_orig into the data buffer, m_buf,
   // with out any scaling (or fear of losing data)
   m_numBytes = m_numPixels*bytesPerColor*m_ncolors;
   Init(m_width, m_height, bytesPerColor*m_ncolors);
   switch (bytesPerColor) {   
   case 1:
      im_orig.GetData((mxUint8*)m_buf, numElements);
      break;
   case 2:      
      im_orig.GetData((mxUint16*)m_buf, numElements);
      break;
   case 4:
      im_orig.GetData((mxUint32*)m_buf, numElements);
      break;
   case 8:
       im_orig.GetData((mxUint64*)m_buf, numElements);
      break;
   }

   // Rescale the data for display
   for (size_t idx = 0; idx < numElements; ++idx) {
      buffer[idx] *= scaleFactor;
   }
   im_orig.SetData((mxDouble*)buffer, numElements);

   // Get the scaled data for the display buffer, m_displayBuf
   im_orig.GetData((mxUint8*)m_displayBuf, numElements);
   
   delete[] buffer;

   return true;
}
