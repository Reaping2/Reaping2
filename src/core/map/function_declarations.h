#ifndef INCLUDED_CORE_MAP_FUNCTION_DECLARATION_H
#define INCLUDED_CORE_MAP_FUNCTION_DECLARATION_H

#include "boost/function.hpp"

namespace map {

typedef boost::function<void( int32_t )> int_function_t;

} // namespace map

#endif//INCLUDED_CORE_MAP_FUNCTION_DECLARATION_H