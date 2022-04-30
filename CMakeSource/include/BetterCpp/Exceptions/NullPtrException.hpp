#pragma once

#include <BetterCpp/Exceptions/Exception.hpp>

#define THROW_NULL_PTR_EXCEPTION(pointer) throw ::NSP_BETTERCPP::NullPtrException(pointer, __LINE__, __FILE__)

NSP_BETTERCPP_BEGIN

better_class_extends(NullPtrException, Exception) {
	BETTER_OVERRIDE_EXCEPTION_METHODS
private:
	std::type_index type;
	void* ptr;
public:
	template<typename ptrT = void*>
	NullPtrException(ptrT whatPtr, int line_, const char* file_) : Exception("Pointer is null", line_, file_), type(typeid(whatPtr)) {
		ptr = (void*)whatPtr;
		String info = String::format(L"TypeInfo: {}\nPointer: {:#x}\n", String(getTypeInfo().name()), (ulong)getPointer());//fmt::format(L"TypeInfo: {}\nPointer: {:#x}\n", String(getTypeInfo().name()), (ulong)getPointer()); // @suppress("Invalid arguments")
		setInfo(info);
	}

	std::type_index getTypeInfo() { return type; }

	void* getPointer() { return ptr; }
};

NSP_BETTERCPP_END
