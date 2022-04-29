/*
 * Object.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: root
 */

#pragma once

#include "../stdafx.hpp"
#include "../Core.hpp"

NSP_BETTERCPP_BEGIN

better_class(String) {
BETTER_OVERRIDE_CLASS_NAME(String)
BETTER_OVERRIDE_BYTE_SIZE;

String toString() { return *this; }
public:

	String(const char_cptr c_str);
	String(const wchar_cptr wc_str);

	static String fromUtf8(const char* c_str);
};

NSP_BETTERCPP_END
