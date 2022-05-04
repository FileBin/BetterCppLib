/*
 * List.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#pragma once

#include "../Interfaces/Interfaces.hpp"
#include "../Functions.hpp"

NSP_BETTERCPP_BEGIN

template<typename T>
better_template_class(List, T) better_implements(public IListT<T>) {
public:
private:
	constexpr static uint allocation_step = 4;
	uint reserved = 0;
	uint size = 0;
	T* data = nullptr;
protected:
	T& get_elem(uint i){ return data[i]; }
public:
	~List() { delete data; }
	List() {}

	List(const_ref(List<T>) other) {
		resize(other.size);
		T* data_it = data;
		auto en = other.getEnumeratorT();
		do {
			auto& elem = en->currentT();
			new(data_it++) T(elem);
		} while(en->next());
	}

	List(const T* data_ptr, uint data_count) {
		resize(data_count);
		memcpy(data, data_ptr, data_count);
	}

	List(uint count) { if(count) { resize(count); } }
	List(std::initializer_list<T> initializer) {
		push(initializer);
	}

	operator std::initializer_list<T>() const {
		return asStdVector();
	}

	std::vector<T> asStdVector() const override {
		uint n = size;
		std::vector<T> vec(n);
		for(uint i=0;i<n;i++)
			vec[i] = data[i];
		return vec;
	}

	T& operator[](uint pos) {
		if(pos >= size) THROW_INDEX_OUT_OF_RANGE_EXCEPTION;
		return data[pos];
	}

	T* data_ptr() const {
		return data;
	}

	void reserve(uint n) {
		if(n) {
			if(data) {
				uint m = size;
				void* prev_data = data;

				data = (T*)realloc(data, sizeof(T) * n);
				if(!data) THROW_EXCEPTION("Not enough memory for reallocation!");
				reserved = n;
				if(data != prev_data)
					memmove(data, prev_data, m);

				return;
			}

			data = (T*)new byte[n * sizeof(T)];
			reserved = n;
			return;
		}
		clear();
	}

	void resize(uint n) {
		size = n;
		reserve(n);
	}

	bool empty() const override { return size == 0; }
	uint count() const override { return size; }
	uint length() const { return size; }

	ICollectionT<T>& push(T elem) override {
		//expand
		if(reserved <= size) reserve(size + allocation_step);

		new(data+size) T(elem);
		++size;

		return *this;
	}

	ICollectionT<T>& push(std::initializer_list<T> initializer) {
		uint n = initializer.size();
		if(data) {
			uint last = data;
		    size += n;
			reserve(size);
		    T* data_it = data + last;

			for(const_ref(auto) val : initializer) new(data_it++) T(val);
		} else {
			size = n;
			reserve(n);
		    T* data_it = data;
		    for(const_ref(auto) val : initializer) new(data_it++) T(val);
		}
		return *this;
	}

	T pop() override {
		--size;
		T last = data[size];
		//shrink
		if(reserved < size + allocation_step)
			reserve(size);

		return last;
	}

	void clear() override {
		size = 0;
		reserved = 0;
		delete data;
		data = nullptr;
	}

	const_ref(T) first() const override {
		return data[0];
	}
	const_ref(T) last() const override {
		return data[size-1];
	}
	const_ref(T) at(uint pos) const override {
		if(pos >= size) THROW_INDEX_OUT_OF_RANGE_EXCEPTION;
		return data[pos];
	}

	RefPtr<IEnumerator> getEnumerator() const override {
		return as<IEnumerator, Enumerator>(new_ref<Enumerator>(data, data + size));
	}
	RefPtr<IEnumeratorT<T>> getEnumeratorT() const override {
		return new_ref<Enumerator>(data, data + size);
	}

private:
	better_class(Enumerator), public IEnumeratorT<T> {
	public:
		T* cur, *begin, *end;

		Enumerator(T* begin, T* end) : cur(begin-1), begin(begin), end(end) {}

		const_ref(T) currentT() const override {
			if(cur < begin || cur >= end) THROW_INDEX_OUT_OF_RANGE_EXCEPTION;
			return *cur;
		}

		const_ref(Object) current() const override {
			if(cur < begin || cur >= end) THROW_INDEX_OUT_OF_RANGE_EXCEPTION;
			return *(Object*)cur;
		}

		bool next() override {
			if(++cur != end)
				return true;
			return false;
		}

		void reset() override {
			cur = begin - 1;
		}
	};
};

template<> String String::toString<const_ref(IEnumerable)>(const_ref(IEnumerable) val);
NSP_BETTERCPP_END
