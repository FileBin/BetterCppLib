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

NSP_BETTERCPP_END


