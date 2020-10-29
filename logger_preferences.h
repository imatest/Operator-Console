#pragma once
#ifdef _DEBUG
#define new DEBUG_NEW
#define LOGGER_LEVEL LEVEL_FINE
#else
#define LOGGER_LEVEL LEVEL_WARNING
#endif

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME "operator_console"
#endif
