/*
 * Pointers.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#include "../stdafx.hpp"

#ifndef INCLUDE_BETTERCPP_OBJECTS_POINTERS_HPP_
#define INCLUDE_BETTERCPP_OBJECTS_POINTERS_HPP_

#include "../Exceptions.hpp"

NSP_BETTERCPP_BEGIN


template<typename T>
class AutoPtr;

template<typename T>
class RefPtr;

template<typename T>
class BasePtrFunctional {
protected:
	friend class AutoPtr<T>;
	friend class RefPtr<T>;
	struct ptr_data {
		T* ptr;
		uint ref_count;
		bool has_owner;

		ptr_data(T* obj, bool is_owner) {
			has_owner = is_owner;
			ref_count = 1;
			ptr = obj;
		}

		void incr() {
			++ref_count;
		}

		void decr() {
			--ref_count;
			if(ref_count == 0) {
				delete this;
				return;
			}
		}

		void destroyObj() {
			delete ptr;
			ptr = nullptr;
		}
	} *data;

	BasePtrFunctional(ptr_data* data) : data(data) {}

public:
	bool isNull() const {
		if (data) return data->ptr == nullptr;
		return true;
	}

	bool isNotNull() const {
		if (data) return data->ptr != nullptr;
		return false;
	}

	void release() {
		if (isNotNull()) {
			data->destroyObj();
		}
	}

	T* get() const {
		if(data) return data->ptr;
		THROW_NULL_PTR_EXCEPTION(data);
	}

	T** getAddress() const {
		return data;
	}

	T* operator->() const {
		if(data) return data->ptr;
		THROW_NULL_PTR_EXCEPTION(data);
	}

	T& operator*() {
		if(data) return *data->ptr;
		THROW_NULL_PTR_EXCEPTION(data);
	}

	const T& operator*() const {
		if(data) return *data->ptr;
		THROW_NULL_PTR_EXCEPTION(data);
	}
};

template<typename T, bool is_owner>
class BasePtr;

template<typename T>
class BasePtr<T, true> : public BasePtrFunctional<T> {
protected:
	typedef typename BasePtrFunctional<T>::ptr_data ptr_data;

	BasePtr() : BasePtrFunctional<T>(nullptr) {}

	void create(T* obj)  {
		this->data = new ptr_data(obj, true);
	}

	BasePtr(ptr_data* other) : BasePtrFunctional<T>(other) {
		if(other->has_owner) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_data* other) {
		if(this->data == other) return;
		disconnect();
		if(other) {
			if(other->has_owner) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
			this->data = other;
			other->incr();
			other->has_owner = true;
		} else this->data = nullptr;
	}

	void disconnect() {
		if(this->data) {
			this->data->decr();
			this->data->has_owner = false;
		}
	}
};

template<typename T>
class BasePtr<T, false> : public BasePtrFunctional<T> {
protected:
	typedef typename BasePtrFunctional<T>::ptr_data ptr_data;

	BasePtr() : BasePtrFunctional<T>(nullptr) {}

	void create(T* obj)  {
		this->data = new ptr_data(obj, false);
	}

	BasePtr(ptr_data* other) : BasePtrFunctional<T>(other) {
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_data* other) {
		if(this->data == other) return;
		disconnect();
		this->data = other;
		if(other) other->incr();
	}

	void disconnect() {
		if(this->data) this->data->decr();
	}
};

template<typename T>
class RefPtr : public BasePtr<T, false> {
public:

	RefPtr(const RefPtr<T>& other) : BasePtr<T, false>(other.data) {} //connect constructor

	RefPtr(const AutoPtr<T>& other);

	RefPtr(T* obj = nullptr) : BasePtr<T, false>() {
		if(obj) this->create(obj);
	}

	const RefPtr<T>& operator=(const RefPtr<T>& other) {
		this->move(other.data);
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
	typedef typename BasePtrFunctional<T>::ptr_data ptr_data;
friend class RefPtr<T>;
	static T* __cloneNew(ptr_data* data) {
		return object_cloner<T>::cloneNew(*data->ptr);
	}
public:
	AutoPtr() : BasePtr<T, true>() {}

	AutoPtr(const RefPtr<T>& other) : BasePtr<T, true>(other.data) {} //try connect

	AutoPtr(const AutoPtr<T>& other) : BasePtr<T, true>() { this->create(__cloneNew(other.data)); }

	template<typename R>
	AutoPtr(R* ptr) : BasePtr<T, true>() { this->create((T*)ptr); }

	AutoPtr(T* ptr) : BasePtr<T, true>() { this->create(ptr); }

	RefPtr<T> cloneNew() const { return object_cloner<T>::cloneNew(*this->data->ptr); }

	~AutoPtr() {
		this->release();
	}

	const AutoPtr<T>& operator=(const AutoPtr<T>& other) {
		this->release();
		this->disconnect();
		this->create(__cloneNew(other.data));
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
NSP_BETTERCPP_END

#include "../Core.hpp"

#endif /*INCLUDE_BETTERCPP_OBJECTS_POINTERS_HPP_*/
