/*
 * Core.cpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */
#include "stdafx.hpp"

#ifndef INCLUDE_BETTERCPP_OBJECTS_CORE_HPP_
#define INCLUDE_BETTERCPP_OBJECTS_CORE_HPP_

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
template<typename T>
better_class_proto(AutoPtr);

template<typename T>
better_class_proto(RefPtr);

struct RootType {};

struct Interface : public RootType {};

struct Type : public RootType {
	Type(std::type_index info, uint size) : info(info), size(size) {}
	std::type_index info;
	uint size;
};

class Object : public RootType {
protected:
	Object() = default;
public:

	AutoPtr<Object> clone() const;
	RefPtr<Object> cloneNew() const;
	virtual Object* cloneNewUnsafe() const = 0;
	virtual const Type type() const = 0;
	virtual ~Object() {}

	static ulong hash(const_ref(Object) obj);
};

NSP_BETTERCPP_END

#include "object_cloner.hpp"

#endif /* INCLUDE_BETTERCPP_OBJECTS_CORE_HPP_ */



#ifdef INCLUDE_BETTERCPP_OBJECTS_POINTERS_HPP_

#ifndef INCLUDE_BETTERCPP_OBJECTS_CORE_OBJ_CRTP_HPP_
#define INCLUDE_BETTERCPP_OBJECTS_CORE_OBJ_CRTP_HPP_
NSP_BETTERCPP_BEGIN

template<typename Derived>
class Object_CRTP : public Object {
protected:
	Object_CRTP() = default;
public:
	Object* cloneNewUnsafe() const override {
		return new Derived(dynamic_cast<Derived const&>(*this));
	}

	const Type type() const override {
		return Type(typeid(*this), sizeof(*this));
	}
};

NSP_BETTERCPP_END
#endif /* INCLUDE_BETTERCPP_OBJECTS_CORE_OBJ_CRTP_HPP_ */
#endif /*INCLUDE_BETTERCPP_OBJECTS_POINTERS_HPP_*/

