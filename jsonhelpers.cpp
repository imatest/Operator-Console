#pragma unmanaged
#include <iostream>
#include <exception>
#include <string>
#include "libjson.h"
using namespace std;

// For summing the 9 error pixel regions into a single integer
int sumArrayInt (JSONNode node, int count) {
    int sum=0;
    for (int i=0; i<count; ++i) {
        sum += node[i].as_int();
    }
    return sum;
}
int getIntFromArray(JSONNode node, char* name) {
  try {
	  return node.at(name).as_array()[0].as_int();

    } catch(exception& e) {
		cout << " Parse error" << e.what() << endl; 
    }
	return -65535;
}

//
// Concatenate all elements of a JSON string array into a single string, separated by commas
//
void getStringArray(JSONNode &array, int len, std::string *s)
{
	int	i;

	for  (i = 0; i < len; i++)
	{
		if (i > 0)
		{
			s->append(", ");
		}

		s->append(array[i].as_string());
	}
}

//
// Copy all elements of a JSON floating-point array into an array of doubles
// Note: data array must already have been allocated, and be the correct length
//
void getFloatArray(JSONNode &array, double *data, int len)
{
	int	i;

	if (data != NULL)
	{
		for  (i = 0; i < len; i++)
		{
			data[i] = array[i].as_float();
		}
	}
}


/********************************************
 **  Prepare test settings                 **
 ********************************************
 Construct a  JSON object that will communicate the settings for the test to Imatest IT
 */
void prepareTestSettings(int width, int height, int ncolors, const char* extension, const char* fileroot, 
						 const char* serial_number, const char* part_number, int crop_borders[4], json_string &test_settings) {
	// Construct JSON that Serial # and Model # info to the JSON options string
	
	JSONNode n(JSON_NODE);
	n.push_back(JSONNode("width", width));
	n.push_back(JSONNode("height", height));
	n.push_back(JSONNode("extension", extension));
	n.push_back(JSONNode("fileroot", fileroot));
	n.push_back(JSONNode("serial_number", serial_number));
	n.push_back(JSONNode("part_number", part_number));
	n.push_back(JSONNode("ncolors", ncolors));
	JSONNode cbn(JSON_ARRAY);
	cbn.set_name("crop_borders");
	cbn.push_back(JSONNode("",crop_borders[0]));
	cbn.push_back(JSONNode("",crop_borders[1]));
	cbn.push_back(JSONNode("",crop_borders[2]));
	cbn.push_back(JSONNode("",crop_borders[3]));
	n.push_back(cbn);
	n.set_name("jstr");
	JSONNode j(JSON_NODE);
	j.push_back(n);
	test_settings = j.write_formatted();
	// std::cout << "TEST SETTINGS:" << std::endl;
	// std::cout << test_settings << std::endl;
}