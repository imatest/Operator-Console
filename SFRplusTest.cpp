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

#include "SFRplusTest.h"
#include "Config.h"
#if 0
#include "imatest_sfrplus.h"	// header for dll
#else
#include "imatest_library.h"
#endif
#include "mclcppclass.h"
#include "libjson.h"
#include "jsonhelpers.h"
#include "libjson.h"
#include "jsonhelpers.h"
#include "JSON\JSONDefs.h"


using namespace std;

#define TEST_NAME			"SFRplus"
#define INI_RAW_JSON_MODE	"-17"


#define ALL_TESTS	"all_tests_passed"
#define NUM_TESTS	(sizeof(gTestID) / sizeof(gTestID[0]))


static const TestID gTestID[] = 
{
	{"Horizontal_bars_OK_passed",                          "Horizontal bars"},
	{"All_Edge_IDs_detected_passed",                       "Edge IDs"},
	{"Mirrored_chart_passed",                              "Mirrored chart"},
	{"Bayer_decode_passed",                                "Bayer decode"},
	{"Rotation_degrees_passed",                            "Rotation degrees"},
	{"FOV_passed",                                         "FOV"},
	{"Horizontal_convergence_angle_passed",                "Horizontal convergence angle"},
	{"Vertical_convergence_angle_passed",                  "Vertical convergence angle"},
	{"Secondary_readout_1_center_mean_passed",             "Secondary readout 1 center mean"},
	{"Secondary_readout_1_outer_mean_passed",              "Secondary readout 1 outer mean"},
	{"Secondary_readout_2_center_mean_passed",             "Secondary readout 2 center mean"},
	{"Secondary_readout_2_outer_mean_passed",              "Secondary readout 2 outer mean"},
	{"Secondary_readout_1_outer_quadrant_delta_passed",    "Secondary readout 1 outer quadrant delta"}, 
	{"Secondary_readout_1_outer_quadrant_mean_min_passed", "Secondary readout 1 outer quadrant mean min"},
	{"Secondary_readout_2_outer_quadrant_delta_passed",    "Secondary readout 2 outer quadrant delta"},
	{"Secondary_readout_2_outer_quadrant_mean_min_passed", "Secondary readout 2 outer quadrant mean min"},
	{"High_pixel_saturation_fraction_passed",              "High pixel saturation fraction"},
	{"Low_pixel_saturation_fraction_passed",               "Low pixel saturation fraction"},
	{"Chart_mean_pixel_level_passed",                      "Chart mean pixel level"},
	{"Chart_radial_pixel_shift_passed",                    "Chart radial pixel shift"},
};


SFRplusTest::SFRplusTest(void)
{
	m_name        = TEST_NAME;
	m_tests       = gTestID;
	m_numTests    = NUM_TESTS;
	m_imatestFunc = sfrplus_shell;
}


SFRplusTest::~SFRplusTest(void)
{
}

#if 0
//
// The Run method has been moved up to the parent class (ImageTest).  The Imatest lib function
// now gets called using the m_imatestFunc member variable.  This makes it easier to modify and 
// maintain the Run methods.  The Run method is still virtual, so it can be overridden if desired.
//
void SFRplusTest::Run(void *raw_pixels, int width, int height, const Config *config)
{
	Init(raw_pixels, width, height, config);
	Run();
}

void SFRplusTest::Run()
{
	json_string		test_settings;
	int				crop_borders[4] = {1,1,1,1};

	InitResults();	// clear strings, set m_passed to false

	prepareTestSettings(m_width, m_height, 1, m_extension, m_fileroot, m_serialNumber, m_partNumber, crop_borders, test_settings);

	const char* operation_flag[] = {INI_RAW_JSON_MODE};
	const char* file_name[]      = {""};
	const char* iniFile[]        = {m_iniFilePathName};
	const char* program_path[]   = {m_programPath};
	const char* input_keys[]     = {"JSON"}; 
	const char* json_options[]   = {test_settings.c_str()};

	//
	// To create mwArrays using the constructor prototype: 
	// mwArray(mwSize num_strings, const char** str)
	//
	try
	{
		mwArray fileParam(1, file_name);			// Argument 1 - File name (Not applicable for raw files)
		mwArray pathParam(1, program_path);			// Argument 2 -	Program Path
		mwArray keysParam(1, input_keys);			// Argument 3 - Mode of Control
		mwArray modeParam(1, operation_flag);		// Argument 4 - Operation Flag
		mwArray iniFileParam(1, iniFile);			// Argument 5 INI file
		
		int nPixels = m_width * m_height * 3;

		mwArray out;								// this will hold returned data
		mwArray	json_options_param(1,json_options);	// JSON options describing the RAW file	
		mwArray	vararginParam(1,3,mxCELL_CLASS);	// The remaining arguments go in this param
		mwArray rawFileParam(nPixels, 1, mxUINT16_CLASS, mxREAL);

		rawFileParam.SetData((unsigned short *)m_rawPixels, nPixels);

		// Our remaining arguments go into the Varargin parameter
		vararginParam.Get(1,1).Set(iniFileParam);				// Path to INI file
		vararginParam.Get(1,2).Set(rawFileParam);				// 16bit RAW data
		vararginParam.Get(1,3).Set(json_options_param);			// options file

		// Call the Imatest IT library function
//		sfrplus_shell(1, out, fileParam, pathParam, keysParam, modeParam, vararginParam); 
		m_imatestFunc(1, out, fileParam, pathParam, keysParam, modeParam, vararginParam); 

		//
		// Copy the output of the module to a System::String object
		//
		stringstream ss (stringstream::in | stringstream::out);
		ss << out;
		string jsonResults = ss.str();
		ParseResults(jsonResults);
		SetJSON(jsonResults);	// copy results into m_results (m_results is shared between threads)
	}
	catch (mwException &e)
    {
		CString	log;
		log.AppendFormat("Error running %s test\n%s\n", TEST_NAME, e.what());
		m_log.Append(log);
		mwException e2 = e;
		e2.print_stack_trace();
		m_passed = false;	
	}
}
#endif
///
/// Parses the JSON output of an sfrPlus test to determine the pass/fail status of the various metrics. 
///
void SFRplusTest::ParseResults(string &results)
{
	CString		summary;
	JSONNode	passfail;
    JSONNode	data;


	if (GetDataNode(results, data) && GetPassFailNode(data, passfail))
	{
		try
		{
			m_passed = passfail.at(ALL_TESTS).as_array()[0].as_bool();

			if (!m_passed)
			{
				ParseFailures(&passfail);
			}
		}

		catch(...) 
		{
			m_failInfo.Set("Couldn't find pass/fail result");
			m_passed = false;
		}

		GetResults(passfail);			// get the results (they're stored in the pass fail node)
		ParseOverlay(data, passfail);
	}
}

///
/// Gets the pass/fail results for a range of pass/fail metrics to be displayed in summary dialog
///
void SFRplusTest::GetResults(JSONNode &passfail)
{
	CString		summary;
	json_string	center1, outer1, center2, outer2, fov, hConverge, vConverge, rotation;
	char		*format = "MTF#1 Center %s\r\nMTF#1 Outer %s\r\nMTF#2 Center %s\r\nMTF#2 Outer %s\r\nFOV %s\r\nH Tilt %s\r\nV Tilt %s\r\nRotation %s\r\n";
	
	//
	// Get the data values (they're stored in the pass/fail section)
	// GetString() gets the value and handles any exceptions.
	//
	GetString(passfail, "Secondary_readout_1_center_mean", center1);
	GetString(passfail, "Secondary_readout_1_outer_mean", outer1);
	GetString(passfail, "Secondary_readout_2_center_mean", center2);
	GetString(passfail, "Secondary_readout_2_outer_mean", outer2);
	GetString(passfail, "FOV_degrees_diagonal", fov);
	GetString(passfail, "Horizontal_convergence_angle_degrees", hConverge);
	GetString(passfail, "Vertical_convergence_angle_degrees", vConverge);
	GetString(passfail, "Rotation_degrees", rotation);

	summary.Format(format, center1.c_str(), outer1.c_str(), center2.c_str(), outer2.c_str(), fov.c_str(), hConverge.c_str(), vConverge.c_str(), rotation.c_str());
	m_summary.Set(summary);
}

///
/// Determines the pass/fail result for each quadrant
///
void SFRplusTest::ParseOverlay(JSONNode &data, JSONNode &passfail)
{
	int		i;
	double	readout1[eNumQuadrants];
	double	readout2[eNumQuadrants];
	double	minCenter1, minCenter2;		// minimum allowed values for center quadrant
	double	minOuter1,  minOuter2;		// minimum allowed values for outer (non-center) quadrants
	double	min1, min2;

	memset(m_qso, 0, sizeof(m_qso));

	try
	{
		//
		// The readout data values are stored in the data section of the JSON buffer
		//
		getFloatArray(data.at("secondary_1_quadrant_mean").as_array(), readout1, eNumQuadrants);	// get the readout1 data values
		getFloatArray(data.at("secondary_2_quadrant_mean").as_array(), readout2, eNumQuadrants);	// get the readout2 data values

		//
		// The min allowed values are stored in the pass/fail section of the JSON buffer
		//
		minCenter1 = passfail.at("Secondary_readout_1_center_mean_min").as_array()[0].as_float();	// get the readout1 center min value
		minCenter2 = passfail.at("Secondary_readout_2_center_mean_min").as_array()[0].as_float();	// get the readout2 center min value

		try 
		{
			minOuter1  = passfail.at("Secondary_readout_1_outer_quadrant_mean_min_min").as_array()[0].as_float();	// get the readout1 outer quadrant min value
			minOuter2  = passfail.at("Secondary_readout_2_outer_quadrant_mean_min_min").as_array()[0].as_float();	// get the readout2 outer quadrant min value
		}
		catch (const std::exception &ex)
		{
			cerr << ex.what() << endl;
			cout << "Unable to locate 'Secondary_readout_1_outer_quadrant_mean_min_min' or" <<endl;
			cout << "'Secondary_readout_2_outer_quadrant_mean_min_min'. Will use 'Secondary_readout_1_outer_mean_min'" << endl;
			cout << " and 'Secondary_readout_2_outer_mean_min' instead. Note that there may be discrepancy between" << endl;
			cout << " the overall pass/fail result and the Quadrant-status overlays." << endl;
			minOuter1  = passfail.at("Secondary_readout_1_outer_mean_min").as_array()[0].as_float();	// get the readout1 outer min value
			minOuter2  = passfail.at("Secondary_readout_2_outer_mean_min").as_array()[0].as_float();	// get the readout2 outer min value
		}

		//
		// See whether or not each quadrant passed the test.
		//
		for (i = 0; i < eNumQuadrants; i++)
		{
			if (i == eCenter)
			{
				min1 = minCenter1;
				min2 = minCenter2;
			}
			else
			{
				min1 = minOuter1;
				min2 = minOuter2;
			}

			m_qso[i] = (readout1[i] >= min1) && (readout2[i] >= min2);
		}
	}

	catch (...)
	{
		CString	log;
		log.AppendFormat("%s Error parsing quadrant results\n", (LPCTSTR)m_timestamp.Get());
		m_log.Append(log);
	} 
}


void SFRplusTest::ClearQSO(void)
{
	for (int i1 = 0; i1 < eNumQuadrants; ++i1)
	{
		m_qso[i1] = false;
	}
}
