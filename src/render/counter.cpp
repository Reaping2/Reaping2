#include "counter.h"

namespace render {

Counts_t count( GeneratorFunc f )
{
    Counts_t Counts;
    GLuint TexId = -1, NextTexId = -1;
    size_t Count = 0;
    size_t Start = 0;
    while( f ( NextTexId ) )
    {
        if( TexId != NextTexId )
        {
            if( Count )
            {
                Counts.push_back( CountByTexId( TexId, Start, Count ) );
            }
            Start += Count;
            Count = 0;
            TexId = NextTexId;
        }
        ++Count;
    }
    if( Count )
    {
        Counts.push_back( CountByTexId( TexId, Start, Count ) );
    }
    return Counts;
}

}

