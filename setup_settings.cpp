#include "setup_settings.h"
#include <locale>
#include "imatest_library.h"
#include "logger.h"
#include "logger_preferences.h"
#include "op_console_preferences.h"

// This CLogger and its associated handler is meant for logging details about the Operator Console's operations, not Imatest IT per se.
static CLogger logger("SetupSettings");

static CFileLogHandler fileLogHandler(LOGGER_LEVEL, "setup_settings"); // A file extension is not necessary; *.log is automatically appended.


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

void setup_settings::writeOpConsoleKeys()
{
#ifdef INI_INCLUDE_SECTION
	const mwSize numWriteColumns = 4;
#else
	const mwSize numWriteColumns = 3;
#endif
	mwArray vararginParam = mwArray(1, 2, mxCELL_CLASS);
	mwArray writeKeys = mwArray(4, numWriteColumns, mxCELL_CLASS);
	mwArray inifilename((const char*)op_console_ini_file);
	mwArray mode("write");
	mwArray style("plain");
	static const char* subsection_blank = "";



	addStringWriteRequest(writeKeys, 1, IMATEST_INI_SECTION, subsection_blank, INI_FILE_PATH_KEY, (const char*)ini_file);
	addStringWriteRequest(writeKeys, 2, OP_CONSOLE_SECTION, subsection_blank, PART_NUMBER_KEY, (const char*)part_number);
	addStringWriteRequest(writeKeys, 3, OP_CONSOLE_SECTION, subsection_blank, SERIAL_NUMBER_KEY, (const char*)serial_number);
	addStringWriteRequest(writeKeys, 4, OP_CONSOLE_SECTION, subsection_blank, DEVICE_NAME_KEY, (const char*)device_name);


	vararginParam.Get(2, 1, 1).Set(writeKeys);
	vararginParam.Get(2, 1, 2).Set(style);
	try
	{
		inifile(inifilename, mode, vararginParam);
	}
	catch (mwException& e)
	{
		std::cout << "Run Error!" << std::endl;
		std::cerr << e.what() << std::endl;
		e.print_stack_trace();
	}
	
}

void setup_settings::addStringReadRequest(mwArray& readKeys, mwSize rowIndex, const char* section, const char* subsection, const char* key, const char* default_value) noexcept
{
	mwSize getIndex = 1;
	mwArray temp;
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(section));
	
#ifdef INI_INCLUDE_SUBSECTION
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(subsection));
#endif
	
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(key));
	
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray("")); // Indicates a string entry in the INI
	
	readKeys.Get(2, rowIndex, getIndex++).Set(mwArray(default_value));
}

void setup_settings::readOpConsoleKeys()
{
#ifdef INI_INCLUDE_SECTION
	const mwSize numReadColumns = 5;
#else
	const mwSize numReadColumns = 4;
#endif
	logger.AddHandler(&fileLogHandler);
	mwArray vararginParam = mwArray(1, 1, mxCELL_CLASS);
	mwArray readKeys = mwArray(4, numReadColumns, mxCELL_CLASS);
	mwArray inifilename((const char*)op_console_ini_file);
	mwArray mode("read");
	static const char*  subsection_blank = "";
	mwSize iniIndex = 1;
	mwSize serialNumIndex = iniIndex + 1;
	mwSize partNumIndex = serialNumIndex + 1;
	mwSize deviceNameIndex = partNumIndex + 1;
	
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


