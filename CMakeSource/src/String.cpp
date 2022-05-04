/*
 * String.cpp
 *
 *  Created on: Apr 30, 2022
 *      Author: filebin
 */

#include "pch.hpp"

#include "fmt/format.h"
#include "fmt/xchar.h"
#include "fmt/args.h"

NSP_BETTERCPP_BEGIN

String::~String() { delete data; }

String::String() { data = new wchar_t[1]; data[0] = 0; }

String::String(const_ref(String) other) {
	data = new wchar_t[wcslen(other.data) + 1];
	wcscpy(data, other.data);
}

const String& String::operator=(const_ref(String) other) {
	data = (wchar_t*)realloc(data, sizeof(wchar_t)*(wcslen(other.data) + 1));
	wcscpy(data, other.data);
	return *this;
}

String::String(const char_cptr str) {
	#ifdef _MSC_VER
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), NULL, 0);
    WCHAR* data = (WCHAR*)malloc(size_needed);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)strlen(str), data, size_needed);
	#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	auto ws = utf8_conv.from_bytes(str);
	const wchar_t* src = ws.c_str();
	data = new wchar_t[wcslen(src) + 1];
	wcscpy(data, src);
	#endif
}
String::String(const wchar_cptr src) {
	data = new wchar_t[wcslen(src) + 1];
	wcscpy(data, src);
}

String::String(const std::string str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	auto ws = utf8_conv.from_bytes(str.c_str());
	const wchar_t* src = ws.c_str();
	data = new wchar_t[wcslen(src) + 1];
	wcscpy(data, src);
}
String::String(const std::wstring ws){
	const wchar_t* src = ws.c_str();
	data = new wchar_t[wcslen(src) + 1];
	wcscpy(data, src);
}

String& String::append(String other) {
	uint size = wcslen(data) + wcslen(other.data) + 1;
	data = (wchar_ptr)realloc(data, size * sizeof(wchar_t));
	wcscat(data, other.data);
	return *this;
}

String String::__fmt(std::wstring ws, std::vector<std::wstring> args) {
	using ctx = fmt::buffer_context<wchar_t>;

	fmt::dynamic_format_arg_store<ctx> ds;
	uint n = args.size();
	for(uint i=0;i<n;++i) {
		ds.push_back(args[i]);
	}
	/*typedef fmt::basic_format_arg<ctx> basic_arg;
	uint n = args.size();
	basic_arg* fmt_args = (basic_arg*)new byte[sizeof(basic_arg)*n];
	basic_arg* it = fmt_args;
	for(uint i=0;i<n;++i) {
		new(it++) basic_arg(fmt::make_arg<ctx>(args[i]));
	}*/

	return fmt::vformat(fmt::to_string_view<wchar_t>(ws), ds);
			//fmt::basic_format_args<ctx>(fmt_args, n));
}

String String::fromUtf8(const char_cptr c_str) { return c_str; }

std::string String::toUtf8() const {
	#ifdef _MSC_VER
	wchar_t* wstr = data;
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wcslen(wstr), NULL, 0, NULL, NULL);
    char* strTo = (char*)malloc(size_needed);
    WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wcslen(wstr), strTo, size_needed, NULL, NULL);
    return strTo;
	#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	return utf8_conv.to_bytes(data);
	#endif
}
const wchar_cptr String::toWide() const {
	return data;
}

wchar_t& String::operator[](uint pos) { return data[pos]; }

template<> String String::toString<const_ref(Object)>(const_ref(Object) val) { return val.type().info.name(); }

template<> String String::toString<const char*>(const char* val) { return val; }
template<> String String::toString<const wchar_t*>(const wchar_t* val) { return val; }

template<> String String::toString<std::string>(std::string val) { return val; }
template<> String String::toString<std::wstring>(std::wstring val) { return val; }

template<> String String::toString<float>(float val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<double>(double val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<long double>(long double val) { return std::to_wstring(val).c_str(); }

template<> String String::toString<char>(char val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<short>(short val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<int>(int val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<long>(long val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<long long>(long long val) { return std::to_wstring(val).c_str(); }

template<> String String::toString<unsigned char>(unsigned char val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<unsigned short>(unsigned short val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<unsigned int>(unsigned int val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<unsigned long>(unsigned long val) { return std::to_wstring(val).c_str(); }
template<> String String::toString<unsigned long long>(unsigned long long val) { return std::to_wstring(val).c_str(); }

NSP_BETTERCPP_END




