#include "platform/i_platform.h"
#include "target_factory.h"
#include "wall_target.h"
#include "ctf_soldier_spawn_target.h"
#include "soldier_spawn_target.h"
#include "pickup_target.h"
#include "flag_spawn_target.h"
#include "target_repo.h"

using platform::AutoId;
namespace map {

TargetFactory::TargetFactory()
{
    Bind( AutoId("default_i_target"), &CreateITarget<DefaultTarget> );
    SetDefault( AutoId("default_i_target") );
    // wall types
    Bind( AutoId("wall_target"), &CreateITarget<WallTarget>);
    // ctf soldier spawn types
    Bind( AutoId("ctf_soldier_spawn_target"), &CreateITarget<CtfSoldierSpawnTarget>);
    // soldier spawn types
    Bind( AutoId("soldier_spawn_target"), &CreateITarget<SoldierSpawnTarget>);
    // pickup types
    Bind( AutoId("revive_pickup"), &CreateITarget<PickupTarget>);
    // flag spawn types
    Bind( AutoId( "flag_spawn_target" ), &CreateITarget<FlagSpawnTarget>);

}

} // namespace map

