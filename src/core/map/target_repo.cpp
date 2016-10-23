#include "platform/i_platform.h"
#include "target_repo.h"
#include "wall_target.h"
#include "ctf_soldier_spawn_target.h"
#include "../ctf_program_state.h"
#include "pickup_target.h"
#include "flag_spawn_target.h"
#include "soldier_spawn_target.h"

using platform::AutoId;

namespace map {

DefaultTarget const TargetRepo::mDefault = DefaultTarget();

TargetRepo::TargetRepo()
    : Repository<ITarget>( mDefault )
{
    int32_t id = AutoId( "wall" );
    mElements.insert( id, new WallTarget( AutoId( "wall" ), AutoId( "wall" ), AutoId( "wall" ) ) );
    id = AutoId( "wall_small" );
    mElements.insert( id, new WallTarget( AutoId( "wall_small" ), AutoId( "wall_small" ), AutoId( "wall_small" ) ) );
    id = AutoId( "stone_wall" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "water" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "concrete" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "rogue_grass200" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "rogue_wall50_background" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "rogue_wall100_background" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "rogue_wall100" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "rogue_wall50" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "guard" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "skull" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "grass_tile" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "end" );
    mElements.insert( id, new WallTarget( id, id, id ) );
    id = AutoId( "ctf_soldier_spawn_blue" );
    mElements.insert( id, new CtfSoldierSpawnTarget( id, AutoId( "ctf_player" ), Team::Blue ) );
    id = AutoId( "ctf_soldier_spawn_red" );
    mElements.insert( id, new CtfSoldierSpawnTarget( id, AutoId( "ctf_player" ), Team::Red ) );
    id = AutoId( "soldier_spawn" );
    mElements.insert( id, new SoldierSpawnTarget( id, AutoId( "player" ) ) );

    id = AutoId( "flash_normal_item" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "normal" ), id ) );
    id = AutoId( "grenade_normal_item" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "normal" ), id ) );
    id = AutoId( "blue_grenade_normal_item" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "normal" ), id ) );
    id = AutoId( "cloak_normal_item" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "normal" ), id ) );
    id = AutoId( "pistol" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "plasma_gun" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "ion_gun" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "gatling_gun" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "gauss_gun" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "rocket_launcher" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "shotgun" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "gauss_gun" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "weapon" ), id ) );
    id = AutoId( "HealOverTimeBuff" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "buff" ), id ) );
    id = AutoId( "MoveSpeedBuff" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "buff" ), id ) );
    id = AutoId( "AccuracyBuff" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "buff" ), id ) );
    id = AutoId( "ArmorBuff" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "buff" ), id ) );
    id = AutoId( "CloakBuff" );
    mElements.insert( id, new PickupTarget( AutoId( "revive_pickup" ), AutoId( "buff" ), id ) );
    id = AutoId( "ctf_flag_spawn_blue" );
    mElements.insert( id, new FlagSpawnTarget( id, AutoId( "platform" ), Team::Blue ) );
    id = AutoId( "ctf_flag_spawn_red" );
    mElements.insert( id, new FlagSpawnTarget( id, AutoId( "platform" ), Team::Red ) );

}


void DefaultTarget::Update( double DeltaTime )
{

}

DefaultTarget::DefaultTarget()
    : ITarget( -1 )
{

}

void DefaultTarget::PutTarget( glm::vec2 position )
{

}

} // namespace map

