#include "JSONDebug.h"
#ifdef JSON_DEBUG

#ifdef JSON_STDERROR
    #include <iostream>  //need std::cerr
#else
    #include "JSONSingleton.h"
    //otherwise, use a callback to tell the end user what happened
    json_error_callback_t JSONDebug::register_callback(json_error_callback_t callback) json_nothrow {
		json_error_callback_t res = JSONSingleton<json_error_callback_t>::get();
        JSONSingleton<json_error_callback_t>::set(callback);
		return res;
    }
#endif

//Something went wrong or an assert failed
void JSONDebug::_JSON_FAIL(const json_string & msg) json_nothrow {
    #ifdef JSON_STDERROR  //no callback, just use stderror
	   #ifndef JSON_UNICODE
		  std::cerr << msg << std::endl;
	   #else
		  std::cerr << std::string(msg.begin(), msg.end()) << std::endl;
	   #endif
    #else
	   if (json_error_callback_t ErrorCallback = JSONSingleton<json_error_callback_t>::get()){  //only do anything if the callback is registered
		  #ifdef JSON_LIBRARY
			 ErrorCallback(msg.c_str());
		  #else
			 ErrorCallback(msg);
		  #endif
	   }
    #endif
}

//asserts that condition is true, more useful than cassert because it lets you keep going
void JSONDebug::_JSON_ASSERT(bool condition, const json_string & msg) json_nothrow {
    if (json_unlikely(!condition)){
	   _JSON_FAIL(msg);
    }
}
#endif

#ifdef JSON_UNIT_TEST
static std::map<std::string, libjson_CodeCoverage *> _libjson_CodeCoverageMapping;
libjson_CodeCoverage::libjson_CodeCoverage(const std::string & desc, size_t branches) : _branches(branches), _desc(desc) {
	_libjson_CodeCoverageMapping[_desc] = this;
}

libjson_CodeCoverage::~libjson_CodeCoverage(void){
	std::map<std::string, libjson_CodeCoverage *>::iterator pos = _libjson_CodeCoverageMapping.find(_desc);
	if (pos == _libjson_CodeCoverageMapping.end()) return;
	_libjson_CodeCoverageMapping.erase(pos);
}

libjson_CodeCoverage * libjson_CodeCoverage::getByName(const std::string & desc){
	std::map<std::string, libjson_CodeCoverage *>::iterator pos = _libjson_CodeCoverageMapping.find(desc);
	if (pos == _libjson_CodeCoverageMapping.end()) return 0;
	return (*pos).second;
}
#endif
