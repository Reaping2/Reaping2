#include "platform/i_platform.h"
#include "network/message_handler_sub_system_factory.h"
#include "platform/auto_id.h"
#include "my_name_message_handler_sub_system.h"
#include "client_id_message_handler_sub_system.h"
#include "lifecycle_message_handler_sub_system.h"
#include "create_actor_message_handler_sub_system.h"
#include "position_message_handler_sub_system.h"
#include "move_message_handler_sub_system.h"
#include "player_controller_message.h"
#include "damage_taken_message.h"
#include "orientation_message.h"
#include "heading_message.h"
#include "pickup_message.h"
#include "ping_message.h"
#include "revive_message.h"
#include "flash_message.h"
#include "heal_taken_message.h"
#include "soldier_properties_message.h"
#include "client_datas_message.h"
#include "health_message.h"
#include "accuracy_message.h"
#include "set_ownership_message.h"
#include "ctf_client_datas_message.h"
#include "set_team_message.h"
#include "ctf_score_message.h"
#include "show_text_message_message.h"
#include "collision_message.h"
#include "shot_message.h"
#include "kill_score_message.h"
#include "client_score_message.h"
#include "item_changed_message.h"
#include "cloak_changed_message.h"
#include "border_message.h"
#include "sync_item_message.h"
#include "secs_to_revive_message.h"
#include "modify_audible_component_message.h"
#include "flag_state_changed_message.h"
#include "fade_out_message.h"
#include "gamemode_selected_message.h"
#include "team_switch_request_message.h"
#include "actor_list_message.h"
#include "rotate_message.h"
#include "data_checksum_message.h"
#include "map_start_message.h"
#include "map_load_message.h"
#include "suppress_message.h"
#include "waypoint_message.h"
#include "waypoints_data_message.h"
#include "dark_matter_message.h"

using platform::AutoId;
namespace network {

MessageHandlerSubSystemFactory::MessageHandlerSubSystemFactory()
{
    Bind( AutoId( "default_message_handler_sub_system" ), &CreateSubSystem<DefaultMessageHandlerSubSystem> );
    SetDefault( AutoId( "default_message_handler_sub_system" ) );

    Bind( AutoId( "my_name_message_handler_sub_system" ), &CreateSubSystem<MyNameMessageHandlerSubSystem> );
    Bind( AutoId( "client_id_message_handler_sub_system" ), &CreateSubSystem<ClientIdMessageHandlerSubSystem> );
    Bind( AutoId( "lifecycle_message_handler_sub_system" ), &CreateSubSystem<LifecycleMessageHandlerSubSystem> );
    Bind( AutoId( "create_actor_message_handler_sub_system" ), &CreateSubSystem<CreateActorMessageHandlerSubSystem> );
    Bind( AutoId( "set_ownership_message_handler_sub_system" ), &CreateSubSystem<SetOwnershipMessageHandlerSubSystem> );
    Bind( AutoId( "position_message_handler_sub_system" ), &CreateSubSystem<PositionMessageHandlerSubSystem> );
    Bind( AutoId( "move_message_handler_sub_system" ), &CreateSubSystem<MoveMessageHandlerSubSystem> );
    Bind( AutoId( "player_controller_message_handler_sub_system" ), &CreateSubSystem<PlayerControllerMessageHandlerSubSystem> );
    Bind( AutoId( "damage_taken_message_handler_sub_system" ), &CreateSubSystem<DamageTakenMessageHandlerSubSystem> );
    Bind( AutoId( "heal_taken_message_handler_sub_system" ), &CreateSubSystem<HealTakenMessageHandlerSubSystem> );
    Bind( AutoId( "orientation_message_handler_sub_system" ), &CreateSubSystem<OrientationMessageHandlerSubSystem> );
    Bind( AutoId( "heading_message_handler_sub_system" ), &CreateSubSystem<HeadingMessageHandlerSubSystem> );
    Bind( AutoId( "pickup_message_handler_sub_system" ), &CreateSubSystem<PickupMessageHandlerSubSystem> );
    Bind( AutoId( "ping_message_handler_sub_system" ), &CreateSubSystem<PingMessageHandlerSubSystem> );
    Bind( AutoId( "revive_message_handler_sub_system" ), &CreateSubSystem<ReviveMessageHandlerSubSystem> );
    Bind( AutoId( "flash_message_handler_sub_system" ), &CreateSubSystem<FlashMessageHandlerSubSystem> );
    Bind( AutoId( "soldier_properties_message_handler_sub_system" ), &CreateSubSystem<SoldierPropertiesMessageHandlerSubSystem> );
    Bind( AutoId( "client_datas_message_handler_sub_system" ), &CreateSubSystem<ClientDatasMessageHandlerSubSystem> );
    Bind( AutoId( "ctf_client_datas_message_handler_sub_system" ), &CreateSubSystem<ctf::ClientDatasMessageHandlerSubSystem> );
    Bind( AutoId( "health_message_handler_sub_system" ), &CreateSubSystem<HealthMessageHandlerSubSystem> );
    Bind( AutoId( "accuracy_message_handler_sub_system" ), &CreateSubSystem<AccuracyMessageHandlerSubSystem> );
    Bind( AutoId( "set_team_message_handler_sub_system" ), &CreateSubSystem<SetTeamMessageHandlerSubSystem> );
    Bind( AutoId( "ctf_score_message_handler_sub_system" ), &CreateSubSystem<ctf::CtfScoreMessageHandlerSubSystem> );
    Bind( AutoId( "show_text_message_message_handler_sub_system" ), &CreateSubSystem<ShowTextMessageMessageHandlerSubSystem> );
    Bind( AutoId( "collision_message_handler_sub_system" ), &CreateSubSystem<CollisionMessageHandlerSubSystem> );
    Bind( AutoId( "shot_message_handler_sub_system" ), &CreateSubSystem<ShotMessageHandlerSubSystem> );
    Bind( AutoId( "kill_score_message_handler_sub_system" ), &CreateSubSystem<KillScoreMessageHandlerSubSystem> );
    Bind( AutoId( "client_score_message_handler_sub_system" ), &CreateSubSystem<ClientScoreMessageHandlerSubSystem> );
    Bind( AutoId( "item_changed_message_handler_sub_system" ), &CreateSubSystem<ItemChangedMessageHandlerSubSystem> );
    Bind( AutoId( "cloak_changed_message_handler_sub_system" ), &CreateSubSystem<CloakChangedMessageHandlerSubSystem> );
    Bind( AutoId( "border_message_handler_sub_system" ), &CreateSubSystem<BorderMessageHandlerSubSystem> );
    Bind( AutoId( "sync_item_message_handler_sub_system" ), &CreateSubSystem<SyncItemMessageHandlerSubSystem> );
    Bind( AutoId( "secs_to_revive_message_handler_sub_system" ), &CreateSubSystem<SecsToReviveMessageHandlerSubSystem> );
    Bind( AutoId( "modify_audible_component_message_handler_sub_system" ), &CreateSubSystem<ModifyAudibleComponentMessageHandlerSubSystem> );
    Bind( AutoId( "flag_state_changed_message_handler_sub_system" ), &CreateSubSystem<FlagStateChangedMessageHandlerSubSystem> );
    Bind( AutoId( "fade_out_message_handler_sub_system" ), &CreateSubSystem<FadeOutMessageHandlerSubSystem> );
    Bind( AutoId( "gamemode_selected_message_handler_sub_system" ), &CreateSubSystem<GamemodeSelectedMessageHandlerSubSystem> );
    Bind( AutoId( "team_switch_request_message_handler_sub_system" ), &CreateSubSystem<TeamSwitchRequestMessageHandlerSubSystem> );
    Bind( AutoId( "actor_list_message_handler_sub_system" ), &CreateSubSystem<ActorListMessageHandlerSubSystem> );
    Bind( AutoId( "rotate_message_handler_sub_system" ), &CreateSubSystem<RotateMessageHandlerSubSystem> );
    Bind( AutoId( "data_checksum_message_handler_sub_system" ), &CreateSubSystem<DataChecksumMessageHandlerSubSystem>);
    Bind( AutoId( "map_start_message_handler_sub_system" ), &CreateSubSystem<MapStartMessageHandlerSubSystem> );
    Bind( AutoId( "map_load_message_handler_sub_system" ), &CreateSubSystem<MapLoadMessageHandlerSubSystem> );
    Bind( AutoId( "suppress_message_handler_sub_system" ), &CreateSubSystem<SuppressMessageHandlerSubSystem> );
    Bind( AutoId( "waypoint_message_handler_sub_system" ), &CreateSubSystem<WaypointMessageHandlerSubSystem> );
    Bind( AutoId( "waypoints_data_message_handler_sub_system" ), &CreateSubSystem<WaypointsDataMessageHandlerSubSystem> );
    Bind( AutoId( "dark_matter_message_handler_sub_system" ), &CreateSubSystem<DarkMatterMessageHandlerSubSystem> );
}

} // namespace network
