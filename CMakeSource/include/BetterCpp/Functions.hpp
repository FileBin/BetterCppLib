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

template<typename From, typename To, bool is_base = is_base<To, From>::value>
struct object_converter;

template<typename From, typename To>
struct object_converter<From, To, true> {
	static RefPtr<To> convert(const RefPtr<From>& from) {
		return from;
	}
};

template<typename From, typename To>
struct object_converter<From, To, false> {
	static RefPtr<To> convert(const RefPtr<From>& from) {
		RefPtr<To> to;
		const To* ptr = dynamic_cast<const To*>(from.cget());
		if(ptr == nullptr) return to;
		to.data = from.data->push((void*)ptr, false);
		return to;
	}
};

template<typename Base, typename Derived>
constexpr void assert_base(){ static_assert(is_base<Base, Derived>::value, "Compilation error: Base isn't base of Derived!"); }

template<typename T> RefPtr<RootType> root(RefPtr<T> arg) {
assert_base<RootType, T>();
return *(pRootType*)&arg;
}

template<typename T, typename R>
const_ref(T) as(const_ref(R) arg) {
	return *static_cast<const T*>(&arg);
}

template<typename R, typename T>
RefPtr<R> as(RefPtr<T> arg) {
	return object_converter<T, R>::convert(arg);
}

template<typename R, typename T>
RefPtr<R> as(T* arg) {
	assert_base<R, T>();
	return RefPtr<R>(static_cast<R*>(arg));
}
template<typename T, bool is_class, typename... Args>
struct object_instancer;

template<typename T, typename... Args>
struct object_instancer<T, true, Args...> {
	static RefPtr<T> create(Args... args) {
		T* obj = new T(args...);
		auto ptr = static_cast<EnableThisRefPtr<T>*>(obj);
		ptr->allocated = true;
		RefPtr<T> ref(obj);
		((ptr_cluster_hub_root*)ref.data)->enable_ptr_ref_offset.set_offset<T, EnableThisRefPtrBase>(obj);
		return ref;
	}
};

template<typename T, typename... Args>
struct object_instancer<T, false, Args...> {
	static RefPtr<T> create(Args... args) {
		return RefPtr<T>(new T(args...));
	}
};

template<typename T, typename... ArgsT>
RefPtr<T> new_ref(ArgsT... args) {
	//return RefPtr<T>(new T(args...));
	auto ref = object_instancer<T, is_base<EnableThisRefPtr<T>, T>::value, ArgsT...>::create(args...);
	return ref;
}

template<typename T> RefPtr<T> as(RefPtr<RootType> arg){ return dynamic_cast<T*>(arg); }
template<typename T> bool implements(const_ref(RootType) arg){ if(as<T>(&arg)) return true; return false; }
template<typename T> bool implements(RefPtr<RootType> arg){ if(as<T>(arg)) return true; return false; }

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






