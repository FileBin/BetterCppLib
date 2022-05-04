/*
 * BasePtr.hpp
 *
 *  Created on: May 3, 2022
 *      Author: filebin
 */

#pragma once

#include "../Exceptions.hpp"

NSP_BETTERCPP_BEGIN

//TODO make class EnableThisRefPtr<T>
template<typename T>
class EnableThisRefPtr;

template<typename T>
class AutoPtr;

template<typename T>
class RefPtr;

struct ptr_cluster_hub_base {
protected:
	void* ptr;
	ptr_cluster_hub_base* next = 0;
	uint ref_count = 1;
	byte flags;
public:
	static constexpr byte has_owner_bit = 0x1;
	static constexpr byte extended_bit = 0x2;

	ptr_cluster_hub_base(void* ptr, byte flags = 0) : ptr(ptr), flags(flags) {}

	void* get() { return ptr; }

	void incr() {
		++ref_count;
	}
	void decr();

	void try_destroy();
};

template<typename T>
struct ptr_cluster_hub {
	void (*destoy_func)(T*) = 0;
	T* ptr;
	uint ref_count;
	bool has_owner;

	ptr_cluster_hub(T* obj, bool is_owner) : ptr(obj), has_owner(is_owner), ref_count(1) {}

	void incr() {
		++this->ref_count;
	}

	void decr() {
		--this->ref_count;
		if(this->ref_count == 0) {
			destroyObj();
			delete this;
			return;
		}
	}

	void destroyObj();

	//void destroy() {
	//	destroyObj();
	//	delete this;
	//}
};

struct ptr_cluster_extended_hub : public ptr_cluster_hub_base {
protected:
	ptr_cluster_hub_base* parent;
public:
	ptr_cluster_extended_hub(void* obj, ptr_cluster_hub_base* parent, bool is_owner) :
		ptr_cluster_hub_base(obj,
				is_owner ?
						ptr_cluster_hub_base::has_owner_bit | ptr_cluster_hub_base::extended_bit
						: ptr_cluster_hub_base::extended_bit),
		parent(parent) {}

	void destroy() {
		delete this;
	}
};

/*void ptr_cluster_hub_base::decr() {
	--ref_count;
	if(ref_count == 0) {
		if(flags & has_owner_bit)
			static_cast<ptr_cluster_extended_hub*>(this)->destroy();
		else
			static_cast<ptr_cluster_hub*>(this)->destroy();
	}
}*/


template<typename T>
class BasePtrFunctional {
protected:
	template<typename R>
	friend class AutoPtr;

	template<typename R>
	friend class RefPtr;

	ptr_cluster_hub<T> *data;

	BasePtrFunctional(ptr_cluster_hub<T>* data) : data(data) {}

public:
	bool isNull() const {
		if (data) return data->ptr == nullptr;
		return true;
	}

	void setDestoyer(void (*destroyer)(T*)){
		data->destoy_func = destroyer;
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

template<typename T, bool is_owner, bool is_this_ref_ptr_class = is_base<EnableThisRefPtr<T>,T>::value>
class BasePtr;

template<typename T>
class BasePtr<T, false, false> : public BasePtrFunctional<T> {
protected:
	BasePtr() : BasePtrFunctional<T>(nullptr) {}

	void create(T* obj)  {
		this->data = new ptr_cluster_hub<T>(obj, false);
	}

	BasePtr(ptr_cluster_hub<T>* other) : BasePtrFunctional<T>(other) {
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub<T>* other) {
		if(this->data == other) return;
		disconnect();
		this->data = other;
		if(other) other->incr();
	}

	bool needClone(T* other) { return false; }

	void disconnect() {
		if(this->data) this->data->decr();
		this->data = nullptr;
	}
};

template<typename T>
class BasePtr<T, true, false> : public BasePtrFunctional<T> {
protected:

	BasePtr() : BasePtrFunctional<T>(nullptr) {}

	void create(T* obj)  {
		this->data = new ptr_cluster_hub<T>(obj, true);
	}

	BasePtr(ptr_cluster_hub<T>* other) : BasePtrFunctional<T>(other) {
		if(other->has_owner) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
		this->data->incr();
		this->data->has_owner = true;
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub<T>* other) {
		if(this->data == other) return;
		disconnect();
		if(other) {
			if(other->has_owner) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
			this->data = other;
			other->incr();
			other->has_owner = true;
		} else this->data = nullptr;
	}

	bool needClone(T* other) { return false; }

	void disconnect() {
		if(this->data) {
			this->data->decr();
			this->data->has_owner = false;
		}
		this->data = nullptr;
	}
};


template<typename T>
class EnableThisRefPtr {
protected:
	template<typename R, typename... Args>
	friend struct object_instancer;

	template<typename R>
	friend struct ptr_cluster_hub;

	void* operator new(size_t n) {
		return malloc(n);
	}

	friend class BasePtr<T, false, true>;
	friend class BasePtr<T, true, true>;
	RefPtr<T> refptr_this();
private:
	bool allocated = false;
	ptr_cluster_hub<T>* hub = nullptr;
};

template<typename T>
void ptr_cluster_hub<T>::destroyObj() {
	if(ptr) {
		if(is_base<EnableThisRefPtr<T>, T>::value) {
			auto en_ptr = (EnableThisRefPtr<T>*)ptr;
			if(en_ptr->allocated) delete ptr;
		} else {
			if(destoy_func) destoy_func(ptr);
			else delete ptr;
		}
	}
	ptr = nullptr;
}

template<typename T>
class BasePtr<T, false, true> : public BasePtrFunctional<T> {
protected:
	BasePtr() : BasePtrFunctional<T>(nullptr) {}

	void create(T* _obj)  {
		EnableThisRefPtr<T>& obj = static_cast<EnableThisRefPtr<T>&>(*_obj);
		if(obj.hub) {
			obj.hub->incr();
			this->data = obj.hub;
		} else {
			this->data = obj.hub = new ptr_cluster_hub<T>(_obj, false);
		}
	}

	BasePtr(ptr_cluster_hub<T>* other) : BasePtrFunctional<T>(other) {
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub<T>* other) {
		if(this->data == other) return;
		disconnect();
		this->data = other;
		if(other) other->incr();
	}

	bool needClone(T* other) {
		EnableThisRefPtr<T>& obj = static_cast<EnableThisRefPtr<T>&>(*other);
		return obj.hub;
	}

	void move(T* other) {
		if(other == nullptr) disconnect();
		EnableThisRefPtr<T>& obj = static_cast<EnableThisRefPtr<T>&>(*other);
		if(obj.hub) {
			move(obj.hub);
		} else {
			this->data = obj.hub = new ptr_cluster_hub<T>(other, false);
		}
	}

	void disconnect() {
		if(this->data) this->data->decr();
		this->data = nullptr;
	}
};

template<typename T>
class BasePtr<T, true, true> : public BasePtrFunctional<T> {
protected:

	BasePtr() : BasePtrFunctional<T>(nullptr) {}

	void create(T* _obj)  {
		EnableThisRefPtr<T>& obj = static_cast<EnableThisRefPtr<T>&>(*_obj);
		if(obj.hub) {
			obj.hub->incr();
			this->data = obj.hub;
		} else {
			this->data = obj.hub = new ptr_cluster_hub<T>(_obj, true);
		}
	}

	BasePtr(ptr_cluster_hub<T>* other) : BasePtrFunctional<T>(other) {
		if(other->has_owner) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub<T>* other) {
		if(this->data == other) return;
		disconnect();
		if(other) {
			if(other->has_owner) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
			this->data = other;
			other->incr();
			other->has_owner = true;
		} else this->data = nullptr;
	}

	bool needClone(T* other) {
		EnableThisRefPtr<T>& obj = static_cast<EnableThisRefPtr<T>&>(*other);
		return obj.hub;
	}

	void move(T* other) {
		if(other == nullptr) disconnect();
		EnableThisRefPtr<T>& obj = static_cast<EnableThisRefPtr<T>&>(*other);
		if(obj.hub) {
			move(obj.hub);
		} else {
			this->data = obj.hub = new ptr_cluster_hub<T>(other, false);
		}
	}

	void disconnect() {
		if(this->data) {
			this->data->decr();
			this->data->has_owner = false;
		}
		this->data = nullptr;
	}
};

NSP_BETTERCPP_END
