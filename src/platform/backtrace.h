#pragma once
#ifndef INCLUDED_BACKTRACE_H
#define INCLUDED_BACKTRACE_H

#include "static_init.h"
#include <iosfwd>

namespace platform {
std::ostream& backtrace( std::ostream& stream );
}

FORCE_STATIC_INIT( backtrace )

#endif // INCLUDED_BACKTRACE_H

