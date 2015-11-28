#include "i_render.h"
#include "sprite_phase.h"

SpritePhase::SpritePhase( GLuint i/*=0*/, GLfloat t/*=0.f*/, GLfloat l/*=0.f*/, GLfloat b/*=0.f*/, GLfloat r/*=0.f*/ )
    : TexId( i )
    , Top( t )
    , Left( l )
    , Bottom( b )
    , Right( r )
{
}

