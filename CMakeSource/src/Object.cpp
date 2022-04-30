/*
 * Object.cpp
 *
 *  Created on: Apr 29, 2022
 *      Author: root
 */

#include "pch.hpp"

NSP_BETTERCPP_BEGIN

String Object::getFullName() const {
	return "Object";
}

String Object::toString() const {
	return "Object";
}

ulong Object::hash() const {
	uint n = byteSize();
	ulong* data_chunk = (ulong*)this;
	ulong res = 0;
	for (uint i = 0; i < n; i+=sizeof(ulong))
		res ^= Math::hash(*(data_chunk++));

	return res;
}

NSP_BETTERCPP_END




