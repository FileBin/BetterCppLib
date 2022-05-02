/*
 * Object.cpp
 *
 *  Created on: Apr 29, 2022
 *      Author: root
 */

#include "pch.hpp"

NSP_BETTERCPP_BEGIN

RefPtr<Object> Object::clone() const {
	return cloneNewUnsafe();
	//return new Object();
}

Object* Object::cloneNewUnsafe() const {
	THROW_EXCEPTION((String(type().info.name()) + " is abstract class").toUtf8().c_str());
	//return new Object();
}

const Type Object::type() const {
	return Type(typeid(*this), sizeof(*this));
}

ulong Object::hash(const_ref(Object) obj) {
	uint n = obj.type().size;
	ulong* data_chunk = (ulong*)&obj;
	ulong res = 0;
	for (uint i = 0; i < n; i+=sizeof(ulong))
		res ^= Math::hash(*(data_chunk++));

	return res;
}

NSP_BETTERCPP_END




