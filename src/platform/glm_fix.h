#ifndef INCLUDED_PLATFORM_GLM_FIX_H
#define INCLUDED_PLATFORM_GLM_FIX_H

#if defined(_MSC_VER) && _MSC_VER <= 1400
// vs2005 sux.
#include <emmintrin.h>
#include <xmmintrin.h>

#define _mm_castsi128_ps(v) (*( reinterpret_cast<__m128*>((void*)&v) ) )
#define _mm_castps_si128(v) (*( reinterpret_cast<__m128i*>((void*)&v) ) )
#endif// defined(_MSC_VER) && _MSC_VER <= 1400

#endif//INCLUDED_PLATFORM_GLM_FIX_H
