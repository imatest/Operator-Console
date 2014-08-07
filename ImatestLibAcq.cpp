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
	
	mwArray toRGBrows =  mwArray(1,1,mxDOUBLE_CLASS);
	toRGBrows=1.0;// by being TRUE we call for the image to be in column major format

	mwArray deviceID((mxDouble)m_device_ID);// select which source we are using

	mwArray ini_file(m_ini_file.c_str());	// the Omnivision part of acquire_image needs to read from 
											// the correct imatest.ini file
	//const double varArgIn[] = {1.0, 1.0};   
	mwArray vararginParam = mwArray(1,2,mxCELL_CLASS);
	vararginParam.Get(1,1).Set(toRGBrows);		

	if ( m_source_ID == 2)
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
	}
	catch (const mwException& e)
    {
        cout << "Run Error!" << endl;
        cerr << e.what() << endl;
		mwException e2 = e;
		e2.print_stack_trace();
	}

	//im_orig.GetData((mxUint64*)m_buf,(mwSize)im_orig.ElementSize());
	mwSize dataSize = im_orig.NumberOfElements();

	im_orig.GetData((mxUint8*)m_buf, dataSize);
	
	

	//m_buf = (char*)mxGetPr(im_orig);
	//doCapture(m_cameraIndex);

	//while (!isCaptureDone(m_cameraIndex))	// captures into m_buf
	//{
	//	Sleep(1);
	//}

	return true;
}
