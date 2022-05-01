/*
 * Event.hpp
 *
 *  Created on: Apr 10, 2022
 *      Author: root
 */

#include "../stdafx.hpp"

#include "RegisterCollection.hpp"

#pragma once

NSP_BETTERCPP_BEGIN

template<typename ...args_t>
class Event {
public:
	typedef void func_t(args_t...);
	typedef std::function<func_t> std_func_t;
	struct UUID {
	friend class Event;
	UUID() : coll_id(SIZE_MAX) {};
	private:
		size_t coll_id;
	};

private:
	RegisterCollection<std_func_t> handlers;

public:
	UUID subscribe(std_func_t function) {
		UUID uuid;
		uuid.coll_id = handlers.registerObj(function);
		return uuid;
	}

	UUID subscribe(func_t* function) {
		return subscribe([function]() { function(); });
	}

	void unsubscribe(UUID uuid) {
		handlers.unregister(uuid.coll_id);
	}

	void invoke(args_t... args) {
		const_ref(auto) coll = as<IEnumerableT<AutoPtr<std_func_t>>>(handlers.getCollectionWithGaps());

		//auto en = coll.getEnumeratorT();
		//while(en->next()) {
		foreach(coll) {
			auto& func = foreach_elem;//en->currentT();
			if(func.isNull()) return;
			(*func.get())(args...);
		}
		//}
	}
};

NSP_BETTERCPP_END
