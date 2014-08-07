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
#include <sstream>
#include <string>
#include <string.h>
#include "ImageTest.h"
#include "OperatorConsole.h"
//
// This defines the delimiter to use when formatting the string of failed tests
//
#define FAIL_DELIMITER	"\r\n"

ImageTest::ImageTest(void)
{
	m_width           = 0;
	m_height          = 0;
	m_extension       = NULL;
	m_fileroot        = NULL;
	m_serialNumber    = NULL;
	m_partNumber      = NULL;
	m_iniFilePathName = NULL;
	m_programPath     = NULL;
	m_rawPixels		  = NULL;
	m_name            = NULL;
	m_imatestFunc     = NULL;
	m_rgb             = NULL;
}


ImageTest::~ImageTest(void)
{
	DeleteRGB();
}

void ImageTest::Init(void *raw_pixels, int width, int height, const Config *config)
{
	m_rawPixels       = raw_pixels;
	m_width           = width;
	m_height          = height;
	m_fileroot        = config->m_fileRoot;
	m_serialNumber    = config->m_serialNumber;
	m_partNumber      = config->m_partNumber;
	m_extension       = config->m_extension;
	m_iniFilePathName = config->m_iniFilePathName;
	m_programPath     = config->m_programPath;
	m_ncolors         = config->m_ncolors;

	AllocateRGB();
}

bool ImageTest::AllocateRGB()
{
	//
	// If the source data is 3-channel (i.e. rgb) in rgba format, then
	// we will need to convert it into a planar format for MATLAB.  
	//
	if (m_ncolors == 3)
	{
		DeleteRGB();
		m_rgb = new unsigned char[m_width * m_height * 3];
	}

	return m_rgb != NULL;
}

void ImageTest::DeleteRGB()
{
	if (m_rgb != NULL)
	{
		delete [] m_rgb;
		m_rgb = NULL;
	}
}

/// converts from m_rawPixels to m_rgb
void ImageTest::PlanesFromRGB()
{
	int				i, numPixels;
	unsigned char	*r, *g, *b;		// pointers into m_rgb
	unsigned char	*R, *G, *B;		// pointers into m_rawPixels


	//
	// m_rawPixels is an array of RGBQUAD structs (32-bit ints), where each byte represents one color component (r, g, b), plus
	// one unused byte.  MATLAB stores an RGB image sequentially, with all of the red values followed by all of the green values, 
	// followed by all of the blue values.  This function converts an arry of RGBQUAD values into MATLAB's color plane format.
	//
	numPixels = m_width * m_height;
	r = m_rgb;
	g = r + numPixels;
	b = g + numPixels;

	R = (unsigned char *)m_rawPixels;
	G = R+1;
	B = G+1;

	// TODO, vectorize this loop?
	for (i = 0; i < numPixels; i++)
	{
		*r = *R;
		*g = *G;
		*b = *B;

		r++;
		g++;
		b++;
		R += 4;
		G += 4;
		B += 4;
	}
}

/// converts from m_rgb to rgb
void ImageTest::RGBFromPlanes(UINT *rgb)
{
	int				i, numPixels;
	unsigned char	*r, *g, *b;		// pointers into m_rgb
	unsigned char	*R, *G, *B;		// pointers into m_rawPixels


	//
	// rgb is an array of 32-bit ints, arranged in rgba order.
	// MATLAB stores an RGB image sequentally, with all of the red values followed
	// by all of the green values, followed by all of the blue values.
	// 
	// This function converts from MATLAB's color plane format into rgb format
	//
	numPixels = m_width * m_height;
	r = m_rgb;
	g = r + numPixels;
	b = g + numPixels;

	R = (unsigned char *)rgb;
	G = R+1;
	B = G+1;

	for (i = 0; i < numPixels; i++)
	{
		*R = *r;
		*G = *g;
		*B = *b;

		r++;
		g++;
		b++;
		R += 4;
		G += 4;
		B += 4;
	}
}

void ImageTest::Run(void *raw_pixels, int width, int height, const Config *config)
{
	Init(raw_pixels, width, height, config);
	Run();
}

///
/// Runs the test pointed to by m_imatestFunc
///
void ImageTest::Run()
{
	json_string		test_settings;
	int				crop_borders[4] = {0,0,0,0};

	InitResults();	// clear out strings, set m_passed to false

//#ifdef IMATEST_CAMERA
//	RGBFromPlanes((UINT *)m_rawPixels);	
//#else
	PlanesFromRGB();					// convert m_rawPixels from rgba format to planar format for MATLAB
//#endif
	//	RGBFromPlanes((UINT *)m_rawPixels);	// convert it back, just for fun

#if 0
	//
	// Save the data as a png file
	//
	UINT *tmp = new UINT[m_width*m_height];
	RGBFromPlanes(tmp);	
	SaveImage((RGBQUAD *)tmp, "image.png");
	delete [] tmp;
#endif
	prepareTestSettings(m_width, m_height, m_ncolors, m_extension, m_fileroot, m_serialNumber, m_partNumber, crop_borders, test_settings);

	const char	*operation_flag[] = {INI_RAW_JSON_MODE};
	const char	*file_name[]      = {""};
	const char	*iniFile[]        = {m_iniFilePathName};
	const char	*program_path[]   = {m_programPath};
	const char	*input_keys[]     = {"JSON"}; 
	const char	*json_options[]   = {test_settings.c_str()}; // make sure to call prepareTestSettings() first

	//
	// To create mwArrays using the constructor prototype: 
	// mwArray(mwSize num_strings, const char** str)
	//
	try
	{
		mwArray fileParam(1, file_name);										// Argument 1 - File name (Not applicable for raw files)
		mwArray pathParam(1, program_path);										// Argument 2 -	Program Path
		mwArray keysParam(1, input_keys);										// Argument 3 - Mode of Control
		mwArray modeParam(1, operation_flag);									// Argument 4 - Operation Flag
		mwArray iniFileParam(1, iniFile);										// Argument 5 INI file
//		mwArray rawFileParam(m_width * m_height, 1, mxUINT16_CLASS, mxREAL);	// Argument 6 - Raw Data
		mwArray rawFileParam(m_width * m_height * 3, 1, mxUINT8_CLASS, mxREAL);	// Argument 6 - RGB Data
		mwArray	json_options_param(1,json_options);								// JSON options describing the RAW file;
		mwArray	vararginParam(1,3,mxCELL_CLASS);								// The remaining arguments go in this param
		mwArray	out;															// this will hold returned data

#ifdef IMATEST_CAMERA
		
		rawFileParam.SetData(m_rgb, m_width * m_height * 3);
		//rawFileParam.SetData((unsigned short *)m_rawPixels, m_width * m_height);
#else
		rawFileParam.SetData(m_rgb, m_width * m_height * 3);
#endif
		// Our remaining arguments go into the Varargin parameter
		vararginParam.Get(1,1).Set(iniFileParam);			// Path to INI file
		vararginParam.Get(1,2).Set(rawFileParam);			// data description
		vararginParam.Get(1,3).Set(json_options_param);		// options file

		//
		// Call the Imatest IT library function
		//
		if (m_imatestFunc == NULL)
		{
		}
		else
		{
			m_imatestFunc(1, out, fileParam, pathParam, keysParam, modeParam, vararginParam);
		}

		//
		// Copy the output of the module to a System::String object
		//
		stringstream ss (stringstream::in | stringstream::out);
		ss << out;
		string jsonResults = ss.str();
		ParseResults(jsonResults);
		SetJSON(jsonResults);	// copy results into m_jsonResults (m_jsonResults is shared between threads, so we don't want to access it directly)
	}

	catch (const mwException &e)
	{
		CString log;
		log.AppendFormat("%s Error running %s test\n%s\n", (LPCTSTR)m_timestamp.Get(), m_name, e.what());
		log.Replace("\n", "\r\n");	// the exception may have '\n' characters that we need to replace with "\r\n"
		AppendLog(log);
		mwException e2 = e;
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e2.print_stack_trace();
		m_passed = false;
	}
}




bool ImageTest::GetDataNode(string &results, JSONNode &data, bool logErrors)
{
	bool		success = false;
	JSONNode	node;
	CString		log;

	try
	{
		node = libjson::parse(results.c_str());
		data = node.at(0);
		success = true;
	}
	catch(...)
	{
		if (logErrors)
		{
			log.AppendFormat("%s Results have invalid format\n", (LPCTSTR)m_timestamp.Get());
		}

		success = false;
	}
	
	m_log.Append(log);

	return success;
}


bool ImageTest::GetPassFailNode(JSONNode &data, JSONNode &passFail, bool logErrors)
{
	bool	validNode;
	CString	log;

	try 
	{
		passFail = data.at("passfail");
		validNode = true;
	}
	
	catch(...) 
	{
		validNode = false;
		
		if (logErrors)
		{
			log.AppendFormat("%s Couldn't find pass/fail section in results\n", (LPCTSTR)m_timestamp.Get());
		}
	}

	m_log.Append(log);

	return validNode;
}

void ImageTest::GetString(JSONNode &node, const char *name, json_string &string, bool logErrors)
{
	try
	{
		string = node.at(name).as_array()[0].as_string();
	}

	catch(...)
	{
		CString	log;
		log.AppendFormat("%s Couldn't find %s value\n", (LPCTSTR)m_timestamp.Get(), name);
		m_log.Append(log);
	}
}

void ImageTest::ParseFailures(const JSONNode *passfail)
{
	CString	fail;
	CString	log;
	int		i;
	bool	first  = true;
	bool	passed = false;


	for (i = 0; i < m_numTests; i++)
	{
		try 
		{
			passed = passfail->at(m_tests[i].key).as_array()[0].as_bool();
			
			if (!passed)
			{
				if (!first)
				{
					fail.Append(FAIL_DELIMITER);	// add a separator before adding the next test name
				}

				fail.Append(m_tests[i].name);	// add the name of the test that failed to the m_failInfo string
				first = false;
			}
		}
		
		catch(...) 
		{
			log.AppendFormat("Couldn't find %s in results\n", m_tests[i].key);
		}
	}

	m_failInfo.Set(fail);
	m_log.Append(log);
}

UINT __cdecl ImageTest::ThreadProc(void *param)
{
	ThreadControl	*info = (ThreadControl *)param;
	ImageTest		*test = (ImageTest *)info->m_data;
	bool			done  = false;
	DWORD			eventIndex;


	while (!done)
	{
		//
		// Wait for an event to be signaled.  info->m_events is an array of 2 CEvent structs.
		// 'eventIndex' tells us which event was signaled.  Once we know which event was signaled
		// we can take the appropriate action.
		//
		// While we're waiting for an event, our thread blocks, allowing other threads (like the 
		// GUI handler) to run.
		//
		eventIndex = ::WaitForMultipleObjects(NUM_THREAD_EVENTS, info->m_events, FALSE, INFINITE);	// wait for an event to be signaled

		if (eventIndex == DONE_INDEX)
		{
			//
			// We get a DONE event when the program is terminating.  We just exit the loop and return.
			//
			done = TRUE;
		}
		else if (eventIndex == RUN_INDEX)
		{
			//
			// We get a RUN event when it's time to run the test.  We send a
			// message to the main application thread when the test is done.
			//
			test->m_time.Start();
			test->Run();
			test->m_time.Stop();
			test->m_time.Get(&test->m_elapsedStr);
			test->m_time.Get(&test->m_elapsed);
			::PostThreadMessage(info->m_parentID, info->m_threadMsg, (WPARAM)0, (LPARAM)0);
		}
	}

	return 0;	// this terminates the thread
}


void ImageTest::InitResults()
{
	m_jsonResults.Clear();
	m_summary.Clear();
	m_failInfo.Clear();
	m_log.Clear();
	m_passed = false;
}


void ImageTest::SaveImage(RGBQUAD *buf, const char *filename)
{
	CImage		image;
	HDC			hdc;
	HBITMAP		hbitmap;
	BITMAPINFO	info;


	memset(&info, 0, sizeof(info));
	info.bmiHeader.biSize          = sizeof(info.bmiHeader);
	info.bmiHeader.biWidth         = m_width;
	info.bmiHeader.biHeight        = -m_height;	// a negative value means top-down instead of bottom-up
	info.bmiHeader.biPlanes        = 1;
	info.bmiHeader.biBitCount      = 32;
	info.bmiHeader.biCompression   = BI_RGB;
	info.bmiHeader.biSizeImage     = 0;
	info.bmiHeader.biXPelsPerMeter = 1;
	info.bmiHeader.biYPelsPerMeter = 1;
	info.bmiHeader.biClrUsed       = 0;
	info.bmiHeader.biClrImportant  = 0;

	image.Create(m_width, m_height, 32);
	hdc = image.GetDC();
	hbitmap = (HBITMAP)image;

	SetDIBits(hdc, hbitmap, 0, m_height, buf, &info, DIB_RGB_COLORS);
	image.ReleaseDC();
	image.Save(filename, Gdiplus::ImageFormatPNG);
}
