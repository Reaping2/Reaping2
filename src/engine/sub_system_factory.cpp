
#include "platform/i_platform.h"
#include "engine/sub_system_factory.h"
#include "engine/sub_system.h"
#include "platform/auto_id.h"
#include "engine/controllers/player_controller_sub_system.h"
#include "engine/controllers/random_controller_sub_system.h"
#include "engine/controllers/target_player_controller_sub_system.h"
#include "engine/controllers/pointer_target_controller_sub_system.h"
#include "engine/items/normal_item_sub_system.h"
#include "engine/items/weapon_item_sub_system.h"
#include "engine/items/plasma_gun_weapon_sub_system.h"
#include "engine/items/pistol_weapon_sub_system.h"
#include "engine/items/rocket_launcher_weapon_sub_system.h"
#include "engine/items/grenade_normal_item_sub_system.h"
#include "collisions/pickup_collision_sub_system.h"
#include "collisions/wall_collision_sub_system.h"
#include "collisions/shot_collision_sub_system.h"
#include "collisions/normal_collision_sub_system.h"
#include "collisions/aoe_collision_sub_system.h"
#include "collisions/bounce_collision_sub_system.h"
#include "items/flash_normal_item_sub_system.h"
#include "buffs_engine/heal_over_time_buff_sub_system.h"
#include "buffs_engine/move_speed_buff_sub_system.h"
#include "buffs_engine/max_health_buff_sub_system.h"
#include "buffs_engine/accuracy_buff_sub_system.h"
#include "collisions/flag_collision_sub_system.h"
#include "items/shotgun_weapon_sub_system.h"
#include "items/ion_gun_weapon_sub_system.h"
#include "buffs_engine/armor_buff_sub_system.h"
#include "buffs_engine/cloak_buff_sub_system.h"
#include "items/cloak_normal_item_sub_system.h"
#include "items/gatling_gun_weapon_sub_system.h"
#include "items/gauss_gun_weapon_sub_system.h"
#include "items/lucky_rocket_weapon_subsystem.h"
#include "items/blue_grenade_normal_item_sub_system.h"
#include "items/rusty_reaper_weapon_sub_system.h"
#include "controllers/guard_controller_sub_system.h"
#include "controllers/path_controller_sub_system.h"
#include "controllers/scripted_controller_sub_system.h"
#include "collisions/dark_matter_collision_sub_system.h"


using platform::AutoId;
namespace engine {

SubSystemFactory::SubSystemFactory()
{
    Bind( AutoId( "default_sub_system" ), &CreateSubSystem<DefaultSubSystem> );
    SetDefault( AutoId( "default_sub_system" ) );

    Bind( AutoId( "player_controller_sub_system" ), &CreateSubSystem<PlayerControllerSubSystem> );
    Bind( AutoId( "random_controller_sub_system" ), &CreateSubSystem<RandomControllerSubSystem> );
    Bind( AutoId( "target_player_controller_sub_system" ), &CreateSubSystem<TargetPlayerControllerSubSystem> );
    Bind( AutoId( "pointer_target_controller_sub_system" ), &CreateSubSystem<PointerTargetControllerSubSystem> );
    Bind( AutoId( "guard_controller_sub_system" ), &CreateSubSystem<GuardControllerSubSystem> );
    Bind( AutoId( "path_controller_sub_system" ), &CreateSubSystem<PathControllerSubSystem> );
    Bind( AutoId( "scripted_controller_sub_system" ), &CreateSubSystem<ScriptedControllerSubSystem> );

    Bind( AutoId( "normal_item_sub_system" ), &CreateSubSystem<NormalItemSubSystem> );
    Bind( AutoId( "flash_normal_item_sub_system" ), &CreateSubSystem<FlashNormalItemSubSystem> );
    Bind( AutoId( "grenade_normal_item_sub_system" ), &CreateSubSystem<GrenadeNormalItemSubSystem> );
    Bind( AutoId( "cloak_normal_item_sub_system" ), &CreateSubSystem<CloakNormalItemSubSystem> );
    Bind( AutoId( "blue_grenade_normal_item_sub_system" ), &CreateSubSystem<BlueGrenadeNormalItemSubSystem> );

    Bind( AutoId( "weapon_item_sub_system" ), &CreateSubSystem<WeaponItemSubSystem> );
    Bind( AutoId( "plasma_gun_weapon_sub_system" ), &CreateSubSystem<PlasmaGunWeaponSubSystem> );
    Bind( AutoId( "pistol_weapon_sub_system" ), &CreateSubSystem<PistolWeaponSubSystem> );
    Bind( AutoId( "shotgun_weapon_sub_system" ), &CreateSubSystem<ShotgunWeaponSubSystem> );
    Bind( AutoId( "rocket_launcher_weapon_sub_system" ), &CreateSubSystem<RocketLauncherWeaponSubSystem> );
    Bind( AutoId( "ion_gun_weapon_sub_system" ), &CreateSubSystem<IonGunWeaponSubSystem> );
    Bind( AutoId( "gatling_gun_weapon_sub_system" ), &CreateSubSystem<GatlingGunWeaponSubSystem> );
    Bind( AutoId( "gauss_gun_weapon_sub_system" ), &CreateSubSystem<GaussGunWeaponSubSystem> );
    Bind( AutoId( "lucky_rocket_weapon_sub_system" ), &CreateSubSystem<LuckyRocketWeaponSubSystem> );
    Bind( AutoId( "rusty_reaper_weapon_sub_system" ), &CreateSubSystem<RustyReaperWeaponSubSystem>);


    Bind( AutoId( "pickup_collision_sub_system" ), &CreateSubSystem<PickupCollisionSubSystem> );
    Bind( AutoId( "wall_collision_sub_system" ), &CreateSubSystem<WallCollisionSubSystem> );
    Bind( AutoId( "shot_collision_sub_system" ), &CreateSubSystem<ShotCollisionSubSystem> );
    Bind( AutoId( "normal_collision_sub_system" ), &CreateSubSystem<NormalCollisionSubSystem> );
    Bind( AutoId( "aoe_collision_sub_system" ), &CreateSubSystem<AoeCollisionSubSystem> );
    Bind( AutoId( "bounce_collision_sub_system" ), &CreateSubSystem<BounceCollisionSubSystem> );
    Bind( AutoId( "flag_collision_sub_system" ), &CreateSubSystem<ctf::FlagCollisionSubSystem> );
    Bind( AutoId( "dark_matter_collision_sub_system" ), &CreateSubSystem<DarkMatterCollisionSubSystem> );

    Bind( AutoId( "heal_over_time_buff_sub_system" ), &CreateSubSystem<HealOverTimeBuffSubSystem> );
    Bind( AutoId( "move_speed_buff_sub_system" ), &CreateSubSystem<MoveSpeedBuffSubSystem> );
    Bind( AutoId( "max_health_buff_sub_system" ), &CreateSubSystem<MaxHealthBuffSubSystem> );
    Bind( AutoId( "accuracy_buff_sub_system" ), &CreateSubSystem<AccuracyBuffSubSystem> );
    Bind( AutoId( "armor_buff_sub_system" ), &CreateSubSystem<ArmorBuffSubSystem> );
    Bind( AutoId( "cloak_buff_sub_system" ), &CreateSubSystem<CloakBuffSubSystem> );
}

} // namespace engine
