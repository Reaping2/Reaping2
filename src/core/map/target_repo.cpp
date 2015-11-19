#include "platform/i_platform.h"
#include "target_repo.h"
#include "wall_target.h"
#include "ctf_soldier_spawn_target.h"
#include "../ctf_program_state.h"

using platform::AutoId;

namespace map {

DefaultTarget const TargetRepo::mDefault = DefaultTarget();

TargetRepo::TargetRepo()
    : Repository<ITarget>(mDefault)
{
    int32_t id=AutoId("wall");
    mElements.insert(id, new WallTarget(AutoId("wall"),AutoId("wall"),AutoId("wall")));
    id=AutoId("wall_small");
    mElements.insert(id, new WallTarget(AutoId("wall_small"),AutoId("wall_small"),AutoId("wall_small")));
    id=AutoId("ctf_soldier_spawn_blue");
    mElements.insert(id, new CtfSoldierSpawnTarget(id,AutoId("ctf_player"),Team::Blue));
    id=AutoId("ctf_soldier_spawn_red");
    mElements.insert(id, new CtfSoldierSpawnTarget(id,AutoId("ctf_player"),Team::Red));
}


void DefaultTarget::Update(double DeltaTime)
{

}

DefaultTarget::DefaultTarget()
    : ITarget(-1)
{

}

void DefaultTarget::PutTarget(glm::vec2 position)
{

}

} // namespace map

