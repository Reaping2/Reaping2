#include "absolute_grid.h"

namespace map {

AbsoluteGrid::AbsoluteGrid(int32_t Id)
    : IGrid(Id)
{

}

    
void AbsoluteGrid::Update(double DeltaTime)
{

}

void AbsoluteGrid::SetCursorPosition(double x, double y)
{
    IGrid::SetCursorPosition(x,y);
    mProcessedPosition=mCursorPosition;
}


} // namespace map

