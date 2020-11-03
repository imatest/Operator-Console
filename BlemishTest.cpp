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

#include "BlemishTest.h"
#include "imatest_library.h"
#include "mclcppclass.h"

using namespace std;

#define TEST_NAME			"Blemish"
#define ALL_TESTS			"all_tests_passed"
#define INI_RAW_JSON_MODE	"-17"
#define PIXEL_ERROR_REGIONS 9   // for blemish counts, the frame of the image is split into nine areas

#define NUM_TESTS	(sizeof(gTestID) / sizeof(gTestID[0]))


static const TestID gTestID[] = 
{
	{"Dead_pixels_passed", "Dead Pixels"},
	{"Hot_pixels_passed", "Hot Pixels"},
	{"Optical_center_offset_X_passed", "Center Offset X" },
	{"Optical_center_offset_Y_passed", "Center Offset Y" },
	{"Optical_center_offset_passed", "Center Offset"},
	{"Relative_illumination_worst_corner_passed", "Corner Darkness"},
	{"Relative_illumination_corner_diff_pct_passed", "Corner Delta"},
	{"Uniformity_RoverG_corners_pct_passed", "R over G "},
	{"Uniformity_BoverG_corners_pct_passed", "B over G "},
	{"Blemishes_all_passed", "Blemishes"},
};


BlemishTest::BlemishTest(void)
{
	m_name        = TEST_NAME;
	m_tests       = gTestID;
	m_numTests    = NUM_TESTS;
	m_imatestFunc = blemish_shell;
}


BlemishTest::~BlemishTest(void)
{
}

#if 0
//
// The Run method has been moved up to the parent class (ImageTest).  The Imatest lib function
// now gets called using the m_imatestFunc member variable.  This makes it easier to modify and 
// maintain the Run methods.  The Run method is still virtual, so it can be overridden if desired.
//
void BlemishTest::Run(void *raw_pixels, int width, int height, const Config *config)
{
	Init(raw_pixels, width, height, config);
	Run();
}

void BlemishTest::Run()
{
	json_string		test_settings;
	int				crop_borders[4] = {0,0,0,0};

	InitResults();	// clear out strings, set m_passed to false

	prepareTestSettings(m_width, m_height, 1, m_extension, m_fileroot, m_serialNumber, m_partNumber, crop_borders, test_settings);

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
		mwArray rawFileParam(m_width * m_height, 1, mxUINT16_CLASS, mxREAL);	// Argument 6 - Raw Data
		mwArray	json_options_param(1,json_options);								// JSON options describing the RAW file;
		mwArray	vararginParam(1,3,mxCELL_CLASS);								// The remaining arguments go in this param
		mwArray	out;															// this will hold returned data

		rawFileParam.SetData((unsigned short *)m_rawPixels, m_width * m_height);

		// Our remaining arguments go into the Varargin parameter
		vararginParam.Get(1,1).Set(iniFileParam);			// Path to INI file
		vararginParam.Get(1,2).Set(rawFileParam);			// 16bit RAW data
		vararginParam.Get(1,3).Set(json_options_param);		// options file

		//
		// Call the Imatest IT library function
		//
//		blemish_shell(1, out, fileParam, pathParam, keysParam, modeParam, vararginParam); 
		m_imatestFunc(1, out, fileParam, pathParam, keysParam, modeParam, vararginParam);

			//
		// Copy the output of the module to a System::String object
		//
		stringstream ss (stringstream::in | stringstream::out);
		ss << out;
		string jsonResults = ss.str();
		ParseResults(jsonResults);
		SetJSON(jsonResults);	// copy results into m_jsonResults (m_jsonResults is shared between threads, so we don't want to access it directly)
	}

	catch (mwException &e)
	{
		CString log;
		log.AppendFormat("%s Error running %s test\n%s\n", (LPCTSTR)m_timestamp.Get(), TEST_NAME, e.what());
		AppendLog(log);
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
		m_passed = false;
	}
}
#endif

///
/// Parses the JSON output of an sfrPlus test to determine the pass/fail status of the various metrics. 
///
void BlemishTest::ParseResults(std::string &results)
{
	JSONNode	passfail;
    JSONNode	data;

	InitResults();

	if (GetDataNode(results, data) && GetPassFailNode(data, passfail))// this will copy any errors into m_log
	{
		//
		// Get the pass/fail results
		// 
		try 
		{
			m_passed = passfail.at("all_tests_passed").as_array()[0].as_bool();

			if (!m_passed)
			{
				ParseFailures(&passfail);
			}
		}
		catch(...) 
		{
			CString str("Unable to find pass/fail result");
			m_failInfo.Set(str);
			m_log.Append(str);
			m_passed = false;
		}

		//
		// Get the results of interest from the data and passfail node
		//
		GetResults(data, passfail);
	}
}


void BlemishTest::GetResults(const JSONNode& data, const JSONNode &passfail)
{
	CString		summary;
	JSONNode	pixelErrors;
	JSONNode	blemishSize;
	json_string	sizes;
	int			total;
	int			dead;
	int			hot;
	int			blemishes;
	const char	*format = "Total pixel errors %d\r\nDead pixels %d\r\nHot pixels %d\r\nBlemishes %d\r\nBlemish size (pixels) %s";

	try
	{
		pixelErrors = data.at("pixelErrors").as_array();
		total       = sumArrayInt(pixelErrors, PIXEL_ERROR_REGIONS);
		blemishes   = getIntFromArray(data, "N_blemish_count");
		
		// Note: 'blemishSizePxls' is only written to JSON output if N_blemish_count > 0

		if (blemishes > 0) {
			blemishSize = data.at("blemishSizePxls").as_array();


			// Note: blemishSize is limited to 50 entries in the Imatest library and does 
			// not necessarily equal the JSON entry "N_blemish_count" = blemishes
			if (blemishes < (int)blemishSize.size())
			{
				getStringArray(blemishSize, blemishes, &sizes);
			}
			else
			{
				getStringArray(blemishSize, blemishSize.size(), &sizes);
			}
		}

		dead = passfail.at("Dead_pixels").as_array()[0].as_int();
		hot = passfail.at("Hot_pixels").as_array()[0].as_int();
		summary.Format(format, total, dead, hot, blemishes, sizes.c_str());
		m_summary.Set(summary);
	}

	catch(...)
	{
		CString	log;
		log.AppendFormat("%s Error parsing results\n", (LPCTSTR)m_timestamp.Get());
		AppendLog(log);
	}
}
