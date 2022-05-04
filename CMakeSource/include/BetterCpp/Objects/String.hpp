/*
 * Object.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: root
 */

#pragma once

#include "../stdafx.hpp"

NSP_BETTERCPP_BEGIN

template<typename T>
struct string_convert;

template<typename T>
struct string_convert {
	static std::wstring value(T o) {
		return std::to_wstring(o);
	}
};

better_class(String) {
private:

wchar_t* data;

public:
	~String();
	String();
	String(const_ref(String) other);
	const String& operator=(const_ref(String) other);

	String(const char_cptr utf8_str);
	String(const wchar_cptr c_wstr);

	String(const std::string utf8_str);
	String(const std::wstring wstr);

	String& append(String other);

	template<typename T>
	String& operator+=(T other){
		append(toString<T>(other));
		return *this;
	}

	String operator+(String other) {
		String copy = *this;
		return copy.append(other);
	}

	static String fromUtf8(const char_cptr c_str);

	std::string toUtf8() const;
	const wchar_cptr toWide() const;


	template<typename T>
	static String toString(T val) {
		if(is_base<IEnumerableT<T>, T>::value)
			return IEnumerableT<T>::toString((const IEnumerableT<T>&)val);

		if(is_base<IEnumerable, T>::value)
			return IEnumerable::toString((const IEnumerable&)val);

		return typeid(T).name();
	}

	wchar_t& operator[](uint pos);

private:
	static String __fmt(std::wstring ws, std::vector<std::wstring> args);

public:

	static String format(String fmt) {
		return fmt;
	}

	template<class... _Types>
	static String format(String s, const _Types&... args) {
		std::vector<std::wstring> vec { string_convert<_Types>::value(args)... };
		return __fmt(s.toWide(), vec);

		//return fmt::vformat(fmt::to_string_view(std::wstring(s.toWide())), fmt::make_format_args<fmt::buffer_context<wchar_t>>(convert<_Types>::value(args)...));
	}

	/*template<> String toString<const_ref(Object)>(const_ref(Object) val) { return val.type().info.name(); }

	template<> String toString<const char*>(const char* val) { return val; }
	template<> String toString<const wchar_t*>(const wchar_t* val) { return val; }

	template<> String toString<std::string>(std::string val) { return val; }
	template<> String toString<std::wstring>(std::wstring val) { return val; }

	template<> String toString<String>(String val) { return val; }

	template<> String toString<float>(float val) { return std::to_wstring(val).c_str(); }
	template<> String toString<double>(double val) { return std::to_wstring(val).c_str(); }
	template<> String toString<long double>(long double val) { return std::to_wstring(val).c_str(); }

	template<> String toString<char>(char val) { return std::to_wstring(val).c_str(); }
	template<> String toString<short>(short val) { return std::to_wstring(val).c_str(); }
	template<> String toString<int>(int val) { return std::to_wstring(val).c_str(); }
	template<> String toString<long>(long val) { return std::to_wstring(val).c_str(); }
	template<> String toString<long long>(long long val) { return std::to_wstring(val).c_str(); }

	template<> String toString<unsigned char>(unsigned char val) { return std::to_wstring(val).c_str(); }
	template<> String toString<unsigned short>(unsigned short val) { return std::to_wstring(val).c_str(); }
	template<> String toString<unsigned int>(unsigned int val) { return std::to_wstring(val).c_str(); }
	template<> String toString<unsigned long>(unsigned long val) { return std::to_wstring(val).c_str(); }
	template<> String toString<unsigned long long>(unsigned long long val) { return std::to_wstring(val).c_str(); }*/
};

template<typename T>
String IEnumerableT<T>::toString(const_ref(IEnumerableT<T>) collection) {
	auto en = collection.getEnumeratorT();
	bool empty = true;
	String str = "{ ";
	while(en->next()) {
		if(!empty)
			str += ", ";
		const T& o = en->currentT();
		str += String::toString<T>(o);
		empty = false;
	}
	if(empty)
		return "{ empty }";
	str += " }";
	return str;
}

template<typename T>
String operator+(String s, T o) {
	return s + String::toString(o);
}

template<typename T>
String operator+(T o, String s) {
	return String::toString(o) + s;
}

template<> String String::toString<const_ref(Object)>(const_ref(Object) val);

template<> String String::toString<const char*>(const char* val);
template<> String String::toString<const wchar_t*>(const wchar_t* val);

template<> String String::toString<std::string>(std::string val);
template<> String String::toString<std::wstring>(std::wstring val);

template<> String String::toString<float>(float val);
template<> String String::toString<double>(double val);
template<> String String::toString<long double>(long double val);

template<> String String::toString<char>(char val);
template<> String String::toString<short>(short val);
template<> String String::toString<int>(int val);
template<> String String::toString<long>(long val);
template<> String String::toString<long long>(long long val);

template<> String String::toString<unsigned char>(unsigned char val);
template<> String String::toString<unsigned short>(unsigned short val);
template<> String String::toString<unsigned int>(unsigned int val);
template<> String String::toString<unsigned long>(unsigned long val);
template<> String String::toString<unsigned long long>(unsigned long long val);

template<>
struct string_convert<String> {
	static std::wstring value(String o) {
		std::wstring wstr = o.toWide();
		return wstr;
	}
};

NSP_BETTERCPP_END
