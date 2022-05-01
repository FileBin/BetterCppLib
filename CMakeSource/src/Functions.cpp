/*
 * Functions.cpp
 *
 *  Created on: Apr 30, 2022
 *      Author: filebin
 */

#include "pch.hpp"

NSP_BETTERCPP_BEGIN

void for_each(const_ref(IEnumerable) coll, std::function<void(const_ref(Object))> func){
	RefPtr<IEnumerator> en = coll.getEnumerator();
	while(en->next()) func(en->current());
}

NSP_BETTERCPP_END


