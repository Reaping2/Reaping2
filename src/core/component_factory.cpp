#include "platform/i_platform.h"
#include "core/component_factory.h"
#include "platform/auto_id.h"
#include "core/move_component.h"
#include "core/player_controller_component.h"
#include "core/target_player_controller_component.h"
#include "core/random_controller_component.h"
#include "core/inventory_component.h"
#include "core/health_component.h"
#include "core/collision_component.h"
#include "core/shot_collision_component.h"
#include "core/wall_collision_component.h"
#include "core/pickup_collision_component.h"
#include "core/position_component.h"
#include "core/renderable_component.h"
#include "core/remove_on_death_component.h"
#include "core/drop_on_death_component.h"
#include "core/fade_out_component.h"
#include "core/acceleration_component.h"
#include "core/explode_on_death_component.h"
#include "explosion_component.h"
#include "platform/factory.h"
#include "aoe_collision_component.h"
#include "bounce_collision_component.h"
#include "stop_on_death_component.h"
#include "target_holder_component.h"
#include "listen_child_death_component.h"
#include "notify_parent_on_death_component.h"
#include "pointer_target_controller_component.h"
#include "buffs/buff_holder_component.h"
#include "accuracy_component.h"
#include "team_component.h"
#include "flag_collision_component.h"
#include "attachable_component.h"
#include "flag_carrier_component.h"
#include "flag_receiver_component.h"
#include "score_on_death_component.h"
#include "owner_component.h"
#include "kill_score_on_death_component.h"
#include "heat_source_component.h"
#include "emitter_component.h"
#include "explode_on_hit_component.h"
#include "detonate_on_hit_component.h"
#include "armor_component.h"
#include "cloak_component.h"
#include "border_component.h"
#include "water_collision_component.h"
#include "audible_component.h"
#include "remove_components_on_death_component.h"
#include "rotate_component.h"
#include "render/visual_box_multiplier_component.h"
#include "cell_component.h"
#include "level_end_component.h"
#include "guard_controller_component.h"
#include "static_actor_component.h"
#include "path_controller_component.h"
#include "pulse_component.h"
#include "scripted_controller/scripted_controller_component.h"
#include "worm_head_component.h"
#include "worm_body_component.h"

using platform::AutoId;

ComponentFactory::ComponentFactory()
{
    Bind( AutoId( "default_component" ), &CreateComponent<DefaultComponent> );
    SetDefault( AutoId( "default_component" ) );

    Bind( AutoId( "position_component" ), &CreateComponent<PositionComponent> );
    Bind( AutoId( "rotate_component" ), &CreateComponent<RotateComponent> );
    Bind( AutoId( "visual_box_multiplier_component" ), &CreateComponent<render::VisualBoxMultiplierComponent> );
    Bind( AutoId( "move_component" ), &CreateComponent<MoveComponent> );
    Bind( AutoId( "acceleration_component" ), &CreateComponent<AccelerationComponent> );
    Bind( AutoId( "pulse_component" ), &CreateComponent<PulseComponent> );

    Bind( AutoId( "player_controller_component" ), &CreateComponent<PlayerControllerComponent> );
    Bind( AutoId( "target_player_controller_component" ), &CreateComponent<TargetPlayerControllerComponent> );
    Bind( AutoId( "random_controller_component" ), &CreateComponent<RandomControllerComponent> );
    Bind( AutoId( "guard_controller_component" ), &CreateComponent<GuardControllerComponent> );
    Bind( AutoId( "path_controller_component" ), &CreateComponent<PathControllerComponent> );

    Bind( AutoId( "scripted_controller_component" ), &CreateComponent<scriptedcontroller::ScriptedControllerComponent> );

    Bind( AutoId( "inventory_component" ), &CreateComponent<InventoryComponent> );
    Bind( AutoId( "static_actor_component" ), &CreateComponent<StaticActorComponent> );
    Bind( AutoId( "health_component" ), &CreateComponent<HealthComponent> );

    Bind( AutoId( "collision_component" ), &CreateComponent<CollisionComponent> );
    Bind( AutoId( "shot_collision_component" ), &CreateComponent<ShotCollisionComponent> );
    Bind( AutoId( "wall_collision_component" ), &CreateComponent<WallCollisionComponent> );
    Bind( AutoId( "water_collision_component" ), &CreateComponent<WaterCollisionComponent> );
    Bind( AutoId( "pickup_collision_component" ), &CreateComponent<PickupCollisionComponent> );
    Bind( AutoId( "aoe_collision_component" ), &CreateComponent<AoeCollisionComponent> );
    Bind( AutoId( "bounce_collision_component" ), &CreateComponent<BounceCollisionComponent> );

    Bind( AutoId( "renderable_component" ), &CreateComponent<RenderableComponent> );
    Bind( AutoId( "border_component" ), &CreateComponent<BorderComponent> );

    Bind( AutoId( "remove_on_death_component" ), &CreateComponent<RemoveOnDeathComponent> );
    Bind( AutoId( "explode_on_death_component" ), &CreateComponent<ExplodeOnDeathComponent> );
    Bind( AutoId( "drop_on_death_component" ), &CreateComponent<DropOnDeathComponent> );
    Bind( AutoId( "stop_on_death_component" ), &CreateComponent<StopOnDeathComponent> );
    Bind( AutoId( "score_on_death_component" ), &CreateComponent<ScoreOnDeathComponent> );
    Bind( AutoId( "owner_component" ), &CreateComponent<OwnerComponent> );
    Bind( AutoId( "kill_score_on_death_component" ), &CreateComponent<KillScoreOnDeathComponent> );

    Bind( AutoId( "notify_parent_on_death_component" ), &CreateComponent<NotifyParentOnDeathComponent> );
    Bind( AutoId( "listen_child_death_component" ), &CreateComponent<ListenChildDeathComponent> );
    Bind( AutoId( "pointer_target_controller_component" ), &CreateComponent<PointerTargetControllerComponent> );
    Bind( AutoId( "heat_source_component" ), &CreateComponent<HeatSourceComponent> );

    Bind( AutoId( "fade_out_component" ), &CreateComponent<FadeOutComponent> );

    Bind( AutoId( "explosion_component" ), &CreateComponent<ExplosionComponent> );

    Bind( AutoId( "target_holder_component" ), &CreateComponent<TargetHolderComponent> );
    Bind( AutoId( "buff_holder_component" ), &CreateComponent<BuffHolderComponent> );
    Bind( AutoId( "accuracy_component" ), &CreateComponent<AccuracyComponent> );
    Bind( AutoId( "armor_component" ), &CreateComponent<ArmorComponent> );
    Bind( AutoId( "cloak_component" ), &CreateComponent<CloakComponent> );

    Bind( AutoId( "team_component" ), &CreateComponent<TeamComponent> );
    Bind( AutoId( "flag_collision_component" ), &CreateComponent<ctf::FlagCollisionComponent> );
    Bind( AutoId( "attachable_component" ), &CreateComponent<ctf::AttachableComponent> );
    Bind( AutoId( "flag_carrier_component" ), &CreateComponent<ctf::FlagCarrierComponent> );
    Bind( AutoId( "flag_receiver_component" ), &CreateComponent<ctf::FlagReceiverComponent> );

    Bind( AutoId( "worm_head_component" ), &CreateComponent<WormHeadComponent> );
    Bind( AutoId( "worm_body_component" ), &CreateComponent<WormBodyComponent> );

    Bind( AutoId( "emitter_component" ), &CreateComponent<EmitterComponent> );
    Bind( AutoId( "explode_on_hit_component" ), &CreateComponent<ExplodeOnHitComponent> );
    Bind( AutoId( "detonate_on_hit_component" ), &CreateComponent<DetonateOnHitComponent> );
    Bind( AutoId( "audible_component" ), &CreateComponent<AudibleComponent> );
    Bind( AutoId( "remove_components_on_death_component" ), &CreateComponent<RemoveComponentsOnDeathComponent> );
    Bind( AutoId( "cell_component" ), &CreateComponent<CellComponent> );
    Bind( AutoId( "level_end_component" ), &CreateComponent<LevelEndComponent> );
}
