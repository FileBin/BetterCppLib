/*
 * RegisterCollection.hpp
 *
 *  Created on: Apr 29, 2022
 *      Author: filebin
 */

#pragma once

#include "../Interfaces/Interfaces.hpp"
#include "Pointers.hpp"

#include "../Core.hpp"

NSP_BETTERCPP_BEGIN

template<typename ElemT, typename Id_t = uint>
class RegisterCollection {
public:
	typedef RefPtr<ElemT> ptr_t;
private:
	List<ptr_t> collection{};
	List<Id_t> emptyPositions{};
public:

	RegisterCollection() {
		emptyPositions.reserve(0xfff);
	}

	const_ref(List<ptr_t>) getCollection() const {
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


	const_ref(List<ptr_t>) getCollectionWithGaps() const {
		return collection;
	}

	ptr_t getElemAt(Id_t idx) {
		return collection[idx];
	}

	Id_t registerPtr(ptr_t pObject) {
		if (emptyPositions.empty()) {
			collection.push_back(pObject);
			return collection.size() - 1;
		} else {
			uint idx = emptyPositions.size() - 1;
			collection[idx] = pObject;
			emptyPositions.pop_back();
			return idx;
		}
	}

	Id_t registerObj(ElemT object) {
		return registerPtr(ptr_t{new ElemT(object)});
	}

	void unregister(Id_t index) {
		collection[index] = nullptr;
		emptyPositions.push_back(index);
	}

	void cleanup() {
		collection.clear();
		emptyPositions.clear();
	}
};

NSP_BETTERCPP_END
