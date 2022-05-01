/*
 * object_cloner.hpp
 *
 *  Created on: May 1, 2022
 *      Author: filebin
 */

#ifndef INCLUDE_BETTERCPP_OBJECT_CLONER_HPP_
#define INCLUDE_BETTERCPP_OBJECT_CLONER_HPP_

#include "Core.hpp"
#include "Exceptions.hpp"

NSP_BETTERCPP_BEGIN

template <typename Base, typename Derived>
struct is_base {
private:
    constexpr static bool check(Base*)   { return true; }
    constexpr static bool check(...)     { return false; }
public:
    enum { value = check(static_cast<Derived*>(0)) };
};

template <typename T, bool is_object = is_base<Object, T>::value, bool is_abstact = std::is_abstract<T>::value>
struct object_cloner;


//cloner for the object types
template <typename T>
struct object_cloner<T, true, true> {
	static T* cloneNew(const_ref(T) o) {
		return as<Object>(o).cloneNewUnsafe();
	}
};

template <typename T>
struct object_cloner<T, true, false> {
	static T* cloneNew(const_ref(T) o) {
		return as<Object>(o).cloneNewUnsafe();
	}
};

//cloner for the abstract non object types
template <typename T>
struct object_cloner<T, false, true> {
	static T* cloneNew(const_ref(T) o) {
		const Object* object = dynamic_cast<const Object*>(&o);
		if(object) return (T*)object->cloneNewUnsafe();
		THROW_EXCEPTION((std::string(typeid(T).name()) + " is abstract!").c_str());
		return nullptr;
	}
};

//cloner for the not abstract non object types
template <typename T>
struct object_cloner<T, false, false> {
	static T* cloneNew(const_ref(T) o) {
		return new T(o);
	}
};

NSP_BETTERCPP_END


#endif /* INCLUDE_BETTERCPP_OBJECT_CLONER_HPP_ */
