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

String::String() { data = new wchar_t[1]; data[0] = 0; }

String::String(const_ref(String) other) {
	data = new wchar_t[wcslen(other.data) + 1];
	wcscpy(data, other.data);
}

String::String(const char_cptr str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	auto ws = utf8_conv.from_bytes(str);
	const wchar_t* src = ws.c_str();
	data = new wchar_t[wcslen(src) + 1];
	wcscpy(data, src);
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
	data = (wchar_ptr)realloc(data, wcslen(data) + wcslen(other.data) + 1);
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
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	return utf8_conv.to_bytes(data);
}
const wchar_cptr String::toWide() const {
	return data;
}

wchar_t& String::operator[](uint pos) { return data[pos]; }

NSP_BETTERCPP_END




