#include "platform/i_platform.h"
#include "core/component_loader_factory.h"
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

ComponentLoaderFactory::ComponentLoaderFactory()
{
    Bind( AutoId( "default_component" ), &CreateComponentLoader<DefaultComponentLoader>);
    SetDefault( AutoId( "default_component" ) );

    Bind( AutoId("position_component"), &CreateComponentLoader<PositionComponentLoader>);
    Bind( AutoId("move_component"), &CreateComponentLoader<MoveComponentLoader>);
    Bind( AutoId("acceleration_component"), &CreateComponentLoader<AccelerationComponentLoader>);
    Bind( AutoId("player_controller_component"), &CreateComponentLoader<PlayerControllerComponentLoader>);
    Bind( AutoId("target_player_controller_component"), &CreateComponentLoader<TargetPlayerControllerComponentLoader>);
    Bind( AutoId("random_controller_component"), &CreateComponentLoader<RandomControllerComponentLoader>);
    Bind( AutoId("inventory_component"), &CreateComponentLoader<InventoryComponentLoader>);

    Bind( AutoId("health_component"), &CreateComponentLoader<HealthComponentLoader>);

    Bind( AutoId("collision_component"), &CreateComponentLoader<CollisionComponentLoader>);
    Bind( AutoId("shot_collision_component"), &CreateComponentLoader<ShotCollisionComponentLoader>);
    Bind( AutoId("wall_collision_component"), &CreateComponentLoader<WallCollisionComponentLoader>);
    Bind( AutoId("pickup_collision_component"), &CreateComponentLoader<PickupCollisionComponentLoader>);
    Bind( AutoId("aoe_collision_component"), &CreateComponentLoader<AoeCollisionComponentLoader>);
    Bind( AutoId("bounce_collision_component"), &CreateComponentLoader<BounceCollisionComponentLoader>);
    Bind( AutoId("flag_collision_component"), &CreateComponentLoader<ctf::FlagCollisionComponentLoader>);

    Bind( AutoId("renderable_component"), &CreateComponentLoader<RenderableComponentLoader>);

    Bind( AutoId("remove_on_death_component"), &CreateComponentLoader<RemoveOnDeathComponentLoader>);
    Bind( AutoId("explode_on_death_component"), &CreateComponentLoader<ExplodeOnDeathComponentLoader>);
    Bind( AutoId("drop_on_death_component"), &CreateComponentLoader<DropOnDeathComponentLoader>);
    Bind( AutoId("stop_on_death_component"), &CreateComponentLoader<StopOnDeathComponentLoader>);
    Bind( AutoId("score_on_death_component"), &CreateComponentLoader<ScoreOnDeathComponentLoader>);
    Bind( AutoId("owner_component"), &CreateComponentLoader<OwnerComponentLoader>);
    Bind( AutoId("kill_score_on_death_component"), &CreateComponentLoader<KillScoreOnDeathComponentLoader>);

    Bind( AutoId("notify_parent_on_death_component"), &CreateComponentLoader<NotifyParentOnDeathComponentLoader>);
    Bind( AutoId("listen_child_death_component"), &CreateComponentLoader<ListenChildDeathComponentLoader>);
    Bind( AutoId("pointer_target_controller_component"), &CreateComponentLoader<PointerTargetControllerComponentLoader>);
    Bind( AutoId("heat_source_component"), &CreateComponentLoader<HeatSourceComponentLoader>);

    Bind( AutoId("fade_out_component"), &CreateComponentLoader<FadeOutComponentLoader>);

    Bind( AutoId("explosion_component"), &CreateComponentLoader<ExplosionComponentLoader>);

    Bind( AutoId("target_holder_component"), &CreateComponentLoader<TargetHolderComponentLoader>);
    Bind( AutoId("buff_holder_component"), &CreateComponentLoader<BuffHolderComponentLoader>);
    Bind( AutoId("accuracy_component"), &CreateComponentLoader<AccuracyComponentLoader>);
    Bind( AutoId("team_component"), &CreateComponentLoader<TeamComponentLoader>);
    Bind( AutoId("attachable_component"), &CreateComponentLoader<ctf::AttachableComponentLoader>);
    Bind( AutoId("flag_carrier_component"), &CreateComponentLoader<ctf::FlagCarrierComponentLoader>);
    Bind( AutoId("flag_receiver_component"), &CreateComponentLoader<ctf::FlagReceiverComponentLoader>);

    Bind( AutoId("emitter_component"), &CreateComponentLoader<EmitterComponentLoader>);
}

void DefaultComponentLoader::BindValues()
{

}
DefaultComponentLoader::DefaultComponentLoader()
{
}



