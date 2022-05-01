#pragma once

#include "Exception.hpp"

#define THROW_FILE_NOT_FOUND_EXCEPTION(file_name) throw ::NSP_BETTERCPP::FileNotFoundException(file_name, __LINE__, __FILE__)

NSP_BETTERCPP_BEGIN

class FileNotFoundException : public Exception {
public:
	FileNotFoundException(String file_name, int line_, const char* file_);
};

NSP_BETTERCPP_END
