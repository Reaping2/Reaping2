#include "i_grid.h"

namespace map {

glm::vec2 IGrid::GetProcessedPosition()
{
    return mProcessedPosition;
}

void IGrid::SetCursorPosition(double x, double y)
{
    mCursorPosition.x=x;
    mCursorPosition.y=y;
}

IGrid::IGrid(int32_t Id)
    : mId(Id)
{

}

} // namespace map

