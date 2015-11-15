#include "wall_target.h"

namespace map {

WallTarget::WallTarget(int32_t Id)
    : ITarget(Id)
    , mCursorId(AutoId("wall"))
    , mActorId(AutoId("wall"))
{

}

void WallTarget::Update(double DeltaTime)
{

}

int32_t WallTarget::GetCursorId() const
{
    return mCursorId;
}

int32_t WallTarget::GetActorId() const
{
    return mActorId;
}

} // namespace map
