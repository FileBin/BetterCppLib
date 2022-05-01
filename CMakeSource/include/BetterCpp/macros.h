/*
 * macros.h
 *
 *  Created on: Apr 29, 2022
 *      Author: root
 */

#include "namespaces.h"

//keywords
//class
#define better_class_proto(ClassName) class ClassName
#define better_class_extends(ClassName, SuperClass) better_class_proto(ClassName) : public SuperClass

#define better_static_class(ClassName) struct ClassName
#define better_class(ClassName) better_class_extends(ClassName, ::NSP_BETTERCPP::Object_CRTP<ClassName>)
#define better_template_class(ClassName, ...) better_class_extends(ClassName, ::NSP_BETTERCPP::Object_CRTP<ClassName<__VA_ARGS__>>)
#define better_implements(...) , __VA_ARGS__

#define const_ref(v) const v&

//interface
#define better_interface_proto(InterfaceName) struct InterfaceName
#define better_interface(InterfaceName) better_interface_proto(InterfaceName) : public ::NSP_BETTERCPP::Interface

#define foreach(coll)  for(auto enumerator = coll.getEnumeratorT(); enumerator->next(); )
#define foreach_elem (enumerator->currentT())

