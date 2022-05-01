/*
 * RegisterCollection.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#pragma once

#include "../Interfaces/Interfaces.hpp"
#include "Pointers.hpp"
#include "List.hpp"

NSP_BETTERCPP_BEGIN

template<typename ElemT, typename Id_t = uint>
class RegisterCollection {
public:
	typedef AutoPtr<ElemT> ptr_t;
private:
	List<ptr_t> collection{};
	List<Id_t> emptyPositions{};
public:

	RegisterCollection() {
		emptyPositions.reserve(0xfff);
	}

	const_ref(ICollectionT<ptr_t>) getCollection() const {
		List<ptr_t> list{};
		auto n = collection.size();
		list.reserve(n);
		for (Id_t i = 0; i < n; i++) {
			const_ref(auto) elem = collection[i];
			if (elem != nullptr)
				list.push(elem);
		}
		return list;
	}


	const_ref(ICollectionT<ptr_t>) getCollectionWithGaps() const {
		return collection;
	}

	ptr_t getElemAt(Id_t idx) {
		return collection[idx];
	}

	Id_t registerPtr(ptr_t pObject) {
		if (emptyPositions.empty()) {
			collection.push(pObject);
			return collection.length() - 1;
		} else {
			auto idx = emptyPositions.last();
			collection[idx] = pObject;
			emptyPositions.pop();
			return idx;
		}
	}

	Id_t registerObj(ElemT object) {
		return registerPtr(new ElemT(object));
	}

	void unregister(Id_t index) {
		collection[index] = nullptr;
		emptyPositions.push(index);
	}
};

NSP_BETTERCPP_END
