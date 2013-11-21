#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H
#include "JSON\JSONDefs.h"

// JSON helper functions
int  sumArrayInt (JSONNode node, int count);
void getFloatArray(JSONNode &array, double *data, int len);
int  getIntFromArray(JSONNode node, char* name);
void getStringArray(JSONNode &array, int len, std::string *s);
void prepareTestSettings(int width, int height, int ncolors, const char* extension, const char* fileroot, 
						 const char* serial_number, const char* part_number, int crop_borders[4], json_string &settings);

#endif



