#include "platform/i_platform.h"
#include "platform/init.h"
#include "engine/system_factory.h"
#include "engine/system.h"
#include "engine/collision_system.h"
#include "engine/activity_system.h"
#include "platform/auto_id.h"
#include "input/keyboard.h"
#include "engine/controllers/player_controller_sub_system.h"
#include "engine/controllers/random_controller_sub_system.h"
#include "engine/controllers/target_player_controller_sub_system.h"
#include "engine/controllers/controller_system.h"
#include "engine/items/inventory_system.h"
#include "engine/drop_on_death_system.h"
#include "engine/health_system.h"
#include "engine/remove_on_death_system.h"
#include "engine/explode_on_death_system.h"
#include "engine/fade_out_system.h"
#include "engine/move_system.h"
#include "engine/acceleration_system.h"
#include "input/mouse.h"
#include "input/controller_adapter_system.h"
#include "main/window.h"
#include "platform/timer.h"
#include "engine/frame_counter_system.h"
#include "render/renderer.h"
#include "network/server_system.h"
#include "network/client_system.h"
#include "network/message_handler_sub_system_holder.h"
#include "network/lifecycle_sender_system.h"
#include "network/position_message_sender_system.h"
#include "network/move_message_sender_system.h"
#include "network/player_controller_message.h"
#include "network/create_actor_message_sender_system.h"
#include "network/damage_taken_message.h"
#include "network/orientation_message.h"
#include "network/heading_message.h"
#include "network/pickup_message.h"
#include "network/ping_message.h"
#include "network/revive_message.h"
#include "network/gamemode_selected_message.h"
#include "explosion_system.h"
#include "stop_on_death_system.h"
#include "target_holder_system.h"
#include "removed_actors_system.h"
#include "listen_child_death_system.h"
#include "notify_parent_on_death_system.h"
#include "network/flash_message.h"
#include "engine/buffs_engine/buff_holder_system.h"
#include "network/heal_taken_message.h"
#include "soldier_properties_system.h"
#include "network/soldier_properties_message.h"
#include "network/client_datas_message.h"
#include "network/health_message.h"
#include "network/accuracy_message.h"
#include "local_system.h"
#include "soldier_spawn_system.h"
#include "network/set_ownership_message.h"
#include "core/map/map_system.h"
#include "core/map/link_map_element_system.h"
#include "core/map/map_start_map_element_system.h"
#include "core/map/spawn_soldiers_map_element_system.h"
#include "core/map/soldier_spawn_point_map_element_system.h"
#include "core/free_for_all_game_mode_system.h"
#include "core/capture_the_flag_game_mode_system.h"
#include "core/map/ctf_soldier_spawn_point_map_element_system.h"
#include "core/map/ctf_spawn_soldiers_map_element_system.h"
#include "ctf_soldier_spawn_system.h"
#include "network/set_team_message.h"
#include "core/map/ctf_spawn_flags_map_element_system.h"
#include "attachable_system.h"
#include "flag_spawn_system.h"
#include "network/ctf_score_message.h"
#include "score_on_death_system.h"
#include "show_text_system.h"
#include "network/show_text_message_message.h"
#include "core/map/spawn_actor_map_element_system.h"
#include "network/collision_message.h"
#include "core/map/respawn_actor_map_element_system.h"
#include "core/map/editor_system.h"
#include "core/map/editor_target_system.h"
#include "core/map/editor_grid_system.h"
#include "core/map/editor_brush_system.h"
#include "core/map/editor_soldier_spawn_system.h"
#include "network/shot_message.h"
#include "network/kill_score_message.h"
#include "kill_score_on_death_system.h"
#include "kill_score_system.h"
#include "network/client_score_message.h"
#include "leaderboard_system.h"
#include "network/item_changed_message.h"
#include "detonate_on_hit_system.h"
#include "explode_on_hit_system.h"
#include "armor_system.h"
#include "cloak_system.h"
#include "network/cloak_changed_message.h"
#include "network/border_message.h"
#include "network/ctf_client_list_displaying_system.h"
#include "network/ctf_client_list_handling_system.h"
#include "network/ffa_client_list_system.h"
#include "network/ctf_client_datas_message.h"
#include "network/sync_item_message.h"
#include "audio/audio_system.h"
#include "input/input_system.h"
#include "input/keyboard_adapter_system.h"
#include "input/mouse_adapter_system.h"
#include "soldier_auto_revive_system.h"
#include "core/map/soldier_auto_revive_map_element_system.h"
#include "network/secs_to_revive_message.h"
#include "network/modify_audible_component_message.h"
#include "network/flag_state_changed_message.h"
#include "network/fade_out_message.h"
#include "network/waiting_start_system.h"
#include "network/team_switch_request_message.h"
#include "network/actor_list_message.h"
#include "remove_components_on_death_system.h"
#include "player_model_system.h"
#include "rotate_component_system.h"
#include "network/rotate_message.h"
#include "network/data_checksum_message.h"
#include "core/rogue_game_mode_system.h"
#include "core/map/level_generator/level_generator_map_element_system.h"
#include "core/map/level_generator/level_generated_map_element_system.h"
#include "core/map/group_map_element_system.h"
#include "core/map/room_editor_system.h"
#include "core/map/editor_layer_system.h"
#include "core/map/editor_actor_system.h"
#include "core/map/room_cell_editor_system.h"
#include "core/map/editor_select_system.h"
#include "core/map/editor_group_system.h"
#include "core/map/editor_visiblity_system.h"
#include "core/map/editor_renderable_layer_system.h"
#include "core/map/property_editor_system.h"
#include "core/map/room_start_editor_system.h"
#include "core/map/room_end_editor_system.h"
#include "core/map/spawn_editor_system.h"
#include "core/map/cell_entrance_editor_system.h"
#include "core/map/room_plain_property_editor_system.h"
#include "level_end_system.h"
#include "recreate_borders_map_element_system.h"
#include "core/randomize_sprite_system.h"
#include "path_system.h"
#include "core/level_selection_system.h"
#include "pulse_system.h"
#include "worm_head_system.h"
#include "worm_body_system.h"

using platform::AutoId;
namespace engine {

SystemFactory::SystemFactory()
{

}

void SystemFactory::Init()
{
    Bind( AutoId( "default_system" ), &CreateSystem<DefaultSystem> );
    SetDefault( AutoId( "default_system" ) );

    Bind( AutoId( "window_system" ), &CreateSystem<WindowSystem> );
    Bind( AutoId( "timer_server_system" ), &CreateSystem<TimerServerSystem> );
    Bind( AutoId( "frame_counter_system" ), &CreateSystem<FrameCounterSystem> );
    Bind( AutoId( "renderer_system" ), &CreateSystem<RendererSystem> );
    Bind( AutoId( "leaderboard_system" ), &CreateSystem<engine::LeaderboardSystem> );
    Bind( AutoId( "randomize_sprite_system" ), &CreateSystem<engine::RandomizeSpriteSystem> );
    Bind( AutoId( "path_system" ), &CreateSystem<engine::path::PathSystem> );
    Bind( AutoId( "pulse_system" ), &CreateSystem<engine::PulseSystem> );

    Bind( AutoId( "collision_system" ), &CreateSystem<CollisionSystem> );
    Bind( AutoId( "activity_system" ), &CreateSystem<ActivitySystem> );

    Bind( AutoId( "keyboard_system" ), &CreateSystem<KeyboardSystem> );
    Bind( AutoId( "mouse_system" ), &CreateSystem<MouseSystem> );
    Bind( AutoId( "input_system" ), &CreateSystem<engine::InputSystem> );
    Bind( AutoId( "keyboard_adapter_system" ), &CreateSystem<engine::KeyboardAdapterSystem> );
    Bind( AutoId( "mouse_adapter_system" ), &CreateSystem<engine::MouseAdapterSystem> );
    Bind( AutoId( "controller_adapter_system" ), &CreateSystem<engine::ControllerAdapterSystem> );

    Bind( AutoId( "controller_system" ), &CreateSystem<ControllerSystem> );
    Bind( AutoId( "inventory_system" ), &CreateSystem<InventorySystem> );

    Bind( AutoId( "drop_on_death_system" ), &CreateSystem<DropOnDeathSystem> );
    Bind( AutoId( "remove_on_death_system" ), &CreateSystem<RemoveOnDeathSystem> );
    Bind( AutoId( "explode_on_death_system" ), &CreateSystem<ExplodeOnDeathSystem> );
    Bind( AutoId( "stop_on_death_system" ), &CreateSystem<StopOnDeathSystem> );
    Bind( AutoId( "score_on_death_system" ), &CreateSystem<engine::ScoreOnDeathSystem> );
    Bind( AutoId( "kill_score_on_death_system" ), &CreateSystem<engine::KillScoreOnDeathSystem> );

    Bind( AutoId( "notify_parent_on_death_system" ), &CreateSystem<NotifyParentOnDeathSystem> );

    Bind( AutoId( "listen_child_death_system" ), &CreateSystem<ListenChildDeathSystem> );

    Bind( AutoId( "health_system" ), &CreateSystem<HealthSystem> );
    Bind( AutoId( "fade_out_system" ), &CreateSystem<FadeOutSystem> );
    Bind( AutoId( "move_system" ), &CreateSystem<MoveSystem> );
    Bind( AutoId( "rotate_component_system" ), &CreateSystem<RotateComponentSystem> );
    Bind( AutoId( "acceleration_system" ), &CreateSystem<AccelerationSystem> );
    Bind( AutoId( "explosion_system" ), &CreateSystem<ExplosionSystem> );

    Bind( AutoId( "server_system" ), &CreateSystem<network::ServerSystem> );
    Bind( AutoId( "client_system" ), &CreateSystem<network::ClientSystem> );
    Bind( AutoId( "local_system" ), &CreateSystem<LocalSystem> );

    Bind( AutoId( "soldier_properties_system" ), &CreateSystem<SoldierPropertiesSystem> );
    Bind( AutoId( "soldier_spawn_system" ), &CreateSystem<SoldierSpawnSystem> );
    Bind( AutoId( "soldier_auto_revive_system" ), &CreateSystem<engine::SoldierAutoReviveSystem> );

    Bind( AutoId( "lifecycle_sender_system" ), &CreateSystem<network::LifecycleSenderSystem> );
    Bind( AutoId( "message_handler_sub_system_holder" ), &CreateSystem<network::MessageHandlerSubSystemHolder> );
    Bind( AutoId( "position_message_sender_system" ), &CreateSystem<network::PositionMessageSenderSystem> );
    Bind( AutoId( "move_message_sender_system" ), &CreateSystem<network::MoveMessageSenderSystem> );
    Bind( AutoId( "rotate_message_sender_system" ), &CreateSystem<network::RotateMessageSenderSystem> );
    Bind( AutoId( "player_controller_message_sender_system" ), &CreateSystem<network::PlayerControllerMessageSenderSystem> );
    Bind( AutoId( "create_actor_message_sender_system" ), &CreateSystem<network::CreateActorMessageSenderSystem> );
    Bind( AutoId( "damage_taken_message_sender_system" ), &CreateSystem<network::DamageTakenMessageSenderSystem> );
    Bind( AutoId( "heal_taken_message_sender_system" ), &CreateSystem<network::HealTakenMessageSenderSystem> );
    Bind( AutoId( "orientation_message_sender_system" ), &CreateSystem<network::OrientationMessageSenderSystem> );
    Bind( AutoId( "heading_message_sender_system" ), &CreateSystem<network::HeadingMessageSenderSystem> );
    Bind( AutoId( "pickup_message_sender_system" ), &CreateSystem<network::PickupMessageSenderSystem> );
    Bind( AutoId( "ping_message_sender_system" ), &CreateSystem<network::PingMessageSenderSystem> );
    Bind( AutoId( "revive_message_sender_system" ), &CreateSystem<network::ReviveMessageSenderSystem> );
    Bind( AutoId( "flash_message_sender_system" ), &CreateSystem<network::FlashMessageSenderSystem> );
    Bind( AutoId( "soldier_properties_message_sender_system" ), &CreateSystem<network::SoldierPropertiesMessageSenderSystem> );
    Bind( AutoId( "client_datas_message_sender_system" ), &CreateSystem<network::ClientDatasMessageSenderSystem> );
    Bind( AutoId( "ctf_client_datas_message_sender_system" ), &CreateSystem<network::ctf::ClientDatasMessageSenderSystem> );
    Bind( AutoId( "health_message_sender_system" ), &CreateSystem<network::HealthMessageSenderSystem> );
    Bind( AutoId( "accuracy_message_sender_system" ), &CreateSystem<network::AccuracyMessageSenderSystem> );
    Bind( AutoId( "set_ownership_message_sender_system" ), &CreateSystem<network::SetOwnershipMessageSenderSystem> );
    Bind( AutoId( "set_team_message_sender_system" ), &CreateSystem<network::SetTeamMessageSenderSystem> );
    Bind( AutoId( "show_text_message_message_sender_system" ), &CreateSystem<network::ShowTextMessageMessageSenderSystem> );
    Bind( AutoId( "collision_message_sender_system" ), &CreateSystem<network::CollisionMessageSenderSystem> );
    Bind( AutoId( "shot_message_sender_system" ), &CreateSystem<network::ShotMessageSenderSystem> );
    Bind( AutoId( "kill_score_message_sender_system" ), &CreateSystem<network::KillScoreMessageSenderSystem> );
    Bind( AutoId( "client_score_message_sender_system" ), &CreateSystem<network::ClientScoreMessageSenderSystem> );
    Bind( AutoId( "item_changed_message_sender_system" ), &CreateSystem<network::ItemChangedMessageSenderSystem> );
    Bind( AutoId( "cloak_changed_message_sender_system" ), &CreateSystem<network::CloakChangedMessageSenderSystem> );
    Bind( AutoId( "border_message_sender_system" ), &CreateSystem<network::BorderMessageSenderSystem> );
    Bind( AutoId( "secs_to_revive_message_sender_system" ), &CreateSystem<network::SecsToReviveMessageSenderSystem> );
    Bind( AutoId( "sync_item_message_sender_system" ), &CreateSystem<network::SyncItemMessageSenderSystem> );
    Bind( AutoId( "modify_audible_component_message_sender_system" ), &CreateSystem<network::ModifyAudibleComponentMessageSenderSystem> );
    Bind( AutoId( "flag_state_changed_message_sender_system" ), &CreateSystem<network::FlagStateChangedMessageSenderSystem> );
    Bind( AutoId( "fade_out_message_sender_system" ), &CreateSystem<network::FadeOutMessageSenderSystem> );
    Bind( AutoId( "team_switch_request_message_sender_system" ), &CreateSystem<network::TeamSwitchRequestMessageSenderSystem> );
    Bind( AutoId( "actor_list_message_sender_system" ), &CreateSystem<network::ActorListMessageSenderSystem> );
    Bind( AutoId( "data_checksum_message_sender_system" ), &CreateSystem<network::DataChecksumMessageSenderSystem>);

    Bind( AutoId( "ctf_client_list_displaying_system" ), &CreateSystem<network::CtfClientListDisplayingSystem> );
    Bind( AutoId( "ctf_client_list_handling_system" ), &CreateSystem<network::CtfClientListHandlingSystem> );
    Bind( AutoId( "ffa_client_list_system" ), &CreateSystem<network::FfaClientListSystem> );
    Bind( AutoId( "waiting_system" ), &CreateSystem<network::WaitingStartSystem> );

    Bind( AutoId( "target_holder_system" ), &CreateSystem<TargetHolderSystem> );
    Bind( AutoId( "removed_actors_system" ), &CreateSystem<RemovedActorsSystem> );

    Bind( AutoId( "buff_holder_system" ), &CreateSystem<BuffHolderSystem> );
    Bind( AutoId( "armor_system" ), &CreateSystem<engine::ArmorSystem> );
    Bind( AutoId( "cloak_system" ), &CreateSystem<engine::CloakSystem> );

    Bind( AutoId( "map_system" ), &CreateSystem<map::MapSystem> );
    Bind( AutoId( "link_map_element_system" ), &CreateSystem<map::LinkMapElementSystem> );
    Bind( AutoId( "map_start_map_element_system" ), &CreateSystem<map::MapStartMapElementSystem> );
    Bind( AutoId( "spawn_soldiers_map_element_system" ), &CreateSystem<map::SpawnSoldiersMapElementSystem> );
    Bind( AutoId( "soldier_spawn_point_map_element_system" ), &CreateSystem<map::SoldierSpawnPointMapElementSystem> );
    Bind( AutoId( "spawn_actor_map_element_system" ), &CreateSystem<map::SpawnActorMapElementSystem> );
    Bind( AutoId( "respawn_actor_map_element_system" ), &CreateSystem<map::RespawnActorMapElementSystem> );
    Bind( AutoId( "soldier_auto_revive_map_element_system" ), &CreateSystem<map::SoldierAutoReviveMapElementSystem> );
    Bind( AutoId( "level_generator_map_element_system" ), &CreateSystem<map::LevelGeneratorMapElementSystem> );
    Bind( AutoId( "level_generated_map_element_system" ), &CreateSystem<map::LevelGeneratedMapElementSystem> );
    Bind( AutoId( "group_map_element_system" ), &CreateSystem<map::GroupMapElementSystem> );
    Bind( AutoId( "recreate_borders_map_element_system" ), &CreateSystem<map::RecreateBordersMapElementSystem> );

    Bind( AutoId( "free_for_all_game_mode_system" ), &CreateSystem<core::FreeForAllGameModeSystem> );
    Bind( AutoId( "capture_the_flag_game_mode_system" ), &CreateSystem<core::CaptureTheFlagGameModeSystem> );
    Bind( AutoId( "rogue_game_mode_system" ), &CreateSystem<core::RogueGameModeSystem> );

    Bind( AutoId( "ctf_soldier_spawn_point_map_element_system" ), &CreateSystem<map::ctf::CtfSoldierSpawnPointMapElementSystem> );
    Bind( AutoId( "ctf_spawn_soldiers_map_element_system" ), &CreateSystem<map::ctf::CtfSpawnSoldiersMapElementSystem> );
    Bind( AutoId( "ctf_soldier_spawn_system" ), &CreateSystem<engine::ctf::CtfSoldierSpawnSystem> );
    Bind( AutoId( "ctf_spawn_flags_map_element_system" ), &CreateSystem<map::ctf::CtfSpawnFlagsMapElementSystem> );
    Bind( AutoId( "flag_spawn_system" ), &CreateSystem<engine::ctf::FlagSpawnSystem> );
    Bind( AutoId( "ctf_score_message_sender_system" ), &CreateSystem<network::ctf::CtfScoreMessageSenderSystem> );

    Bind( AutoId( "attachable_system" ), &CreateSystem<engine::ctf::AttachableSystem> );
    Bind( AutoId( "show_text_system" ), &CreateSystem<engine::ShowTextSystem> );
    Bind( AutoId( "editor_system" ), &CreateSystem<map::EditorSystem> );
    Bind( AutoId( "room_editor_system" ), &CreateSystem<map::RoomEditorSystem> );
    Bind( AutoId( "kill_score_system" ), &CreateSystem<engine::KillScoreSystem> );

    Bind( AutoId( "editor_target_system" ), &CreateSystem<map::EditorTargetSystem> );
    Bind( AutoId( "editor_grid_system" ), &CreateSystem<map::EditorGridSystem> );
    Bind( AutoId( "editor_brush_system" ), &CreateSystem<map::EditorBrushSystem> );
    Bind( AutoId( "editor_soldier_spawn_system" ), &CreateSystem<map::EditorSoldierSpawnSystem> );
    Bind( AutoId( "editor_layer_system" ), &CreateSystem<map::EditorLayerSystem> );
    Bind( AutoId( "editor_actor_system" ), &CreateSystem<map::EditorActorSystem> );
    Bind( AutoId( "room_cell_editor_system" ), &CreateSystem<map::RoomCellEditorSystem> );
    Bind( AutoId( "editor_select_system" ), &CreateSystem<map::EditorSelectSystem> );
    Bind( AutoId( "editor_group_system" ), &CreateSystem<map::EditorGroupSystem> );
    Bind( AutoId( "editor_visibility_system" ), &CreateSystem<map::EditorVisibilitySystem> );
    Bind( AutoId( "editor_renderable_layer_system" ), &CreateSystem<map::EditorRenderableLayerSystem> );
    Bind( AutoId( "property_editor_system" ), &CreateSystem<map::PropertyEditorSystem> );
    Bind( AutoId( "room_start_editor_system" ), &CreateSystem<map::RoomStartEditorSystem> );
    Bind( AutoId( "room_end_editor_system" ), &CreateSystem<map::RoomEndEditorSystem> );
    Bind( AutoId( "spawn_editor_system" ), &CreateSystem<map::SpawnEditorSystem> );
    Bind( AutoId( "cell_entrance_editor_system" ), &CreateSystem<map::CellEntranceEditorSystem> );
    Bind( AutoId( "room_plain_property_editor_system" ), &CreateSystem<map::RoomPlainPropertyEditorSystem> );

    Bind( AutoId( "worm_head_system" ), &CreateSystem<engine::WormHeadSystem> );
    Bind( AutoId( "worm_body_system" ), &CreateSystem<engine::WormBodySystem> );

    Bind( AutoId( "detonate_on_hit_system" ), &CreateSystem<engine::DetonateOnHitSystem> );
    Bind( AutoId( "explode_on_hit_system" ), &CreateSystem<engine::ExplodeOnHitSystem> );
    Bind( AutoId( "audio_system" ), &CreateSystem<audio::AudioSystem> );
    Bind( AutoId( "gamemode_selected_message_sender_system" ), &CreateSystem<network::GamemodeSelectedMessageSenderSystem> );
    Bind( AutoId( "remove_components_on_death_system" ), &CreateSystem<engine::RemoveComponentsOnDeathSystem> );
    Bind( AutoId( "player_model_system" ), &CreateSystem<engine::PlayerModelSystem> );
    Bind( AutoId( "level_end_system" ), &CreateSystem<engine::LevelEndSystem> );
    Bind( AutoId( "level_selection_system"), &CreateSystem<core::LevelSelectionSystem>);
}

REGISTER_INIT_PRIO( bbb, SystemFactory, boost::bind( &SystemFactory::Init, &SystemFactory::Get() ) )

} // namespace engine
