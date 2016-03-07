#pragma once
#ifndef INCLUDED_EXPORT_H
#define INCLUDED_EXPORT_H

#include "static_init.h"
#include <boost/serialization/export.hpp>

#define REAPING2_CLASS_EXPORT_KEY2( k, x, y ) \
    BOOST_CLASS_EXPORT_KEY2( x, y ) \
    FORCE_STATIC_INIT( k ) \
 
#define REAPING2_CLASS_EXPORT_IMPLEMENT( k, x ) \
    BOOST_CLASS_EXPORT_IMPLEMENT( x ) \
    FORCE_STATIC_INIT_IMPLEMENT( k ) \
 
#endif // INCLUDED_EXPORT_H

