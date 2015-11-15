#include "platform/i_platform.h"
#include "grid_repo.h"
#include "absolute_grid.h"

using platform::AutoId;

namespace map {

DefaultGrid const GridRepo::mDefault = DefaultGrid();

GridRepo::GridRepo()
    : Repository<IGrid>(mDefault)
{
    int32_t id=AutoId("absolute");
    mElements.insert(id, new AbsoluteGrid(AutoId("absolute")));
}


void DefaultGrid::Update(double DeltaTime)
{

}

DefaultGrid::DefaultGrid()
    : IGrid(-1)
{

}

} // namespace map

