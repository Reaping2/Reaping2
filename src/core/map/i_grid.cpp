#include "i_grid.h"

namespace map {

glm::vec2 IGrid::GetProcessedPosition()
{
    return mProcessedPosition;
}

void IGrid::SetMousePosition(double x, double y)
{
    mMousePosition.x=x;
    mMousePosition.y=y;
}

IGrid::IGrid(int32_t Id)
    : mId(Id)
{

}

glm::vec2 IGrid::GetMousePosition()
{
    return mMousePosition;
}

Neighbors IGrid::GetNeighbors(glm::vec2 position, int32_t actorID)
{
    Neighbors r;
    return r;
}

IGrid::~IGrid()
{

}

} // namespace map

