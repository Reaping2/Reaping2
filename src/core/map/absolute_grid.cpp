#include "absolute_grid.h"

namespace map {

AbsoluteGrid::AbsoluteGrid( int32_t Id )
    : IGrid( Id )
{

}


void AbsoluteGrid::Update( double DeltaTime )
{

}

void AbsoluteGrid::SetMousePosition( double x, double y )
{
    IGrid::SetMousePosition( x, y );
    mProcessedPosition = mMousePosition;
}


} // namespace map

