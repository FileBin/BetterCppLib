/*
 * Interfaces.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#pragma once

#include "../stdafx.hpp"
#include "../Objects/Pointers.hpp"

NSP_BETTERCPP_BEGIN

better_interface(IEnumerator) {
	virtual const_ref(Object) current() const = 0;
	virtual bool next() = 0;
	virtual void reset() = 0;
	virtual ~IEnumerator() {}
};

template<typename T>
better_interface(IEnumeratorT) better_implements(IEnumerator) {
	virtual const_ref(T) currentT() const = 0;
	virtual ~IEnumeratorT() {}
};

template<typename T>
better_interface(IEnumerableT) {
	virtual AutoPtr<IEnumeratorT<T>> getEnumeratorT() const = 0;
	virtual ~IEnumerableT() {}

	static String toString(const_ref(IEnumerableT<T>) collection);/* {
		auto en = collection.getEnumeratorT();
		bool empty = true;
		String str = "{ ";
		while(en->next()) {
			if(!empty)
				str += ", ";
			const T& o = en->current();
			str += String::toString<T>(o);
			empty = false;
		}
		if(empty)
			return "{ empty }";
		str += " }";
		return str;
	}*/
};

better_interface(IEnumerable) {
	virtual AutoPtr<IEnumerator> getEnumerator() const = 0;
	virtual ~IEnumerable() {}

	static String toString(const_ref(IEnumerable) collection);/* {
		auto en = collection.getEnumerator();
		bool empty = true;
		String str = "{ ";
		while(en->next()) {
			if(!empty)
				str += ", ";
			const auto& o = en->current();
			str += String::toString(o);
			empty = false;
		}
		if(empty)
			return "{ empty }";
		str += " }";
		return str;
	}*/
};

template<typename T>
better_interface(ICollectionT) better_implements(public IEnumerableT<T>, public IEnumerable) {
	virtual bool empty() const = 0;
	virtual uint count() const = 0;

	virtual ICollectionT<T>& push(T elem) = 0;
	virtual T pop() = 0;
	virtual void clear() = 0;

	virtual ~ICollectionT() {}
};

template<typename T>
better_interface(IListT) better_implements(public ICollectionT<T>) {
	virtual const_ref(T) first() const = 0;
	virtual const_ref(T) last() const = 0;
	virtual const_ref(T) at(uint index) const = 0;
	virtual std::vector<T> asStdVector() const = 0;
	virtual ~IListT() {}
};

better_interface(IRegistrable) {
	virtual void Register(size_t idx) = 0;
	virtual size_t Unregister() = 0;
	virtual ~IRegistrable() {}
};


better_interface(ICleanable) {
    virtual void cleanup() = 0;
    virtual ~ICleanable() {};
};

NSP_BETTERCPP_END
