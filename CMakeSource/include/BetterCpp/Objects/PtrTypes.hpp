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
#ifdef _DEBUG
	T* debugPtr;
#endif
public:
	/*template<typename R>
	RefPtr(const RefPtr<R>& other) : BasePtr<T, false>() {
		static_assert(is_base<T, R>::value, "Can't convert R to T");
		this->data = other.data->push((void*)(static_cast<T*>(other.cget())), false);
	} //cast constructor*/

	template<typename R>
	RefPtr(const RefPtr<R>& other) : BasePtr<T, false>(other.data, other.offset) {
		static_assert(is_base<T, R>::value, "Can't convert R to T");
		this->offset.template set_offset<R, T>(other.cget());
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
	} //cast constructor

	RefPtr(const RefPtr<T>& other) : BasePtr<T, false>(other.data, other.offset) {
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
	} //connect constructor

	RefPtr(const AutoPtr<T>& other);

	explicit RefPtr(T* obj = nullptr) : BasePtr<T, false>() {
		if(obj) this->create(obj);
#ifdef _DEBUG
		debugPtr = obj;
#endif
	}

	RefPtr(std::nullptr_t) : BasePtr<T, false>() {
#ifdef _DEBUG
		debugPtr = nullptr;
#endif
	}

	const RefPtr<T>& operator=(const RefPtr<T>& other) {
		this->move(other.data, other.offset);
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
		return *this;
	}

	const RefPtr<T>& operator=(T* other) {
		static_assert(is_base<EnableThisRefPtr<T>, T>::value, "Object must has this of type RefPtr<T> for this operator=");
		this->move(other);
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
		return *this;
	}

	const RefPtr<T>& operator=(std::nullptr_t) {
		this->disconnect();
		this->offset = {};
#ifdef _DEBUG
		debugPtr = nullptr;
#endif
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
#ifdef _DEBUG
	T* debugPtr;
#endif
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
	AutoPtr() : BasePtr<T, true>() {
#ifdef _DEBUG
		debugPtr = nullptr;
#endif
	}

	AutoPtr(const RefPtr<T>& other) : BasePtr<T, true>(other.data, other.offset) {
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
	} //try connect

	AutoPtr(const AutoPtr<T>& other) : BasePtr<T, true>() {
		clone(other.data);
		this->offset = other.offset;
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
	}

	explicit AutoPtr(T* ptr) : BasePtr<T, true>() { 
		this->create(ptr); 
#ifdef _DEBUG
		debugPtr = ptr;
#endif
	}

	RefPtr<T> cloneNew() const { return object_cloner<T>::cloneNew(*this->data->ptr); }

	~AutoPtr() {
		this->release();
	}

	const AutoPtr<T>& operator=(const BasePtrFunctional<T>& other) {
		this->release();
		this->disconnect();
		clone(other.data);
		this->offset = other.offset;
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
		return *this;
	}

	const AutoPtr<T>& operator=(T* other) {
		this->release();
		this->disconnect();
		if(needClone(other)) this->create(__cloneNew(other));
		else this->create(other);
#ifdef _DEBUG
		debugPtr = this->cget();
#endif
		return *this;
	}

	const AutoPtr<T>& operator=(std::nullptr_t) {
		this->release();
		this->disconnect();
		this->offset = {};
#ifdef _DEBUG
		debugPtr = nullptr;
#endif
		return *this;
	}
};

template<typename T>
RefPtr<T>::RefPtr(const AutoPtr<T>& other) : BasePtr<T, false>(other.data, other.offset) {
#ifdef _DEBUG
	debugPtr = this->cget();
#endif
} //connect

template<typename T>
RefPtr<T> EnableThisRefPtr<T>::refptr_this(){ return RefPtr<T>((T*)this); }
NSP_BETTERCPP_END
