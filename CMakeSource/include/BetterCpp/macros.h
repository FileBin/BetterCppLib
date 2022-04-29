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
#define better_class_extends(ClassName, SuperClass) better_class_proto(ClassName) : SuperClass

#define better_static_class(ClassName) struct ClassName
#define better_class(ClassName) better_class_extends(ClassName, ::NSP_BETTERCPP::Object)
#define implements(...) , ##__VA_ARGS__

//interface
#define better_interface_proto(InterfaceName) struct InterfaceName
#define better_interface_implements(InterfaceName, ...) better_interface_proto(InterfaceName) : ##__VA_ARGS__
#define better_interface(InterfaceName) better_interface_implements(InterfaceName, ::NSP_BETTERCPP::Interface)

#define BETTER_OVERRIDE_CLASS_NAME(name) public: constexpr static char_cptr className = #name; private:

#define BETTER_OVERRIDE_TO_STRING(name) BETTER_OVERRIDE_CLASS_NAME(name) public: String toString() const override { return className; } private:

#define BETTER_OVERRIDE_BYTE_SIZE public: uint byteSize() const override { return sizeof(*this); } private:

#define BETTER_OVERRIDE_OBJECT_METHODS(ClassName)\
BETTER_OVERRIDE_TO_STRING(ClassName) BETTER_OVERRIDE_BYTE_SIZE

