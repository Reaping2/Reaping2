#ifndef INCLUDED_NETWORK_MESSAGE_ORDER_H
#define INCLUDED_NETWORK_MESSAGE_ORDER_H
#include "boost/serialization/export.hpp"
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


    struct message_order
    {
        message_order()
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
            type=network::SetPickupContentMessage::GetType_static();
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

            type=HealOverTimeBuff::GetType_static();
            type=MoveSpeedBuff::GetType_static();
            type=MaxHealthBuff::GetType_static();
            type=AccuracyBuff::GetType_static();
            type=ArmorBuff::GetType_static();
            type=CloakBuff::GetType_static();

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
            type=platform::AutoId("pow_projectile");
        }
    } _msg_order;

    BOOST_CLASS_EXPORT_GUID(::core::SoldierProperties, "soldier_prop_base")    
    BOOST_CLASS_EXPORT_GUID(::core::ClientData, "client_data")    
    BOOST_CLASS_EXPORT_GUID(::ctf::ClientData, "ctf_client_data")    

    BOOST_CLASS_EXPORT_GUID(network::MyNameMessage,"my_name")
    BOOST_CLASS_EXPORT_GUID(network::ClientIdMessage, "client_id")
    BOOST_CLASS_EXPORT_GUID(network::LifecycleMessage, "lifecycle")
    BOOST_CLASS_EXPORT_GUID(network::CreateActorMessage, "create_actor")
    BOOST_CLASS_EXPORT_GUID(network::SetOwnershipMessage, "set_ownership")
    BOOST_CLASS_EXPORT_GUID(network::PositionMessage, "position")
    BOOST_CLASS_EXPORT_GUID(network::MoveMessage, "move")
    BOOST_CLASS_EXPORT_GUID(network::PlayerControllerMessage, "player_c")
    BOOST_CLASS_EXPORT_GUID(network::DamageTakenMessage, "damage_taken")
    BOOST_CLASS_EXPORT_GUID(network::OrientationMessage, "orientation")
    BOOST_CLASS_EXPORT_GUID(network::HeadingMessage, "heading")
    BOOST_CLASS_EXPORT_GUID(network::PickupMessage, "pickup")
    BOOST_CLASS_EXPORT_GUID(network::SetPickupContentMessage, "set_pickup")
    BOOST_CLASS_EXPORT_GUID(network::PingMessage, "ping")
    BOOST_CLASS_EXPORT_GUID(network::ReviveMessage, "revive")
    BOOST_CLASS_EXPORT_GUID(network::FlashMessage, "flash")
    BOOST_CLASS_EXPORT_GUID(network::HealTakenMessage, "heal_taken")
    BOOST_CLASS_EXPORT_GUID(network::SoldierPropertiesMessage, "soldier_properties")    
    BOOST_CLASS_EXPORT_GUID(network::ClientDatasMessage, "client_datas")
    BOOST_CLASS_EXPORT_GUID(network::ctf::ClientDatasMessage, "ctf_client_datas")
    BOOST_CLASS_EXPORT_GUID(network::HealthMessage, "health")
    BOOST_CLASS_EXPORT_GUID(network::AccuracyMessage, "accuracy")
    BOOST_CLASS_EXPORT_GUID(network::SetTeamMessage, "set_team")
    BOOST_CLASS_EXPORT_GUID(network::ctf::CtfScoreMessage, "ctf_score")
    BOOST_CLASS_EXPORT_GUID(network::ShowTextMessageMessage, "show_text_message")
    BOOST_CLASS_EXPORT_GUID(network::CollisionMessage, "collision")
    BOOST_CLASS_EXPORT_GUID(network::ShotMessage, "shot")
    BOOST_CLASS_EXPORT_GUID(network::KillScoreMessage, "kill_score")
    BOOST_CLASS_EXPORT_GUID(network::ClientScoreMessage, "client_score")
    BOOST_CLASS_EXPORT_GUID(network::ItemChangedMessage, "item_changed")
    BOOST_CLASS_EXPORT_GUID(network::CloakChangedMessage, "cloak_changed")
    BOOST_CLASS_EXPORT_GUID(network::BorderMessage, "border")


#endif//INCLUDED_NETWORK_MESSAGE_ORDER_H
