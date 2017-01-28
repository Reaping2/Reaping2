#include "counter.h"

namespace render {

Counts_t count( GeneratorFunc f )
{
    Counts_t Counts;
    RenderBatch Batch{-1,-1,-1,-1,-1};
    RenderBatch NextBatch{-1,-1,-1,-1,-1};
    size_t Count = 0;
    size_t Start = 0;
    while( f ( NextBatch ) )
    {
        if( Batch != NextBatch )
        {
            if( Count )
            {
                Counts.emplace_back( Batch, Start, Count );
            }
            Start += Count;
            Count = 0;
            Batch = NextBatch;
        }
        ++Count;
    }
    if( Count )
    {
        Counts.emplace_back( Batch, Start, Count );
    }
    return Counts;
}

}

