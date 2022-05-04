#pragma once

#include "../stdafx.hpp"

#include <exception>

#define THROW_EXCEPTION(wtf) throw ::NSP_BETTERCPP::Exception(wtf, __LINE__, __FILE__)

NSP_BETTERCPP_BEGIN

better_class_proto(String);

struct exception_data;

class Exception : public std::runtime_error {
private:
    int _line;
    const char* _file;
    exception_data* data;

protected:

    String getStack();

public:
    Exception(const char* msg, int line, const char* file);

    int line() const;
    String file() const;
    String stack() const;
    String info() const;
    String msg() const;

    virtual ~Exception();

protected:
    void setInfo(String info);
};

NSP_BETTERCPP_END
