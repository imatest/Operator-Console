#ifndef JSON_DEBUG_H
#define JSON_DEBUG_H

#include "JSONDefs.h"

#ifdef JSON_DEBUG
    #ifdef JSON_SAFE
	   #define JSON_ASSERT_SAFE(condition, msg, code)\
		  {\
			 if (json_unlikely(!(condition))){\
				JSON_FAIL(msg);\
				code\
			 }\
		  }
	   #define JSON_FAIL_SAFE(msg, code)\
		  {\
			 JSON_FAIL(msg);\
			 code\
		  }
    #else
	   #define JSON_ASSERT_SAFE(condition, msg, code) JSON_ASSERT(condition, msg)
	   #define JSON_FAIL_SAFE(msg, code) JSON_FAIL(msg)
    #endif

    #define JSON_FAIL(msg) JSONDebug::_JSON_FAIL(msg)
    #define JSON_ASSERT(bo, msg) JSONDebug::_JSON_ASSERT(bo, msg)

    class JSONDebug {
    public:
	   #ifndef JSON_STDERROR
		  static json_error_callback_t register_callback(json_error_callback_t callback) json_nothrow json_cold;
	   #endif
	   static void _JSON_FAIL(const json_string & msg) json_nothrow json_cold;
	   static void _JSON_ASSERT(bool condition, const json_string & msg) json_nothrow json_cold;
    };
#else
    #ifdef JSON_SAFE
	   #define JSON_ASSERT_SAFE(condition, msg, code)\
		  {\
			 if (json_unlikely(!(condition))){\
				code\
			 }\
		  }
	   #define JSON_FAIL_SAFE(msg, code)\
		  {\
			 code\
		  }
    #else
	   #define JSON_ASSERT_SAFE(condition, msg, code)
	   #define JSON_FAIL_SAFE(msg, code)
    #endif

    #define JSON_ASSERT(condition, msg)
    #define JSON_FAIL(msg)
#endif

#ifdef JSON_UNIT_TEST
	#include <map>
	#include <sstream>
	#include <string>
	class libjson_CodeCoverage {
	public:
		libjson_CodeCoverage(const std::string & desc, size_t branches);
		~libjson_CodeCoverage(void);
		static void inc(libjson_CodeCoverage * cc, const std::string & _id){ 
			if (cc){ 
				std::map<std::string, size_t>::iterator pos = cc -> _ids.find(_id);
				if (pos == cc -> _ids.end()){
					cc -> _ids[_id] = 1; 
				} else {
					++(*pos).second;
				}
			} 
		}
		inline bool isComplete(){ return _ids.size() == _branches; }
		static libjson_CodeCoverage * getByName(const std::string & name);
		std::string printOut(void){
			std::stringstream output;
			#ifndef JSON_DEBUG
			if (!isComplete()){
			#endif
				output << "Hit these coverage markers: <table border=\"1\">";
				std::map<std::string, size_t>::iterator pos = _ids.begin();
				for(; pos != _ids.end(); ++pos){
					output << "<tr><td>" << (*pos).first << "</td>";
					output << "<td>" << (*pos).second << "</td></tr>";
				}
				output << "</table>";
				
			#ifndef JSON_DEBUG
			}
			#endif
			output << ((float)(_ids.size()) / (float)(_branches)) * 100.0 << "% complete";
			return output.str();
		}
	protected:
		libjson_CodeCoverage(const libjson_CodeCoverage & );
		size_t _branches;
		std::string _desc;
		std::map<std::string, size_t> _ids;
	};
	#define ScopeCoverageHeap(desc, branches)\
		coverage = new libjson_CodeCoverage(#desc, branches);\
		assertNotNull(GetScopeCoverage(desc))
	#define ScopeCoverage(desc, branches)\
		libjson_CodeCoverage desc(#desc, branches);\
		assertNotNull(GetScopeCoverage(desc))
	#define HitScopeCoverage(name, id)\
		libjson_CodeCoverage::inc(GetScopeCoverage(name), #id)
	#define GetScopeCoverage(name)\
		libjson_CodeCoverage::getByName(std::string( #name ))
	#define AssertScopeCoverage(desc)\
		assertTrue(desc.isComplete());\
		if (!desc.isComplete()){\
			FAIL(desc.printOut());\
		} else {\
			PASS(desc.printOut());\
		}
	#define AssertScopeCoverageHeap(desc)\
		assertTrue(coverage -> isComplete());\
		if (!coverage -> isComplete()){\
			FAIL(coverage -> printOut());\
		} else {\
			PASS(coverage -> printOut());\
		}\
		delete coverage;
#else 
	#define ScopeCoverageHeap(desc, branches) (void)0
	#define ScopeCoverage(desc, branches) (void)0
	#define HitScopeCoverage(name, id) (void)0
	#define GetScopeCoverage(name, activate) (void)0
	#define AssertScopeCoverage(desc) (void)0
	#define AssertScopeCoverageHeap(desc) (void)0
#endif

#endif

