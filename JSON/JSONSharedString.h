#ifndef JSON_SHARED_STRING_H
#define JSON_SHARED_STRING_H

/*
 *	This class allows json objects to share string
 *	Since libjson is a parser, it does a lot of substrings, but since
 *	a string with all of the information already exists, those substrings
 *	can be infered by an offset and length and a pointer to the master
 *	string
 *
 *	EXPERIMENTAL, Not used yet
 */

#include "JSONDebug.h"
#include "JSONGlobals.h"

#ifdef JSON_LESS_MEMORY
	#ifdef __GNUC__
		#pragma pack(push, 1)
	#elif _MSC_VER
		#pragma pack(push, json_shared_string_pack, 1)
	#endif
#endif

class json_shared_string {
public:
	inline json_string::iterator begin(void){
		return _str -> mystring.begin() + offset;
	}
	inline json_string::iterator end(void){
		return begin() + len;
	}
	
	inline json_string::const_iterator begin(void) const{
		return _str -> mystring.begin() + offset;
	}
	inline json_string::const_iterator end(void) const{
		return begin() + len;
	}
	
	inline json_shared_string(void) : offset(0), len(0), _str(new json_shared_string_internal(json_global(EMPTY_JSON_STRING))) {}
	
	inline json_shared_string(const json_string & str) : offset(0), len(str.length()), _str(new json_shared_string_internal(str)) {}
	
	inline json_shared_string(const json_shared_string & str, size_t _offset, size_t _len) : _str(str._str), offset(str.offset + _offset), len(_len) {
		++_str -> refCount;
	}
	
	inline json_shared_string(const json_shared_string & str, size_t _offset) : _str(str._str), offset(str.offset + _offset), len(str.len - _offset) {
		++_str -> refCount;
	}
	
	inline ~json_shared_string(void){
		deref();
	}
	
	inline bool empty(void) const { return len == 0; }
	
	size_t find(json_char ch, size_t pos = 0) const {
		if (_str -> refCount == 1) return _str -> mystring.find(ch, pos);
		json_string::const_iterator e = end();
		for(json_string::const_iterator b = begin() + pos; b != e; ++b){
			if (*b == ch) return b - begin();
		}
		return json_string::npos;
	}
	
	inline json_char & operator[] (size_t loc){
		return _str -> mystring[loc + offset];
	}
	inline json_char operator[] (size_t loc) const {
		return _str -> mystring[loc + offset];
	}
	inline void clear(){ len = 0; }
	inline size_t length() const { return len; }
	inline const json_char * c_str() const { return toString().c_str(); }
	inline const json_char * data() const { return _str -> mystring.c_str() + offset; }
	
	inline bool operator != (const json_shared_string & other) const {
		if ((other._str == _str) && (other.len == len) && (other.offset == offset)) return false;
		return other.toString() != toString();
	}
	
	inline bool operator == (const json_shared_string & other) const {
		if ((other._str == _str) && (other.len == len) && (other.offset == offset)) return true;
		return other.toString() == toString();
	}
	
	inline bool operator == (const json_string & other) const {
		return other == toString();
	}
	
	json_string & toString(void) const {
		//gonna have to do a real substring now anyway, so do it completely
		if (_str -> refCount == 1){
			if (offset || len != _str -> mystring.length()){
				_str -> mystring = json_string(begin(), end());
			}
		} else if (offset || len != _str -> mystring.length()){
			--_str -> refCount;  //dont use deref because I know its not going to be deleted
			_str = new json_shared_string_internal(json_string(begin(), end()));
		}
		offset = 0;
		return _str -> mystring;
	}
	
	
	inline void assign(const json_shared_string & other, size_t _offset, size_t _len){
		if (other._str != _str){
			deref();
			_str = other._str;
		}
		++_str -> refCount;
		offset = other.offset + _offset;
		len = _len;
	}
	
	json_shared_string(const json_shared_string & other) : _str(other._str), offset(other.offset), len(other.len){
		++_str -> refCount;
	}
	
	json_shared_string & operator =(const json_shared_string & other){
		if (other._str != _str){
			deref();
			_str = other._str;
		}
		++_str -> refCount;
		offset = other.offset;
		len = other.len;
		return *this;
	}
JSON_PRIVATE
	struct json_shared_string_internal {
		inline json_shared_string_internal(const json_string & _mystring) : mystring(_mystring), refCount(1) {}
		json_string mystring;
		size_t refCount PACKED(20);
	};
	inline void deref(void){
		if (--_str -> refCount == 0){
			delete _str;
		}
	}
	mutable json_shared_string_internal * _str;
	mutable size_t offset PACKED(20);
	mutable size_t len PACKED(20);
};

#ifdef JSON_LESS_MEMORY
	#ifdef __GNUC__
		#pragma pack(pop)
	#elif _MSC_VER
		#pragma pack(pop, json_shared_string_pack,)
	#endif
#endif

#endif
