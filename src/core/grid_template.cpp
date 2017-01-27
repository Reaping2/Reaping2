#include "platform/i_platform.h"
#include "core/grid_template.h"

CollPair::CollPair( Actor* a1, Actor* a2 ) : A1( a1 < a2 ? a1 : a2 )
    , A2( a1 < a2 ? a2 : a1 )
{
    assert( a1 != a2 );
}

bool CollPairCmp::operator()( CollPair const& a, CollPair const& b ) const
{
    return a.A1 < b.A1 || ( a.A1 == b.A1 && a.A2 < b.A2 );
}
