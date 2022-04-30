/*
 * Functions.hpp
 *
 *  Created on: Apr 30, 2022
 *      Author: filebin
 */

#pragma once

#include "stdafx.hpp"

NSP_BETTERCPP_BEGIN

template <typename Base, typename Derived>
struct is_base {
private:
    constexpr static bool check(Base*)   { return true; }
    constexpr static bool check(...)     { return false; }
public:
    enum { value = check(static_cast<Derived*>(0)) };
};

template<typename T, int size>
uint ArraySize(T(&)[size]){ return size; }

template<typename R, typename T>
constexpr void assert_base(){ static_assert(is_base<R, T>::value, "Compilation error: R isn't base of T!"); }

template<typename T> pRootType root(RefPtr<T> arg) {
assert_base<RootType, T>();
return *(pRootType*)&arg;
}

template<typename T, typename R>
const_ref(T) as(const_ref(R) arg) {
	return *static_cast<const T*>(&arg);
}

template<typename R, typename T>
RefPtr<R> as(RefPtr<T> arg) {
	assert_base<R, T>();
	return static_cast<T*>(arg.get());
}

template<typename T> RefPtr<T> as(pRootType arg){ return dynamic_cast<T*>(arg); }
template<typename T> bool implements(const_ref(RootType) arg){ if(as<T>(&arg)) return true; return false; }
template<typename T> bool implements(pRootType arg){ if(as<T>(arg)) return true; return false; }

void for_each(const_ref(IEnumerable) coll, std::function<void(const_ref(Object))> func);

template<typename... Args>
void for_each(const_ref(IEnumerable) coll, std::function<void(const_ref(Object), Args&&...)> func, Args&&... args){
	RefPtr<IEnumerator> en = coll.getEnumerator();
	do { func(en->current(), args...); } while(en->next());
}

template<typename T>
void for_each(const_ref(IEnumerableT<T>) coll, std::function<void(const_ref(T))> func){
	AutoPtr<IEnumeratorT<T>> en = coll.getEnumeratorT();
	do { func(en->currentT()); } while(en->next());
}



template<typename T, typename... Args>
void for_each(const_ref(IEnumerableT<T>) coll, std::function<void(const_ref(T), Args...)> func, Args... args){

	AutoPtr<IEnumeratorT<T>> en = coll.getEnumeratorT();
	while(en->next()) func(en->getCurrentT(), args...);
}

NSP_BETTERCPP_END






