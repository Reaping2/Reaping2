#include "message_order.h"
#include "platform/init.h"

#include "core/actor.h"
#include "core/cloak_normal_item.h"
#include "core/acceleration_component.h"
#include "core/accuracy_component.h"
#include "core/aoe_collision_component.h"
#include "core/armor_component.h"
#include "core/attachable_component.h"
#include "core/audible_component.h"
#include "core/border_component.h"
#include "core/bounce_collision_component.h"
#include "core/collision_component.h"
#include "core/controller_component.h"
#include "core/detonate_on_hit_component.h"
#include "core/drop_on_death_component.h"
#include "core/emitter_component.h"
#include "core/explode_on_death_component.h"
#include "core/explode_on_hit_component.h"
#include "core/explosion_component.h"
#include "core/fade_out_component.h"
#include "core/flag_carrier_component.h"
#include "core/flag_collision_component.h"
#include "core/flag_receiver_component.h"
#include "core/health_component.h"
#include "core/heat_source_component.h"
#include "core/inventory_component.h"
#include "core/kill_score_on_death_component.h"
#include "core/listen_child_death_component.h"
#include "core/move_component.h"
#include "core/notify_parent_on_death_component.h"
#include "core/owner_component.h"
#include "core/pickup_collision_component.h"
#include "core/player_controller_component.h"
#include "core/pointer_target_controller_component.h"
#include "core/position_component.h"
#include "core/random_controller_component.h"
#include "core/remove_on_death_component.h"
#include "core/renderable_component.h"
#include "core/score_on_death_component.h"
#include "core/shot_collision_component.h"
#include "core/stop_on_death_component.h"
#include "core/target_holder_component.h"
#include "core/target_player_controller_component.h"
#include "core/team_component.h"
#include "core/wall_collision_component.h"
#include "core/water_collision_component.h"
#include "network/my_name_message.h"
#include "network/client_id_message.h"
#include "network/lifecycle_message.h"
#include "create_actor_message.h"
#include "set_ownership_message.h"
#include "position_message.h"
#include "platform/auto_id.h"
#include "move_message.h"
#include "player_controller_message.h"
#include "orientation_message.h"
#include "heading_message.h"
#include "pickup_message.h"
#include "ping_message.h"
#include "revive_message.h"
#include "heal_taken_message.h"
#include "core/buffs/heal_over_time_buff.h"
#include "core/buffs/move_speed_buff.h"
#include "soldier_properties_message.h"
#include "core/soldier_properties.h"
#include "core/program_state.h"
#include "health_message.h"
#include "damage_taken_message.h"
#include "core/buffs/max_health_buff.h"
#include "client_datas_message.h"
#include "accuracy_message.h"
#include "flash_message.h"
#include "core/buffs/accuracy_buff.h"
#include "core/ctf_program_state.h"
#include "set_team_message.h"
#include "ctf_client_datas_message.h"
#include "ctf_score_message.h"
#include "show_text_message_message.h"
#include "collision_message.h"
#include "shot_message.h"
#include "kill_score_message.h"
#include "client_score_message.h"
#include "item_changed_message.h"
#include "cloak_changed_message.h"
#include "core/buffs/armor_buff.h"
#include "core/buffs/cloak_buff.h"
#include "border_message.h"
#include "secs_to_revive_message.h"
#include "flag_state_changed_message.h"
#include "platform/rstdint.h"
#include "core/cloak_component.h"
#include "core/component.h"
#include "core/weapon.h"
#include "core/normal_item.h"
#include "core/buffs/buff_holder_component.h"
#include "core/gatling_gun.h"
#include "core/gauss_gun.h"
#include "core/ion_gun.h"
#include "core/pistol.h"
#include "core/rocket_launcher.h"
#include "core/cloak_normal_item.h"
#include "core/lucky_rocket.h"
#include "core/plasma_gun.h"
#include "core/blue_grenade_normal_item.h"
#include "core/flash_normal_item.h"
#include "core/grenade_normal_item.h"
#include "core/shotgun.h"
#include "actor_list_message.h"
#include "network/fade_out_message.h"
#include "network/sync_item_message.h"
#include "network/modify_audible_component_message.h"
#include "gamemode_selected_message.h"
#include "team_switch_request_message.h"

namespace network {
bool instantiateMessageOrder()
{
    volatile bool rv = true;
    return rv;
}
}
namespace {
void InitMessageOrder()
{
    int32_t type;
    type=network::ClientIdMessage::GetType_static();
    type=network::MyNameMessage::GetType_static();
    type=network::LifecycleMessage::GetType_static();
    type=network::CreateActorMessage::GetType_static();
    type=network::SetOwnershipMessage::GetType_static();
    type=network::PositionMessage::GetType_static();
    type=network::MoveMessage::GetType_static();
    type=network::PlayerControllerMessage::GetType_static();
    type=network::DamageTakenMessage::GetType_static();
    type=network::OrientationMessage::GetType_static();
    type=network::HeadingMessage::GetType_static();
    type=network::PickupMessage::GetType_static();
    type=network::PingMessage::GetType_static();
    type=network::ReviveMessage::GetType_static();
    type=network::FlashMessage::GetType_static();
    type=network::HealTakenMessage::GetType_static();
    type=network::SoldierPropertiesMessage::GetType_static();
    type=network::ClientDatasMessage::GetType_static();
    type=network::ctf::ClientDatasMessage::GetType_static();
    type=network::ctf::CtfScoreMessage::GetType_static();
    type=network::HealthMessage::GetType_static();
    type=network::AccuracyMessage::GetType_static();
    type=network::SetTeamMessage::GetType_static();
    type=network::ShowTextMessageMessage::GetType_static();
    type=network::CollisionMessage::GetType_static();
    type=network::ShotMessage::GetType_static();
    type=network::KillScoreMessage::GetType_static();
    type=network::ClientScoreMessage::GetType_static();
    type=network::ItemChangedMessage::GetType_static();
    type=network::CloakChangedMessage::GetType_static();
    type=network::BorderMessage::GetType_static();
    type=network::SyncItemMessage::GetType_static();
    type=network::SecsToReviveMessage::GetType_static();
    type=network::ModifyAudibleComponentMessage::GetType_static();
    type=network::FlagStateChangedMessage::GetType_static();
    type=network::FadeOutMessage::GetType_static();
    type=network::GamemodeSelectedMessage::GetType_static();
    type=network::TeamSwitchRequestMessage::GetType_static();
    type=network::ActorListMessage::GetType_static();

    type=IControllerComponent::GetType_static();
    type=PlayerControllerComponent::GetType_static();
    type=platform::AutoId("player_controller_component");
    type=IAccelerationComponent::GetType_static();
    type=IAccuracyComponent::GetType_static();
    type=IArmorComponent::GetType_static();
    type=ICloakComponent::GetType_static();
    type=ctf::IAttachableComponent::GetType_static();
    type=IAudibleComponent::GetType_static();
    type=IBorderComponent::GetType_static();
    type=IBuffHolderComponent::GetType_static();
    type=ICollisionComponent::GetType_static();
    type=IControllerComponent::GetType_static();
    type=IDetonateOnHitComponent::GetType_static();
    type=IDropOnDeathComponent::GetType_static();
    type=IEmitterComponent::GetType_static();
    type=IExplodeOnDeathComponent::GetType_static();
    type=IExplodeOnHitComponent::GetType_static();
    type=IExplosionComponent::GetType_static();
    type=IFadeOutComponent::GetType_static();
    type=ctf::IFlagCarrierComponent::GetType_static();
    type=ctf::IFlagReceiverComponent::GetType_static();
    type=IHealthComponent::GetType_static();
    type=IHeatSourceComponent::GetType_static();
    type=IInventoryComponent::GetType_static();
    type=IKillScoreOnDeathComponent::GetType_static();
    type=IListenChildDeathComponent::GetType_static();
    type=IMoveComponent::GetType_static();
    type=INotifyParentOnDeathComponent::GetType_static();
    type=IOwnerComponent::GetType_static();
    type=IPositionComponent::GetType_static();
    type=IRemoveOnDeathComponent::GetType_static();
    type=IRenderableComponent::GetType_static();
    type=IScoreOnDeathComponent::GetType_static();
    type=IStopOnDeathComponent::GetType_static();
    type=ITargetHolderComponent::GetType_static();
    type=ITeamComponent::GetType_static();

    type=platform::AutoId("i_acceleration_component");
    type=platform::AutoId("i_accuracy_component");
    type=platform::AutoId("i_armor_component");
    type=platform::AutoId("i_cloak_component");
    type=platform::AutoId("i_attachable_component");
    type=platform::AutoId("i_audible_component");
    type=platform::AutoId("i_border_component");
    type=platform::AutoId("buff_holder_component");
    type=platform::AutoId("i_collision_component");
    type=platform::AutoId("i_controller_component");
    type=platform::AutoId("i_detonate_on_hit_component");
    type=platform::AutoId("i_drop_on_death_component");
    type=platform::AutoId("i_emitter_component");
    type=platform::AutoId("i_explode_on_death_component");
    type=platform::AutoId("i_explode_on_hit_component");
    type=platform::AutoId("i_explosion_component");
    type=platform::AutoId("i_fade_out_component");
    type=platform::AutoId("i_flag_carrier_component");
    type=platform::AutoId("i_flag_receiver_component");
    type=platform::AutoId("i_health_component");
    type=platform::AutoId("i_heat_source_component");
    type=platform::AutoId("i_inventory_component");
    type=platform::AutoId("i_kill_score_on_death_component");
    type=platform::AutoId("i_listen_child_death_component");
    type=platform::AutoId("i_move_component");
    type=platform::AutoId("i_notify_parent_on_death_component");
    type=platform::AutoId("i_owner_component");
    type=platform::AutoId("i_position_component");
    type=platform::AutoId("i_remove_on_death_component");
    type=platform::AutoId("i_renderable_component");
    type=platform::AutoId("i_score_on_death_component");
    type=platform::AutoId("i_stop_on_death_component");
    type=platform::AutoId("i_target_holder_component");
    type=platform::AutoId("i_team_component");

    type=platform::AutoId("collision_component");
    type=platform::AutoId("aoe_collision_component");
    type=platform::AutoId("bounce_collision_component");
    type=platform::AutoId("collision_component");
    type=platform::AutoId("flag_collision_component");
    type=platform::AutoId("pickup_collision_component");
    type=platform::AutoId("shot_collision_component");
    type=platform::AutoId("wall_collision_component");
    type=platform::AutoId("water_collision_component");


    type=HealOverTimeBuff::GetType_static();
    type=MoveSpeedBuff::GetType_static();
    type=MaxHealthBuff::GetType_static();
    type=AccuracyBuff::GetType_static();
    type=ArmorBuff::GetType_static();
    type=CloakBuff::GetType_static();

    type=platform::AutoId("weapon");
    type=platform::AutoId("player");
    type=platform::AutoId("ctf_player");
    type=platform::AutoId("flag");
    type=platform::AutoId("platform");
    type=platform::AutoId("plasma_gun");
    type=platform::AutoId("plasma_shot");
    type=platform::AutoId("ion_gun");
    type=platform::AutoId("ion_gun_projectile");
    type=platform::AutoId("ion_gun_alt_projectile");
    type=platform::AutoId("ion_gun_explode_projectile");
    type=platform::AutoId("spider1");
    type=platform::AutoId("spider2");
    type=platform::AutoId("spider1target");
    type=platform::AutoId("spider2target");
    type=platform::AutoId("pickup");
    type=platform::AutoId("pistol");
    type=platform::AutoId("pistol_shot");
    type=platform::AutoId("gauss_gun");
    type=platform::AutoId("gauss_shot");
    type=platform::AutoId("gauss_alt_shot");
    type=platform::AutoId("gatling_gun");
    type=platform::AutoId("gatling_gun_projectile");
    type=platform::AutoId("gatling_gun_alt_projectile");
    type=platform::AutoId("rocket_launcher");
    type=platform::AutoId("rocket_launcher_primary");
    type=platform::AutoId("rocket_launcher_secondary");
    type=platform::AutoId("rocket_launcher_projectile");
    type=platform::AutoId("shotgun_projectile");
    type=platform::AutoId("shotgun_alt_projectile");
    type=platform::AutoId("shotgun_alt2_projectile");
    type=platform::AutoId("shotgun");
    type=platform::AutoId("rocket_launcher_target_projectile");
    type=platform::AutoId("laser_pointer_projectile");
    type=platform::AutoId("grenade_projectile");
    type=platform::AutoId("blue_grenade_projectile");
    type=platform::AutoId("pow_projectile");
    type=platform::AutoId("lucky_rocket");
    type=platform::AutoId("lucky_rocket_projectile");
    type=platform::AutoId("lucky_rocket_alt_projectile");
    type=platform::AutoId("lucky_rocket_explosion_projectile");
}
}

BOOST_CLASS_EXPORT_GUID(BuffHolderComponent,"buff_holder_component")
BOOST_CLASS_EXPORT_GUID(ArmorBuff, "armor_buff")
BOOST_CLASS_EXPORT_GUID(AccuracyBuff, "accuracy_buff")
BOOST_CLASS_EXPORT_GUID(CloakBuff, "cloak_buff")
BOOST_CLASS_EXPORT_GUID(HealOverTimeBuff, "heal_over_time_buff")
BOOST_CLASS_EXPORT_GUID(MaxHealthBuff, "max_health_buff")
BOOST_CLASS_EXPORT_GUID(MoveSpeedBuff, "move_speed_buff")
BOOST_CLASS_EXPORT_GUID(IBuffHolderComponent,"i_buff_holder_component")

REGISTER_INIT( MessageOrder, InitMessageOrder );

