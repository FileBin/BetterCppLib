#pragma once

#include "Exception.hpp"

#define THROW_INDEX_OUT_OF_RANGE_EXCEPTION throw ::NSP_BETTERCPP::IndexOutOfRangeException(__LINE__, __FILE__)

NSP_BETTERCPP_BEGIN

better_class_extends(IndexOutOfRangeException, Exception) {
public:
	IndexOutOfRangeException(int line_, const char* file_);
};

NSP_BETTERCPP_END
