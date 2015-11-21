#include "platform/i_platform.h"
#include "target_repo.h"
#include "wall_target.h"
#include "ctf_soldier_spawn_target.h"
#include "../ctf_program_state.h"
#include "pickup_target.h"
#include "flag_spawn_target.h"

using platform::AutoId;

namespace map {

DefaultTarget const TargetRepo::mDefault = DefaultTarget();

TargetRepo::TargetRepo()
    : Repository<ITarget>(mDefault)
{
    int32_t id=AutoId("wall");
    mElements.insert(id, new WallTarget(AutoId("wall"),AutoId("wall"),AutoId("wall")));
    id=AutoId("wall_small"); mElements.insert(id, new WallTarget(AutoId("wall_small"),AutoId("wall_small"),AutoId("wall_small")));
    id=AutoId("ctf_soldier_spawn_blue"); mElements.insert(id, new CtfSoldierSpawnTarget(id,AutoId("ctf_player"),Team::Blue));
    id=AutoId("ctf_soldier_spawn_red"); mElements.insert(id, new CtfSoldierSpawnTarget(id,AutoId("ctf_player"),Team::Red));

    id=AutoId("flash_normal_item"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("normal"),id));
    id=AutoId("grenade_normal_item"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("normal"),id));
    id=AutoId("pistol"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("weapon"),id));
    id=AutoId("plasma_gun"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("weapon"),id));
    id=AutoId("rocket_launcher"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("weapon"),id));
    id=AutoId("shotgun"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("weapon"),id));
    id=AutoId("heal_over_time_buff"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("buff"),id));
    id=AutoId("move_speed_buff"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("buff"),id));
    id=AutoId("accuracy_buff"); mElements.insert(id, new PickupTarget(AutoId("revive_pickup"),AutoId("buff"),id));
    id=AutoId("ctf_flag_spawn_blue"); mElements.insert(id, new FlagSpawnTarget(id,AutoId("platform"),Team::Blue));
    id=AutoId("ctf_flag_spawn_red"); mElements.insert(id, new FlagSpawnTarget(id,AutoId("platform"),Team::Red));

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

