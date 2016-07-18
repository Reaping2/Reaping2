#include "platform/i_platform.h"
#include "map_element_factory.h"
#include "link_map_element.h"
#include "map_start_map_element.h"
#include "spawn_soldiers_map_element.h"
#include "soldier_spawn_point_map_element.h"
#include "ctf_soldier_spawn_point_map_element.h"
#include "ctf_spawn_soldiers_map_element.h"
#include "ctf_flag_spawn_point_map_element.h"
#include "ctf_spawn_flags_map_element.h"
#include "spawn_actor_map_element.h"
#include "respawn_actor_map_element.h"
#include "soldier_auto_revive_map_element.h"
#include "level_generator/level_generator_map_element.h"
#include "level_generator/level_generated_map_element.h"
#include "group_map_element.h"

using platform::AutoId;
namespace map {

MapElementFactory::MapElementFactory()
{
    Bind( AutoId( "default" ), &CreateMapElement<DefaultMapElement> );
    SetDefault( AutoId( "default" ) );

    Bind( AutoId( "link" ), &CreateMapElement<LinkMapElement> );
    Bind( AutoId( "map_start" ), &CreateMapElement<MapStartMapElement> );
    Bind( AutoId( "spawn_soldiers" ), &CreateMapElement<SpawnSoldiersMapElement> );
    Bind( AutoId( "soldier_spawn_point" ), &CreateMapElement<SoldierSpawnPointMapElement> );
    Bind( AutoId( "spawn_actor" ), &CreateMapElement<SpawnActorMapElement> );
    Bind( AutoId( "respawn_actor" ), &CreateMapElement<RespawnActorMapElement> );
    Bind( AutoId( "ctf_soldier_spawn_point" ), &CreateMapElement<ctf::CtfSoldierSpawnPointMapElement> );
    Bind( AutoId( "ctf_spawn_soldiers" ), &CreateMapElement<ctf::CtfSpawnSoldiersMapElement> );
    Bind( AutoId( "ctf_flag_spawn_point" ), &CreateMapElement<ctf::CtfFlagSpawnPointMapElement> );
    Bind( AutoId( "ctf_spawn_flags" ), &CreateMapElement<ctf::CtfSpawnFlagsMapElement> );
    Bind( AutoId( "soldier_auto_revive" ), &CreateMapElement<SoldierAutoReviveMapElement> );
    Bind( AutoId( "level_generator" ), &CreateMapElement<LevelGeneratorMapElement> );
    Bind( AutoId( "level_generated" ), &CreateMapElement<LevelGeneratedMapElement> );
    Bind( AutoId( "group" ), &CreateMapElement<GroupMapElement> );
}

} // namespace map

