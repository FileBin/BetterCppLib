#pragma once

#include "bits/stdc++.h"
#include <typeindex>
#include <type_traits>

#ifdef _MSC_VER
#include "windows.h"
#include "Dbghelp.h"
#pragma comment(lib, "Dbghelp.lib")
#undef min
#undef max
#undef GetObject
#endif

#include "macros.h"
#include "namespaces.h"

#include "Core.hpp"

#ifdef INCLUDE_BETTERCPP_OBJECTS_CORE_HPP_

#include "Objects/Pointers.hpp"

NSP_BETTERCPP_BEGIN

typedef RefPtr<RootType> pRootType;

NSP_BETTERCPP_END

#include "Interfaces/Interfaces.hpp"
#include "Objects/Objects.hpp"

#include "Functions.hpp"

#endif
