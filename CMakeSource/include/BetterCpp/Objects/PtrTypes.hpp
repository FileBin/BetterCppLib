/*
 * PtrTypes.hpp
 *
 *  Created on: May 4, 2022
 *      Author: filebin
 */

#pragma once

#include "../stdafx.hpp"
#include "BasePtr.hpp"

#include "fmt/format.h"

NSP_BETTERCPP_BEGIN

template<typename T>
class RefPtr : public BasePtr<T, false> {
public:
	template<typename R>
	RefPtr(const RefPtr<R>& other) : BasePtr<T, false>() {
		static_assert(is_base<T, R>::value, "Can't convert R to T");
		this->data = other.data->push((void*)(static_cast<T*>(other.cget())), false);
	} //cast constructor

	RefPtr(const RefPtr<T>& other) : BasePtr<T, false>(other.data) {} //connect constructor

	RefPtr(const AutoPtr<T>& other);

	explicit RefPtr(T* obj = nullptr) : BasePtr<T, false>() {
		if(obj) this->create(obj);
	}

	RefPtr(std::nullptr_t) : BasePtr<T, false>() {}

	const RefPtr<T>& operator=(const RefPtr<T>& other) {
		this->move(other.data);
		return *this;
	}

	const RefPtr<T>& operator=(T* other) {
		static_assert(is_base<EnableThisRefPtr<T>, T>::value, "Object must has this of type RefPtr<T> for this operator=");
		this->move(other);
		return *this;
	}

	const RefPtr<T>& operator=(std::nullptr_t) {
		this->disconnect();
		return *this;
	}
};

template<typename T, bool is_owner>
bool operator==(const BasePtrFunctional<T>& a, const BasePtrFunctional<T>& b) {
	return a.get() == b.get();
}

template<typename T>
bool operator==(const BasePtrFunctional<T>& a, std::nullptr_t b) {
	return a.isNull();
}

template<typename T>
bool operator==(std::nullptr_t b, const BasePtrFunctional<T>& a) {
	return a.isNull();
}

template<typename T>
class AutoPtr : public BasePtr<T, true> {
private:
friend class RefPtr<T>;
	static RefPtr<T> __cloneNew(ptr_cluster_hub_base* data) {
		return object_cloner<T>::cloneNew(*data->get<T>());
	}

	void clone(ptr_cluster_hub_base* hub) {
		auto ptr = __cloneNew(hub);
		this->data = ptr.data;
		this->data->incr();
	}
public:
	AutoPtr() : BasePtr<T, true>() {}

	AutoPtr(const RefPtr<T>& other) : BasePtr<T, true>(other.data) {} //try connect

	AutoPtr(const AutoPtr<T>& other) : BasePtr<T, true>() {
		clone(other.data);
	}

	explicit AutoPtr(T* ptr) : BasePtr<T, true>() { this->create(ptr); }

	RefPtr<T> cloneNew() const { return object_cloner<T>::cloneNew(*this->data->ptr); }

	~AutoPtr() {
		this->release();
	}

	const AutoPtr<T>& operator=(const BasePtrFunctional<T>& other) {
		this->release();
		this->disconnect();
		clone(other.data);
		return *this;
	}

	const AutoPtr<T>& operator=(T* other) {
		this->release();
		this->disconnect();
		if(needClone(other)) this->create(__cloneNew(other));
		else this->create(other);
		return *this;
	}

	const AutoPtr<T>& operator=(std::nullptr_t) {
		this->release();
		this->disconnect();
		return *this;
	}
};

template<typename T>
RefPtr<T>::RefPtr(const AutoPtr<T>& other) : BasePtr<T, false>(other.data) {} //connect

template<typename T>
RefPtr<T> EnableThisRefPtr<T>::refptr_this(){ return RefPtr<T>((T*)this); }
NSP_BETTERCPP_END
