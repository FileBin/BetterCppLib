#pragma once

#include "../stdafx.hpp"

#include "../Objects/String.hpp"

#include <exception>

#define THROW_EXCEPTION(wtf) throw ::NSP_BETTERCPP::Exception(wtf, __LINE__, __FILE__)
#define BETTER_OVERRIDE_EXCEPTION_METHODS public: String toString() const override { return what(); } BETTER_OVERRIDE_GET_FULL_NAME BETTER_OVERRIDE_BYTE_SIZE

NSP_BETTERCPP_BEGIN

better_class(Exception) better_implements(public std::runtime_error) {
	BETTER_OVERRIDE_EXCEPTION_METHODS;
private:
    int _line;
    String _file;
    String _stack;
    String _info;

    String getStack() {
#ifdef _MSC_VER
        HANDLE process = GetCurrentProcess();
        SymInitialize(process, NULL, TRUE);

        void* stack[64];
        unsigned short frames = CaptureStackBackTrace(0, 64, stack, NULL);
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        if (symbol == nullptr)
            throw std::runtime_error("MemoryError"); //Memory alloc excep
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        std::string strstack{};
        for (int i = 0; i < frames; i++) {
            SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
            IMAGEHLP_LINE64 line;
            ZeroMemory(&line, sizeof(line));
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD dis;
            SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &dis, &line);
            strstack += fmt::format("[{}]\t{} at line {}, address: {:#x}\n", frames - i - 1, symbol->Name, line.LineNumber, symbol->Address);//'\n' << frames - i - 1 << ':' << symbol->Name << " " << (void*)symbol->Address;
        }

        free(symbol);
        return strstack;
#else
        //TODO make get stack
        return "";
#endif
    }

public:
    Exception(const char* msg, int line, const char* file) : std::runtime_error(msg), _line(line), _file(file) {
        _stack = getStack();
    }

    int line() const { return _line; }
    String file() const { return _file; }
    String stack() const { return _stack; }
    String info() { return _info; }
    String msg() { return what(); }

protected:
    void setInfo(String info) {
    	_info = info;
    }
};

NSP_BETTERCPP_END
