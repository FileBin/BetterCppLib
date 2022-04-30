/*
 * Pointers.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#pragma once
#include "../stdafx.hpp"

#include "../Exceptions.hpp"

NSP_BETTERCPP_BEGIN

template<typename T>
class AutoPtr;

template<typename T = void>
class RefPtr {
protected:
	std::shared_ptr<T*> pptr;
public:

	RefPtr(const RefPtr<T>& other) {
		if (other.pptr.get())
			pptr = other.pptr;
		else
			THROW_NULL_PTR_EXCEPTION(other.pptr.get());
	}

	RefPtr(T* obj = nullptr) {
		pptr = std::make_shared<T*>(obj);
	}

	template<typename... Args>
	static RefPtr<T> create(Args... args) {
		return new T(args...);
	}

	virtual ~RefPtr() {}

	bool isNull() const {
		if (pptr.get() == nullptr)
			THROW_NULL_PTR_EXCEPTION(pptr.get());
		return *pptr == nullptr;
	}

	bool isNotNull() const {
		if (pptr.get() == nullptr)
			THROW_NULL_PTR_EXCEPTION(pptr.get());
		return *pptr != nullptr;
	}

	void release() {
		if (isNotNull()) {
			T* pointer = *pptr;
			delete pointer;
			*pptr = nullptr;
		}
	}

	void releaseArray() {
		if (isNotNull()) {
			T* pointer = *pptr;
			*pptr = nullptr;
			delete[] pointer;
		}
	}

	T* get() const {
		return *pptr;
	}

	T** getAddress() const {
		return pptr.get();
	}

	T* operator->() const {
		if (!*pptr)
			THROW_NULL_PTR_EXCEPTION(*pptr);
		return *pptr;
	}

	const RefPtr<T>& operator=(const RefPtr<T>& other) {
		pptr = other.pptr;
		return *this;
	}

	T& operator*() {
		return **pptr;
	}

	const T& operator*() const {
		return **pptr;
	}
};

template<typename T>
bool operator==(const RefPtr<T>& a, const RefPtr<T>& b) {
	return a.Get() == b.Get();
}

template<typename T>
bool operator==(const RefPtr<T>& a, std::nullptr_t b) {
	return a.isNull();
}

template<typename T>
class AutoPtr : public RefPtr<T> {
public:
	AutoPtr(const RefPtr<T>& other) : RefPtr<T>(other) {}
	AutoPtr(const AutoPtr<T>& other) : RefPtr<T>(new T(**other.pptr)) {}

	template<typename... Args>
	AutoPtr(std::nullptr_t) : RefPtr<T>(nullptr) {}

	template<typename... Args>
	AutoPtr(Args... args) : RefPtr<T>(new T(args...)) {}

	AutoPtr(T* obj) : RefPtr<T>(obj) {}

	~AutoPtr() { this->release(); }

	const RefPtr<T>& operator=(const AutoPtr<T>& other) {
		this->release();
		this->pptr = std::make_shared<T*>(new T(**other.pptr));
		return *this;
	}

	const RefPtr<T>& operator=(std::nullptr_t) {
		this->release();
		this->pptr = std::make_shared<T*>(nullptr);
		return *this;
	}
};

NSP_BETTERCPP_END
