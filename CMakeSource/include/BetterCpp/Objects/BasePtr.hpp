/*
 * BasePtr.hpp
 *
 *  Created on: May 3, 2022
 *      Author: filebin
 */

#pragma once

#include "../Exceptions.hpp"

NSP_BETTERCPP_BEGIN

template<typename T>
class EnableThisRefPtr;

template<typename T>
class AutoPtr;

template<typename T>
class RefPtr;

template<typename T>
void default_destroy(void* ptr) {
	delete (T*)ptr;
}

struct ptr_cluster_hub_root;

namespace ptr {
	namespace flags {
		constexpr byte has_owner 	= 0x1;
		constexpr byte extended 	= 0x2;
		constexpr byte is_class 	= 0x4;
	};
}

//21 bytes for 64-bit
//13 bytes for 64-bit
struct ptr_cluster_hub_base {
protected:
	friend struct ptr_cluster_hub_root;
	void* ptr;
	ptr_cluster_hub_base* next = 0;
	uint ref_count = 1;
	byte flags;
public:

	ptr_cluster_hub_base(void* ptr, byte flags = 0)
	: ptr(ptr),
	  flags(flags) {}

	ptr_cluster_hub_base(const_ref(ptr_cluster_hub_base)) = delete;

	template<typename T>
	T* get() {
		return (T*)ptr;
	}

	template<typename T>
	T** get_address() {
		return (T**)&ptr;
	}

	void incr() {
		++ref_count;
	}
	void decr();

	ptr_cluster_hub_base* push(void* inner_ptr, bool is_owner);

	bool has_refs();

	bool has_owner() {
		return flags & ptr::flags::has_owner;
	}

	void addFlag(byte flag) {
		flags |= flag;
	}

	void try_destroy();

	ptr_cluster_hub_root* root();
};

//29 bytes for 64-bit
//17 bytes for 32-bit
struct ptr_cluster_hub_root : public ptr_cluster_hub_base {
public:
	void (*destoy_func)(void*);

	template<typename T>
	ptr_cluster_hub_root(T* obj, bool is_owner);

	void destroyObj();

protected:
	friend struct ptr_cluster_hub_base;
	void destroy() {
		destroyObj();
		ptr_cluster_hub_base* it = next;
		while(it){
			ptr_cluster_hub_base* t = it;
			it = it->next;
			delete t;
		}
		delete this;
	}
};

//29 bytes for 64-bit
//17 bytes for 32-bit
struct ptr_cluster_extended_hub : public ptr_cluster_hub_base {
protected:
	friend struct ptr_cluster_hub_base;
	ptr_cluster_hub_root* root_hub;
public:
	ptr_cluster_extended_hub(void* obj, ptr_cluster_hub_root* parent, bool is_owner) :
		ptr_cluster_hub_base(obj, ptr::flags::extended | (is_owner ? ptr::flags::has_owner : 0)),
		root_hub(parent) {}
};

template<typename T>
class BasePtrFunctional {
protected:
	template<typename R>
	friend class AutoPtr;

	template<typename R>
	friend class RefPtr;

	template<typename Fr, typename To, bool is_base>
	friend struct object_converter;
	ptr_cluster_hub_base *data;

	BasePtrFunctional(ptr_cluster_hub_base* data) : data(data) {}

public:
	bool isNull() const {
		if (data) return data->get<T>() == nullptr;
		return true;
	}

	void setDestoyer(void (*destroyer)(T*)){
		data->root()->destoy_func = (void (*)(void*))destroyer;
	}

	bool isNotNull() const {
		if (data) return data->get<T>() != nullptr;
		return false;
	}

	void release() {
		if (isNotNull()) {
			data->root()->destroyObj();
		}
	}

	T* get() {
		if(data) return data->get<T>();
		THROW_NULL_PTR_EXCEPTION(data);
	}

	T* cget() const {
		if(data) return data->get<T>();
		THROW_NULL_PTR_EXCEPTION(data);
	}

	T** getAddress() {
		return data->get_address<T>();
	}

	T* operator->() {
		if(data) return data->get<T>();
		THROW_NULL_PTR_EXCEPTION(data);
	}

	T& operator*() {
		if(data) return *data->get<T>();
		THROW_NULL_PTR_EXCEPTION(data);
	}

	const T& operator*() const {
		if(data) return *data->get<T>();
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
		this->data = new ptr_cluster_hub_root((T*)obj, false);
	}

	BasePtr(ptr_cluster_hub_base* other) : BasePtrFunctional<T>(other) {
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub_base* other) {
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
		this->data = new ptr_cluster_hub_root((T*)obj, true);
	}

	BasePtr(ptr_cluster_hub_base* other) : BasePtrFunctional<T>(other) {
		if(other->has_owner()) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
		this->data->incr();
		this->data->addFlag(ptr::flags::has_owner);
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub_base* other) {
		if(this->data == other) return;
		disconnect();
		if(other) {
			if(other->has_owner()) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
			this->data = other;
			other->incr();
			other->addFlag(ptr::flags::has_owner);
		} else this->data = nullptr;
	}

	bool needClone(T* other) { return false; }

	void disconnect() {
		if(this->data) {
			this->data->decr();
			this->data->addFlag(ptr::flags::has_owner);
		}
		this->data = nullptr;
	}
};

struct EnableThisRefPtrBase {
	template<typename T, bool is_class, typename... Args>
	friend struct object_instancer;
	friend struct ptr_cluster_hub_root;
protected:
	ptr_cluster_hub_root* hub = nullptr;
	bool allocated = false;
	void* operator new(size_t n) { return malloc(n); } // @suppress("Function cannot be resolved")
};

template<typename T>
class EnableThisRefPtr : public EnableThisRefPtrBase {
protected:
	friend class BasePtr<T, false, true>;
	friend class BasePtr<T, true, true>;
	RefPtr<T> refptr_this();
};

template<typename T>
ptr_cluster_hub_root::ptr_cluster_hub_root(T* obj, bool is_owner)
: ptr_cluster_hub_base((void*)obj,
		(is_owner ? ptr::flags::has_owner : 0) |
		(is_base<EnableThisRefPtr<T>, T>::value ? ptr::flags::is_class : 0)
		),
  destoy_func(default_destroy<T>)
  {}

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
			this->data = obj.hub = new ptr_cluster_hub_root((T*)_obj, false);
		}
	}

	BasePtr(ptr_cluster_hub_base* other) : BasePtrFunctional<T>(other) {
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub_base* other) {
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
			this->data = obj.hub = new ptr_cluster_hub_root((T*)other, false);
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
			this->data = obj.hub = new ptr_cluster_hub_root((T*)_obj, true);
		}
	}

	BasePtr(ptr_cluster_hub_base* other) : BasePtrFunctional<T>(other) {
		if(other->has_owner()) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
		this->data->incr();
	}

	~BasePtr() {
		disconnect();
	}

	void move(ptr_cluster_hub_base* other) {
		if(this->data == other) return;
		disconnect();
		if(other) {
			if(other->has_owner()) THROW_EXCEPTION("One pointer cluster can't have two or more owners!");
			this->data = other;
			other->incr();
			other->addFlag(ptr::flags::has_owner);
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
			this->data = obj.hub = new ptr_cluster_hub_root((T*)other, true);
		}
	}

	void disconnect() {
		if(this->data) {
			this->data->decr();
			this->data->addFlag(ptr::flags::has_owner);
		}
		this->data = nullptr;
	}
};

NSP_BETTERCPP_END
