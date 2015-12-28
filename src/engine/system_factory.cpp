#include "platform/i_platform.h"
#include "engine/system_factory.h"
#include "engine/system.h"
#include "engine/collision_system.h"
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
#include "network/client_list_system.h"

using platform::AutoId;
namespace engine {

SystemFactory::SystemFactory()
{
    Bind( AutoId("default_system"), &CreateSystem<DefaultSystem> );
    SetDefault( AutoId("default_system") );

    Bind( AutoId("window_system"), &CreateSystem<WindowSystem>);
    Bind( AutoId("timer_server_system"), &CreateSystem<TimerServerSystem>);
    Bind( AutoId("frame_counter_system"), &CreateSystem<FrameCounterSystem>);
    Bind( AutoId("renderer_system"), &CreateSystem<RendererSystem>);
    Bind( AutoId("leaderboard_system"), &CreateSystem<engine::LeaderboardSystem>);

    Bind( AutoId("collision_system"), &CreateSystem<CollisionSystem>);
    Bind( AutoId("keyboard_system"), &CreateSystem<KeyboardSystem>);
    Bind( AutoId("mouse_system"), &CreateSystem<MouseSystem>);
    Bind( AutoId("controller_system"), &CreateSystem<ControllerSystem>);
    Bind( AutoId("inventory_system"), &CreateSystem<InventorySystem>);

    Bind( AutoId("drop_on_death_system"), &CreateSystem<DropOnDeathSystem>);
    Bind( AutoId("remove_on_death_system"), &CreateSystem<RemoveOnDeathSystem>);
    Bind( AutoId("explode_on_death_system"), &CreateSystem<ExplodeOnDeathSystem>);
    Bind( AutoId("stop_on_death_system"), &CreateSystem<StopOnDeathSystem>);
    Bind( AutoId("score_on_death_system"), &CreateSystem<engine::ScoreOnDeathSystem>);
    Bind( AutoId("kill_score_on_death_system"), &CreateSystem<engine::KillScoreOnDeathSystem>);

    Bind( AutoId("notify_parent_on_death_system"), &CreateSystem<NotifyParentOnDeathSystem>);

    Bind( AutoId("listen_child_death_system"), &CreateSystem<ListenChildDeathSystem>);

    Bind( AutoId("health_system"), &CreateSystem<HealthSystem>);
    Bind( AutoId("fade_out_system"), &CreateSystem<FadeOutSystem>);
    Bind( AutoId("move_system"), &CreateSystem<MoveSystem>);
    Bind( AutoId("acceleration_system"), &CreateSystem<AccelerationSystem>);
    Bind( AutoId("explosion_system"), &CreateSystem<ExplosionSystem>);

    Bind( AutoId("server_system"), &CreateSystem<network::ServerSystem>);
    Bind( AutoId("client_system"), &CreateSystem<network::ClientSystem>);
    Bind( AutoId("local_system"), &CreateSystem<LocalSystem>);

    Bind( AutoId("soldier_properties_system"), &CreateSystem<SoldierPropertiesSystem>);
    Bind( AutoId("soldier_spawn_system"), &CreateSystem<SoldierSpawnSystem>);

    Bind( AutoId("lifecycle_sender_system"), &CreateSystem<network::LifecycleSenderSystem>);
    Bind( AutoId("message_handler_sub_system_holder"), &CreateSystem<network::MessageHandlerSubSystemHolder>);
    Bind( AutoId("position_message_sender_system"), &CreateSystem<network::PositionMessageSenderSystem>);
    Bind( AutoId("move_message_sender_system"), &CreateSystem<network::MoveMessageSenderSystem>);
    Bind( AutoId("player_controller_message_sender_system"), &CreateSystem<network::PlayerControllerMessageSenderSystem>);
    Bind( AutoId("create_actor_message_sender_system"), &CreateSystem<network::CreateActorMessageSenderSystem>);
    Bind( AutoId("damage_taken_message_sender_system"), &CreateSystem<network::DamageTakenMessageSenderSystem>);
    Bind( AutoId("heal_taken_message_sender_system"), &CreateSystem<network::HealTakenMessageSenderSystem>);
    Bind( AutoId("orientation_message_sender_system"), &CreateSystem<network::OrientationMessageSenderSystem>);
    Bind( AutoId("heading_message_sender_system"), &CreateSystem<network::HeadingMessageSenderSystem>);
    Bind( AutoId("pickup_message_sender_system"), &CreateSystem<network::PickupMessageSenderSystem>);
    Bind( AutoId("ping_message_sender_system"), &CreateSystem<network::PingMessageSenderSystem>);
    Bind( AutoId("revive_message_sender_system"), &CreateSystem<network::ReviveMessageSenderSystem>);
    Bind( AutoId("flash_message_sender_system"), &CreateSystem<network::FlashMessageSenderSystem>);
    Bind( AutoId("soldier_properties_message_sender_system"), &CreateSystem<network::SoldierPropertiesMessageSenderSystem>);
    Bind( AutoId("client_datas_message_sender_system"), &CreateSystem<network::ClientDatasMessageSenderSystem>);
    Bind( AutoId("health_message_sender_system"), &CreateSystem<network::HealthMessageSenderSystem>);
    Bind( AutoId("accuracy_message_sender_system"), &CreateSystem<network::AccuracyMessageSenderSystem>);
    Bind( AutoId("set_ownership_message_sender_system"), &CreateSystem<network::SetOwnershipMessageSenderSystem>);
    Bind( AutoId("set_team_message_sender_system"), &CreateSystem<network::SetTeamMessageSenderSystem>);
    Bind( AutoId("show_text_message_message_sender_system"), &CreateSystem<network::ShowTextMessageMessageSenderSystem>);
    Bind( AutoId("collision_message_sender_system"), &CreateSystem<network::CollisionMessageSenderSystem>);
    Bind( AutoId("shot_message_sender_system"), &CreateSystem<network::ShotMessageSenderSystem>);
    Bind( AutoId("kill_score_message_sender_system"), &CreateSystem<network::KillScoreMessageSenderSystem>);
    Bind( AutoId("client_score_message_sender_system"), &CreateSystem<network::ClientScoreMessageSenderSystem>);
    Bind( AutoId("item_changed_message_sender_system"), &CreateSystem<network::ItemChangedMessageSenderSystem>);
    Bind( AutoId("cloak_changed_message_sender_system"), &CreateSystem<network::CloakChangedMessageSenderSystem>);
    Bind( AutoId("client_list_system"), &CreateSystem<network::ClientListSystem>);

    Bind( AutoId("target_holder_system"), &CreateSystem<TargetHolderSystem>);
    Bind( AutoId("removed_actors_system"), &CreateSystem<RemovedActorsSystem>);

    Bind( AutoId("buff_holder_system"), &CreateSystem<BuffHolderSystem>);
    Bind( AutoId("armor_system"), &CreateSystem<engine::ArmorSystem>);
    Bind( AutoId("cloak_system"), &CreateSystem<engine::CloakSystem>);
    
    Bind( AutoId("map_system"), &CreateSystem<map::MapSystem>);
    Bind( AutoId("link_map_element_system"), &CreateSystem<map::LinkMapElementSystem>);
    Bind( AutoId("map_start_map_element_system"), &CreateSystem<map::MapStartMapElementSystem>);
    Bind( AutoId("spawn_soldiers_map_element_system"), &CreateSystem<map::SpawnSoldiersMapElementSystem>);
    Bind( AutoId("soldier_spawn_point_map_element_system"), &CreateSystem<map::SoldierSpawnPointMapElementSystem>);
    Bind( AutoId("spawn_actor_map_element_system"), &CreateSystem<map::SpawnActorMapElementSystem>);
    Bind( AutoId("respawn_actor_map_element_system"), &CreateSystem<map::RespawnActorMapElementSystem>);

    Bind( AutoId("free_for_all_game_mode_system"), &CreateSystem<core::FreeForAllGameModeSystem>);
    Bind( AutoId("capture_the_flag_game_mode_system"), &CreateSystem<core::CaptureTheFlagGameModeSystem>);
    Bind( AutoId("ctf_soldier_spawn_point_map_element_system"), &CreateSystem<map::ctf::CtfSoldierSpawnPointMapElementSystem>);
    Bind( AutoId("ctf_spawn_soldiers_map_element_system"), &CreateSystem<map::ctf::CtfSpawnSoldiersMapElementSystem>);
    Bind( AutoId("ctf_soldier_spawn_system"), &CreateSystem<engine::ctf::CtfSoldierSpawnSystem>);
    Bind( AutoId("ctf_spawn_flags_map_element_system"), &CreateSystem<map::ctf::CtfSpawnFlagsMapElementSystem>);
    Bind( AutoId("flag_spawn_system"), &CreateSystem<engine::ctf::FlagSpawnSystem>);
    Bind( AutoId("ctf_score_message_sender_system"), &CreateSystem<network::ctf::CtfScoreMessageSenderSystem>);

    Bind( AutoId("attachable_system"), &CreateSystem<engine::ctf::AttachableSystem>);
    Bind( AutoId("show_text_system"), &CreateSystem<engine::ShowTextSystem>);
    Bind( AutoId("editor_system"), &CreateSystem<map::EditorSystem>);
    Bind( AutoId("kill_score_system"), &CreateSystem<engine::KillScoreSystem>);

    Bind( AutoId("editor_target_system"), &CreateSystem<map::EditorTargetSystem>);
    Bind( AutoId("editor_grid_system"), &CreateSystem<map::EditorGridSystem>);
    Bind( AutoId("editor_brush_system"), &CreateSystem<map::EditorBrushSystem>);
    Bind( AutoId("editor_soldier_spawn_system"), &CreateSystem<map::EditorSoldierSpawnSystem>);

    Bind( AutoId("detonate_on_hit_system"), &CreateSystem<engine::DetonateOnHitSystem>);
    Bind( AutoId("explode_on_hit_system"), &CreateSystem<engine::ExplodeOnHitSystem>);
}

} // namespace engine
