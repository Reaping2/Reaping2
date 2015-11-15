#include "platform/i_platform.h"
#include "target_repo.h"
#include "wall_target.h"

using platform::AutoId;

namespace map {

DefaultTarget const TargetRepo::mDefault = DefaultTarget();

TargetRepo::TargetRepo()
    : Repository<ITarget>(mDefault)
{
    int32_t id=AutoId("wall");
    mElements.insert(id, new WallTarget(AutoId("wall")));
}


void DefaultTarget::Update(double DeltaTime)
{

}

DefaultTarget::DefaultTarget()
    : ITarget(-1)
{

}

} // namespace map

