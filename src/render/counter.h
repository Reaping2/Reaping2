#pragma once
#ifndef INCLUDED_COUNTER_H
#define INCLUDED_COUNTER_H

#include <GL/glew.h>
#include <boost/function.hpp>
#include <vector>
#include "platform/rstdint.h"

namespace render {
struct CountByTexId
{
    GLuint TexId;
    size_t Start;
    size_t Count;
    CountByTexId( GLuint t, size_t s, size_t c ): TexId( t ), Start( s ), Count( c ) {}
};
typedef std::vector<CountByTexId> Counts_t;
typedef boost::function<bool( GLuint& TexId )> GeneratorFunc;
Counts_t count( GeneratorFunc f );
}

#endif // INCLUDED_COUNTER_H

