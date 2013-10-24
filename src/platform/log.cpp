#include "i_platform.h"
#include <cstdio>
#include <cstdarg>

void Logger::Log( char const* format, ... )
{
	va_list arg;
	int done;

	va_start (arg, format);
	done = vfprintf (stderr, format, arg);
	va_end (arg);
}

