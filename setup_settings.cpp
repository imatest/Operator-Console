#include "setup_settings.h"
#include <locale>
#include "imatest_library.h"
#include "logger.h"
#include "logger_preferences.h"
#include "op_console_preferences.h"
#include "ImatestSourceIDs.h"
#include "ini_helpers.h"


//static CLogger logger("SetupSettings");
//
//static CFileLogHandler fileLogHandler(LOGGER_LEVEL, "setup_settings"); // A file extension is not necessary; *.log is automatically appended.


setup_settings::setup_settings() : width(0), height(0), bits_per_pixel(0), epiphan_deviceID(1), sourceID(6), bayer(0), directshow_deviceID(0) {

	/////////////////////////////////////////////////////
	//
	// Do not change these vectors without updating 
	// the functions that rely upon them in Setup.cpp,CSetup::OnLbnSelchangeDeviceList() and CSetup::OnInitDialog()!!!!
	//
	/////////////////////////////////////////////////////
	epiphan_deviceID_list.resize(2, _T("0"));
	epiphan_deviceID_list[1] = _T("1");

	device_list.resize(7, _T(""));
	device_list[0] = _T("Aptina DevWare");
	device_list[1] = _T("Omnivision OVTA");
	device_list[2] = _T("Toshiba ImaTuning");
	device_list[3] = _T("STM Conduit");
	device_list[4] = _T("Graphin EasyLab");
	device_list[5] = _T("Epiphan");
	device_list[6] = _T("Load file");

	bayer_list.resize(4, _T("Red in R1C1 (RG/GB)"));
	bayer_list[1] = _T("Red in R1C2 (GR/BG)");
	bayer_list[2] = _T("Red in R2C1 (GB/RG)");
	bayer_list[3] = _T("Red in R2C2 (BG/GR)");

	allowed_bits_per_pixel.resize(1, 8); // only 1-byte pixels for now
										 //allowed_bits_per_pixel.resize(3,8);
										 //allowed_bits_per_pixel[1] = 10;
										 //allowed_bits_per_pixel[2] = 12;

	deviceInfos.clear();
	video_format = _T("");

	op_console_ini_file = getImatestINIFolderPath() + "\\" + "OperatorConsole.INI";
	ini_file = INI_FILENAME;
}

void setup_settings::addStringWriteRequest(mwArray& writeKeys, mwSize rowIndex, const char* section, const char* subsection, const char* key, const char* value) noexcept
{
	mwSize getIndex = 1;
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
#ifdef INI_INCLUDE_SUBSECTION
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(value));
}

void setup_settings::addIntWriteRequest(mwArray & writeKeys, mwSize rowIndex, const char * section, const char * subsection, const char * key, int value) noexcept
{
	mwSize getIndex = 1;
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
#ifdef INI_INCLUDE_SUBSECTION
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(value));
}

void setup_settings::addDoubleWriteRequest(mwArray & writeKeys, mwSize rowIndex, const char * section, const char * subsection, const char * key, double value) noexcept
{
	mwSize getIndex = 1;
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
#ifdef INI_INCLUDE_SUBSECTION
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	writeKeys.Get(2, rowIndex, getIndex++).Set(mwArray(value));
}

void setup_settings::writeOpConsoleKeys() const
{
#ifdef INI_INCLUDE_SECTION
	const mwSize numWriteColumns = 4;
#else
	const mwSize numWriteColumns = 3;
#endif
	mwArray vararginParam = mwArray(1, 4, mxCELL_CLASS);
	mwArray writeKeys = mwArray(4, numWriteColumns, mxCELL_CLASS);
	mwArray inifilename((const char*)op_console_ini_file);
	mwArray mode("write");
	mwArray style("plain");
	static const char* subsection_blank = "";

	addStringWriteRequest(writeKeys, 1, IMATEST_INI_SECTION, subsection_blank, INI_FILE_PATH_KEY, (const char*)ini_file);
	addStringWriteRequest(writeKeys, 2, OP_CONSOLE_SECTION, subsection_blank, PART_NUMBER_KEY, (const char*)part_number);
	addStringWriteRequest(writeKeys, 3, OP_CONSOLE_SECTION, subsection_blank, SERIAL_NUMBER_KEY, (const char*)serial_number);
	addStringWriteRequest(writeKeys, 4, OP_CONSOLE_SECTION, subsection_blank, DEVICE_NAME_KEY, (const char*)device_name);

	vararginParam.Get(2, 1, 1).Set(inifilename);
	vararginParam.Get(2, 1, 2).Set(mode);
	vararginParam.Get(2, 1, 3).Set(writeKeys);
	vararginParam.Get(2, 1, 4).Set(style);
	try
	{
		inifile(vararginParam);
	}
	catch (mwException& e)
	{
		std::cout << "Run Error!" << std::endl;
		std::cerr << e.what() << std::endl;
		e.print_stack_trace();
	}
	
}

void setup_settings::writeImatestKeys() const
{
	mwArray vararginParam = mwArray(1, 4, mxCELL_CLASS);
	mwArray writeKeys = mwArray(8, 4, mxCELL_CLASS);
	mwArray inifilename(ini_file);
	mwArray mode("write"), style("plain");
	static const char* subsection_blank = "";
	static const char* subsection_current = "current";
	std::string subsection;
	std::string section;


	if (sourceID == SOURCE_Omnivision) // Omnivision
	{
		section = OMNIVISION_SECTION;
		subsection = subsection_current;
	}
	else
	{
		section = OP_CONSOLE_SECTION;
		subsection = subsection_blank;
	}
	
	// NOTE: the mwArray::Get function has input syntax Get(number of indexes, i1, i2,...in)
	// first read the 'acquire' key from [imatest]
	addIntWriteRequest(writeKeys, 1, IMATEST_INI_SECTION, subsection_blank, ACQUIRE_KEY, sourceID);
	
	// to write the Epiphan 'device_ID' key 
	addIntWriteRequest(writeKeys, 2, section.c_str(), subsection.c_str(), DEVICE_ID_KEY, epiphan_deviceID);

	// to write the 'width' key 
	addIntWriteRequest(writeKeys, 3, section.c_str(), subsection.c_str(), WIDTH_KEY, width);

	// to write the 'height' key 
	addIntWriteRequest(writeKeys, 4, section.c_str(), subsection.c_str(), HEIGHT_KEY, height);

	// to write the 'bitdepth' key 
	addIntWriteRequest(writeKeys, 5, section.c_str(), subsection.c_str(), BITDEPTH_KEY, bits_per_pixel);

	// to write the 'bayer_pattern' 
	addIntWriteRequest(writeKeys, 6, section.c_str(), subsection.c_str(), BAYER_PATTERN_KEY, bayer);

	// to write the 'register_files' 
	addStringWriteRequest(writeKeys, 7, section.c_str(), subsection.c_str(), OMNI_REGISTER_FILE_KEY, (const char*)omnivision_reg_file);

	// write the 'vid_format' key from [imatest]
	addStringWriteRequest(writeKeys, 8, IMATEST_INI_SECTION, subsection_blank, VIDEO_FORMAT_KEY, (const char*)video_format);

	vararginParam.Get(2, 1, 1).Set(inifilename);
	vararginParam.Get(2, 1, 2).Set(mode);
	vararginParam.Get(2, 1, 3).Set(writeKeys);
	vararginParam.Get(2, 1, 4).Set(style);
	try
	{
		inifile(vararginParam);
	}
	catch (mwException& e)
	{
		std::cout << "Run Error!" << std::endl;
		std::cerr << e.what() << std::endl;
		e.print_stack_trace();
	}

	writeOpConsoleKeys();
	
}

void setup_settings::addStringReadRequest(mwArray& readKeys, mwSize rowIndex, const char* section, const char* subsection, const char* key, const char* default_value) noexcept
{
	mwSize getIndex = 1;
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
#ifdef INI_INCLUDE_SUBSECTION
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray("")); // Indicates a string entry in the INI
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(default_value));
}

void setup_settings::addIntReadRequest(mwArray & readKeys, mwSize rowIndex, const char * section, const char * subsection, const char * key, int default_value) noexcept
{
	mwSize getIndex = 1;
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
#ifdef INI_INCLUDE_SUBSECTION
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray("i")); // Indicates an integer entry in the INI
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(default_value));
}

void setup_settings::addDoubleReadRequest(mwArray & readKeys, mwSize rowIndex, const char * section, const char * subsection, const char * key, double default_value) noexcept
{
	mwSize getIndex = 1;
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
#ifdef INI_INCLUDE_SUBSECTION
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray("d")); // Indicates an integer entry in the INI
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(default_value));
}

void setup_settings::readOpConsoleKeys()
{
#ifdef INI_INCLUDE_SECTION
	const mwSize numReadColumns = 5;
#else
	const mwSize numReadColumns = 4;
#endif

	mwArray vararginParam = mwArray(1, 1, mxCELL_CLASS);
	mwArray readKeys = mwArray(4, numReadColumns, mxCELL_CLASS);
	mwArray inifilename((const char*)op_console_ini_file);
	mwArray mode("read");
	static const char*  subsection_blank = "";
	const mwSize iniIndex = 1;
	const mwSize serialNumIndex = iniIndex + 1;
	const mwSize partNumIndex = serialNumIndex + 1;
	const mwSize deviceNameIndex = partNumIndex + 1;
	
	addStringReadRequest(readKeys, iniIndex, IMATEST_INI_SECTION, subsection_blank, INI_FILE_PATH_KEY, ini_file);
	addStringReadRequest(readKeys, serialNumIndex, OP_CONSOLE_SECTION, subsection_blank, SERIAL_NUMBER_KEY, serial_number);
	addStringReadRequest(readKeys, partNumIndex, OP_CONSOLE_SECTION, subsection_blank, PART_NUMBER_KEY, part_number);
	addStringReadRequest(readKeys, deviceNameIndex, OP_CONSOLE_SECTION, subsection_blank, DEVICE_NAME_KEY, device_name);
	
	vararginParam.Get(1, 1).Set(readKeys);
	mwArray readSett = mwArray(1, 4, mxCELL_CLASS);;
	try
	{
		inifile(1, readSett, inifilename, mode,vararginParam);

		ini_file = readSett.Get(1,1).Get(1, iniIndex).ToString();
		serial_number = readSett.Get(1, 1).Get(1, serialNumIndex).ToString();
		part_number = readSett.Get(1, 1).Get(1, partNumIndex).ToString();
		device_name = readSett.Get(1, 1).Get(1, deviceNameIndex).ToString();

	}
	catch (mwException& e)
	{
		std::cout << "Run Error!" << std::endl;
		std::cerr << e.what() << std::endl;
		std::string x = e.what();
		std::cout << x << std::endl;
		e.print_stack_trace();
	}


}

void setup_settings::readImatestKeys()
{
#ifdef INI_INCLUDE_SECTION
	const mwSize numReadColumns = 5;
#else
	const mwSize numReadColumns = 4;
#endif

	readOpConsoleKeys();

	bool result = false;
	mwArray vararginParam = mwArray(1, 3, mxCELL_CLASS);
	mwArray readKeys = mwArray(1, 5, mxCELL_CLASS);
	mwArray inifilename((const char*)ini_file);
	mwArray mode("read");
	static const char* subsection_blank = "";
	std::string section;
	std::string subsection;
	static const char* subsection_current = "current";

	// NOTE: the mwArray::Get function has input syntax Get(number of indexes, i1, i2,...in)

	// first read the 'acquire' key from [imatest]
	addIntReadRequest(readKeys, 1, IMATEST_INI_SECTION, subsection_blank, ACQUIRE_KEY, 0);

	vararginParam.Get(2, 1, 1).Set(inifilename);
	vararginParam.Get(2, 1, 2).Set(mode);
	vararginParam.Get(2, 1, 3).Set(readKeys);
	mwArray readSett = mwArray(1, 1, mxCELL_CLASS);
	int temp_source_id = sourceID;
	try
	{
		inifile(1, readSett, vararginParam);
		temp_source_id = static_cast<int>(readSett.Get(1, 1).Get(1, 1).Get(1, 1));
		sourceID = temp_source_id;

	}
	catch (mwException& e)
	{
		std::cout << "Run Error!" << std::endl;
		std::cerr << e.what() << std::endl;
		const char* x = e.what();
		e.print_stack_trace();
	}

	if (2 == sourceID )
	{
		section = OMNIVISION_SECTION;
		subsection = subsection_current;
	}
	else
	{
		section = OP_CONSOLE_SECTION;
		subsection = subsection_blank;
	}

	readKeys = mwArray(7, numReadColumns, mxCELL_CLASS);
	
	// to read the Epiphan 'device_ID' key 
	const mwSize epiphanIDIndex = 1;
	addIntReadRequest(readKeys, epiphanIDIndex, section.c_str(), subsection.c_str(), DEVICE_ID_KEY, 0);
	
	// to read the 'width' key 
	const mwSize widthIndex = 2;
	addIntReadRequest(readKeys, widthIndex, section.c_str(), subsection.c_str(), WIDTH_KEY, 0);

	// to read the 'height' key 
	const mwSize heightIndex = 3;
	addIntReadRequest(readKeys, heightIndex, section.c_str(), subsection.c_str(), HEIGHT_KEY, 0);

	// to read the 'bitdepth' key 
	const mwSize bitdepthIndex = 4;
	addIntReadRequest(readKeys, bitdepthIndex, section.c_str(), subsection.c_str(), BITDEPTH_KEY, 0);

	// to read the 'bayer_pattern' key
	const mwSize bayerIndex = 5;
	addIntReadRequest(readKeys, bayerIndex, section.c_str(), subsection.c_str(), BAYER_PATTERN_KEY, 0);

	// to read the 'register_files' key
	const mwSize registerIndex = 6;
	addStringReadRequest(readKeys, registerIndex, section.c_str(), subsection.c_str(), OMNI_REGISTER_FILE_KEY, (const char*)omnivision_reg_file);

	// to read the 'vid_format' key 
	const mwSize videoFormatIndex = 7;
	addStringReadRequest(readKeys, videoFormatIndex, section.c_str(), subsection.c_str(), VIDEO_FORMAT_KEY, (const char*)video_format);

	vararginParam.Get(1, 1).Set(inifilename);
	vararginParam.Get(1, 2).Set(mode);
	vararginParam.Get(1, 3).Set(readKeys);

	readSett = mwArray(1, 7, mxCELL_CLASS);
	int temp_epiphan_deviceid = epiphan_deviceID;
	int temp_width = width;
	int temp_height = height;
	int temp_bits_per_pixel = bits_per_pixel;
	int temp_bayer = bayer;
	CString temp_reg_file = omnivision_reg_file;
	CString temp_vid_format = video_format;
	try
	{
		inifile(1, readSett, vararginParam);
		mwArray settings = readSett.Get(1, 1);
		temp_epiphan_deviceid = settings.Get(1, epiphanIDIndex).Get(1, 1);
		temp_width = settings.Get(1, widthIndex).Get(1, 1);
		temp_height = settings.Get(1, heightIndex).Get(1, 1);
		temp_bits_per_pixel = settings.Get(1, bitdepthIndex).Get(1, 1);
		temp_bayer = settings.Get(1, bayerIndex).Get(1, 1);
		temp_reg_file = settings.Get(1, registerIndex).ToString();
		temp_vid_format = settings.Get(1, videoFormatIndex).ToString();

		// copy the values into the corresponding fields in setup_settings
		epiphan_deviceID = temp_epiphan_deviceid;
		width = temp_width;
		height = temp_height;
		bits_per_pixel = temp_bits_per_pixel;
		bayer = temp_bayer;
		omnivision_reg_file = temp_reg_file;
		video_format = temp_vid_format;

	}
	catch (mwException& e)
	{
		std::cout << "Run Error!" << std::endl;
		std::cerr << e.what() << std::endl;
		e.print_stack_trace();

	}

	omnivision_reg_file.Remove('\n'); // inifile() reads newline and carriage return characters into the string
	omnivision_reg_file.Remove('\r'); // which causes the setup window to be unable to find the file. These characters must be removed.

	if (omnivision_reg_file.Compare("[]") == 0) // Matlab returns empty strings slightly differently when cells are involved...
	{
		omnivision_reg_file = "";
	}

}


CString setup_settings::getImatestINIFolderPath()
{
	PWSTR pAppDataPath = nullptr;
	CString iniFolderPath;

	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &pAppDataPath)))
	{
		std::wstring  w_iniPath = pAppDataPath;
		w_iniPath += L"\\Imatest";
	
		std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> converter;
		iniFolderPath = converter.to_bytes(w_iniPath).c_str();
	}

	if (nullptr != pAppDataPath) {
		CoTaskMemFree(pAppDataPath);
	}

	return iniFolderPath;
}


