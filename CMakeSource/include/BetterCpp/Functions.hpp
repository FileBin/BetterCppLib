/*
 * Functions.hpp
 *
 *  Created on: Apr 30, 2022
 *      Author: filebin
 */

#pragma once

#include "stdafx.hpp"

NSP_BETTERCPP_BEGIN

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
	return RefPtr<R>(static_cast<R*>(arg.get()));
}

template<typename R, typename T>
RefPtr<R> as(T* arg) {
	assert_base<R, T>();
	return RefPtr<R>(static_cast<R*>(arg));
}

template<typename T, typename... ArgsT>
RefPtr<T> new_ref(ArgsT... args) {
	return RefPtr<T>(new T(args...));
}

template<typename T> RefPtr<T> as(pRootType arg){ return dynamic_cast<T*>(arg); }
template<typename T> bool implements(const_ref(RootType) arg){ if(as<T>(&arg)) return true; return false; }
template<typename T> bool implements(pRootType arg){ if(as<T>(arg)) return true; return false; }

void for_each(const_ref(IEnumerable) coll, std::function<void(const_ref(Object))> func);

template<typename... Args>
void for_each(const_ref(IEnumerable) coll, std::function<void(const_ref(Object), Args&&...)> func, Args&&... args){
	RefPtr<IEnumerator> en = coll.getEnumerator();
	while(en->next()) func(en->current(), args...);
}

template<typename T>
void for_each(const_ref(IEnumerableT<T>) coll, std::function<void(const_ref(T))> func){
	AutoPtr<IEnumeratorT<T>> en = coll.getEnumeratorT();
	while(en->next()) func(en->currentT());
}



template<typename T, typename... Args>
void for_each(const_ref(IEnumerableT<T>) coll, std::function<void(const_ref(T), Args...)> func, Args... args){

	AutoPtr<IEnumeratorT<T>> en = coll.getEnumeratorT();
	while(en->next()) func(en->getCurrentT(), args...);
}




NSP_BETTERCPP_END






