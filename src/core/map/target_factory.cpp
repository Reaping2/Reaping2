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
    Bind( AutoId("wall"), &CreateITarget<WallTarget>);
    Bind( AutoId("wall_small"), &CreateITarget<WallTarget>);
    Bind( AutoId("stone_wall"), &CreateITarget<WallTarget>);
    Bind( AutoId("water"), &CreateITarget<WallTarget>);
    Bind( AutoId("concrete"), &CreateITarget<WallTarget>);
    Bind( AutoId("rogue_grass200"), &CreateITarget<WallTarget>);
    Bind( AutoId("rogue_wall50_background"), &CreateITarget<WallTarget>);
    Bind( AutoId("rogue_wall100_background"), &CreateITarget<WallTarget>);
    Bind( AutoId("rogue_wall100"), &CreateITarget<WallTarget>);
    Bind( AutoId("rogue_wall50"), &CreateITarget<WallTarget>);
    Bind( AutoId("guard"), &CreateITarget<WallTarget>);
    Bind( AutoId("skull"), &CreateITarget<WallTarget>);
    Bind( AutoId("grass_tile"), &CreateITarget<WallTarget>);
    Bind( AutoId("end"), &CreateITarget<WallTarget>);
    // ctf soldier spawn types
    Bind( AutoId("ctf_soldier_spawn_blue"), &CreateITarget<CtfSoldierSpawnTarget>);
    Bind( AutoId("ctf_soldier_spawn_red"), &CreateITarget<CtfSoldierSpawnTarget>);
    // soldier spawn types
    Bind( AutoId("soldier_spawn"), &CreateITarget<SoldierSpawnTarget>);
    // pickup types
    Bind( AutoId("revive_pickup"), &CreateITarget<PickupTarget>);
    // flag spawn types
    Bind( AutoId( "ctf_flag_spawn_blue" ), &CreateITarget<FlagSpawnTarget>);
    Bind( AutoId( "ctf_flag_spawn_red" ), &CreateITarget<FlagSpawnTarget>);

}

} // namespace map

