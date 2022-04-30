#pragma once

#include "Exception.hpp"

#define THROW_FILE_NOT_FOUND_EXCEPTION(file_name) throw ::NSP_BETTERCPP::FileNotFoundException(file_name, __LINE__, __FILE__)

NSP_BETTERCPP_BEGIN

better_class_extends(FileNotFoundException,Exception) {
	BETTER_OVERRIDE_OBJECT_METHODS;
public:
	FileNotFoundException(String file_name, int line_, const char* file_) : Exception("File not found!", line_, file_) {
		String info{};
		info.append("File Name: ");
		info.append(file_name);
		info.append("\n");
		setInfo(info);
	}
};

NSP_BETTERCPP_END
