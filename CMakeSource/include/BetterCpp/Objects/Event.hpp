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
struct EventRegistration;

class_with_ref_ptr(EventBase) {
protected:
	friend struct EventRegistration;
	RefPtr<EventRegistration> createRegistration(uint id);
	uint getID(EventRegistration& reg);
	void check(EventRegistration& reg);
	virtual void unregister(uint id) = 0;
public:
	virtual ~EventBase() {}
};

struct EventRegistration : ICleanable {
friend class EventBase;
constexpr static uint unregistered_id = 0xffffffff;
EventRegistration(RefPtr<EventBase> collection_ref) : collection_ref(collection_ref), coll_id(unregistered_id) {};
private:
	uint coll_id;
	RefPtr<EventBase> collection_ref;
public:
	EventRegistration() : collection_ref(nullptr), coll_id(unregistered_id) {}
	~EventRegistration() { cleanup(); }
	EventRegistration(const_ref(EventRegistration) other) = delete;

	void move(EventRegistration& other);
	bool isRegistered() { return coll_id != unregistered_id; }

	const_ref(EventRegistration) operator=(const_ref(EventRegistration) other) {
		cleanup();
		collection_ref = other.collection_ref;
		coll_id = other.coll_id;
		return *this;
	}

	void cleanup() override;

};

template<typename ...args_t>
class Event : EventBase {
public:
	typedef void func_t(args_t...);
	typedef std::function<func_t> std_func_t;

private:
	RegisterCollection<std_func_t> handlers;
protected:
	void unregister(uint id) override {
		handlers.unregister(id);
	}
public:
	void subscribe(EventRegistration& reg, std_func_t function) {
		createRegistration(handlers.registerObj(function))->move(reg);
	}

	void subscribe(EventRegistration& reg, func_t* function) {
		subscribe(reg, [function]() { function(); });
	}

	void unsubscribe(EventRegistration& reg) {
		reg.cleanup();
	}

	void invoke(args_t... args) {
		const_ref(auto) coll = handlers.getCollectionWithGaps();

		//auto en = coll.getEnumeratorT();
		//while(en->next()) {
		for(auto& func : coll) {
			if(func.isNull()) return;
			(*func)(args...);
		}
		//}
	}
};

NSP_BETTERCPP_END
