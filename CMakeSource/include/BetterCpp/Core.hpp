/*
 * Core.cpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#include "stdafx.hpp"

#pragma once

typedef char* char_ptr;
typedef char const* char_cptr;
typedef wchar_t* wchar_ptr;

typedef wchar_t const* wchar_cptr;

typedef int8_t sbyte;
typedef uint8_t byte;
typedef uint32_t uint;
typedef uint64_t ulong;


NSP_BETTERCPP_BEGIN
better_class_proto(String);

struct Interface {};



class Object {
public:
	virtual String toString() const;
	virtual ulong hash() const;
	virtual uint byteSize() const = 0;
	virtual ~Object() {}
};


NSP_BETTERCPP_END
