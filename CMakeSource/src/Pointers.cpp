/*
 * Pointers.cpp
 *
 *  Created on: May 4, 2022
 *      Author: filebin
 */

#include "pch.hpp"

NSP_BETTERCPP_BEGIN

void ptr_cluster_hub_base::decr() {
	--ref_count;
	try_destroy();
}

void ptr_cluster_hub_base::try_destroy() {
	ptr_cluster_hub_root* r = root();
	if(!r->has_refs()) {
		r->destroy();
	}
}

bool ptr_cluster_hub_base::has_refs() {
	if(ref_count) return true;
	if(next) return next->has_refs();
	return false;
}

ptr_cluster_hub_root* ptr_cluster_hub_base::root() {
	if(flags & ptr::flags::extended)
		return ((ptr_cluster_extended_hub*)this)->root_hub;
	return (ptr_cluster_hub_root*)this;
}

ptr_cluster_hub_base* ptr_cluster_hub_base::push(void* inner_ptr, bool is_owner) {
	ptr_cluster_hub_base* place = root();

	if(place->ptr == inner_ptr) {
		place->incr();
		return place;
	}
	ptr_cluster_hub_base* it = place->next;

	while(it) {
		if(place->ptr == inner_ptr) {
			place->incr();
			return place;
		}
		place = it;
		it = it->next;
	}
	return place->next = new ptr_cluster_extended_hub(inner_ptr, root(), is_owner);
}

void ptr_cluster_hub_root::destroyObj() {
	if(ptr) {
		if(flags & ptr::flags::is_class) {
			auto en_ptr = base_ref;
			if(en_ptr)
				if(en_ptr->allocated)
					if(destoy_func)
						destoy_func(ptr);
		} else {
			if(destoy_func) destoy_func(ptr);
		}
	}
	ptr = nullptr;
}

NSP_BETTERCPP_END


