/*
 * List.cpp
 *
 *  Created on: May 1, 2022
 *      Author: filebin
 */

#include "pch.hpp"

NSP_BETTERCPP_BEGIN

String IEnumerable::toString(const_ref(IEnumerable) collection) {
	auto en = collection.getEnumerator();
	bool empty = true;
	String str = "{ ";
	while(en->next()) {
		if(!empty)
			str += ", ";
		const Object& o = en->current();
		str += String::toString<const_ref(Object)>(o);
		empty = false;
	}
	if(empty)
		return "{ empty }";
	str += " }";
	return str;
}

void EventRegistration::cleanup() {
	if(collection_ref.isNotNull()) {
		collection_ref->unregister(coll_id);
		coll_id = unregistered_id;
		collection_ref = nullptr;
	}
}

void EventRegistration::move(EventRegistration& other) {
	other.coll_id = coll_id;
	other.collection_ref = collection_ref;
	coll_id = unregistered_id;
	collection_ref = nullptr;
}

RefPtr<EventRegistration> EventBase::createRegistration(uint id) {
	EventRegistration* reg = new EventRegistration(this);
	reg->coll_id = id;
	return reg;
}

uint EventBase::getID(EventRegistration& reg) { return reg.coll_id; }

void EventBase::check(EventRegistration& reg) {
	if(reg.collection_ref.get() != this)
		THROW_EXCEPTION("Event id doesn't match this event! (maybe you choose wrong event?)");
}

NSP_BETTERCPP_END


