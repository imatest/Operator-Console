#pragma once


//
// Hard coded values go here.  Most or all will eventually come from a Config or Setup dialog.
//
// IMAGE_NAME is the name of the static file to be displayed in the application window (until live acquisition is implemented)
// RAW_BLEMISH_IMAGE is the name of the file to use for blemish tests
// RAW_SFRPLUS_IMAGE is the name of the file to use for SFRplus tests
// RAW_WDITH and RAW_HEIGHT are the width and height of the images in pixels
//
#define LOGO_NAME			"Data\\imatest_logo.png"	// imatest logo to be displayed in dialog
#define IMAGE_NAME			"Data\\sfrplus.raw.png"			// image file to be displayed in the application window (until live acquisition is implemented)
#define RAW_BLEMISH_IMAGE	"Data\\blemish.raw"				// raw file to use for blemish tests
#define RAW_SFRPLUS_IMAGE	"Data\\sfrplus.raw"				// raw file to use for SFRplus tests
#define RAW_WIDTH_BLEMISH	1296								// width  of raw blemish file in pixels
#define RAW_HEIGHT_BLEMISH	808									// height of raw blemish file in pixels
#define RAW_WIDTH_SFRPLUS	1296								// width  of raw sfrplus file in pixels
#define RAW_HEIGHT_SFRPLUS	808									// height of raw sfrplus file in pixels
// #define ADMIN_PASSWORD		"Imatest%840cX"						// password for Administrator access to the pass/fail settings
#define ADMIN_PASSWORD		"a"						// password for Administrator access to the pass/fail settings

#define IMATEST_CAMERA
//  NONO #define FAKE_CAMERA	// if this is defined, data comes from the file specified by IMAGE_NAME

//
// Size of an image from the camera (in pixels).  Ultimately the RAW_ definitions above will go
// away and we'll only be using the camera for images, but right now we're not there yet.
//
// TODO: Define these to the proper size for your camera
//
//#define RES_XGA
//#define RES_VGA
#define RES_WVGA

#if defined RES_VGA
#define CAMERA_WIDTH	640
#define CAMERA_HEIGHT	480
#elif defined RES_WVGA
#define CAMERA_WIDTH	800
#define CAMERA_HEIGHT	600
#elif defined RES_SVGA
#define CAMERA_WIDTH	800
#define CAMERA_HEIGHT	480
#elif defined RES_XGA
#define CAMERA_WIDTH	1024
#define CAMERA_HEIGHT	768
#endif


/// Define different keys for differing Imatest interfaces based on version
#if defined IMATEST_4_2

#define INI_INCLUDE_SUBSECTION
#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_4_3

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_4_4

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_4_5

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_5_0

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_5_1

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_5_2

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_2020_1

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_2020_2

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#else

// All versions below 4.2
#define INI_INCLUDE_SUBSECTION

#endif

#ifdef DEMO_VERSION
#define NO_AUTHORIZATION
#endif

//
// These values are put into m_config, and get passed to the sfrplus and blemish functions
//
#ifdef INI_V2_FORMAT
#define INI_FILENAME		"Data\\imatest-v2.ini"
#else
#define INI_FILENAME		"Data\\imatest.ini"
#endif

#if defined IMATEST_4_2
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.2\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.2\\IT\\bin"
#endif


#elif defined IMATEST_4_3
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.3\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.3\\IT\\bin"
#endif


#elif defined IMATEST_4_4
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.4\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.4\\IT\\bin"
#endif

#elif defined IMATEST_4_5
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.5\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.5\\IT\\bin"
#endif

#elif defined IMATEST_5_0
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.0\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v5.0\\IT\\bin"
#endif

#elif defined IMATEST_5_1
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.1\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v5.1\\IT\\bin"
#endif

#elif defined IMATEST_5_2
#ifdef _M_X64
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.2\\IT\\bin"
#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v5.2\\IT\\bin"
#endif

#elif defined IMATEST_2020_1
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v2020.1\\IT\\bin"

#elif defined IMATEST_2020_2
#define PROGRAMPATH			"C:\\Program Files\\Imatest\\v2020.2\\IT\\bin"

#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.1\\IT\\bin"
#endif

#define RAW_EXTENSION		"raw"
#define RGB_EXTENSION		"rgb2"
#define FILE_ROOT			"NoFile"
#define SERIAL_NUMBER		"123"
#define PART_NUMBER			"720PCam"