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

using platform::AutoId;

ComponentFactory::ComponentFactory()
{
    Bind( AutoId("default_component"), &CreateComponent<DefaultComponent> );
    SetDefault( AutoId("default_component") );

    Bind( AutoId("position_component"), &CreateComponent<PositionComponent>);
    Bind( AutoId("move_component"), &CreateComponent<MoveComponent>);
    Bind( AutoId("acceleration_component"), &CreateComponent<AccelerationComponent>);

    Bind( AutoId("player_controller_component"), &CreateComponent<PlayerControllerComponent>);
    Bind( AutoId("target_player_controller_component"), &CreateComponent<TargetPlayerControllerComponent>);
    Bind( AutoId("random_controller_component"), &CreateComponent<RandomControllerComponent>);

    Bind( AutoId("inventory_component"), &CreateComponent<InventoryComponent>);
    
    Bind( AutoId("health_component"), &CreateComponent<HealthComponent>);

    Bind( AutoId("collision_component"), &CreateComponent<CollisionComponent>);
    Bind( AutoId("shot_collision_component"), &CreateComponent<ShotCollisionComponent>);
    Bind( AutoId("wall_collision_component"), &CreateComponent<WallCollisionComponent>);
    Bind( AutoId("pickup_collision_component"), &CreateComponent<PickupCollisionComponent>);

    Bind( AutoId("renderable_component"), &CreateComponent<RenderableComponent>);

    Bind( AutoId("remove_on_death_component"), &CreateComponent<RemoveOnDeathComponent>);
    Bind( AutoId("drop_on_death_component"), &CreateComponent<DropOnDeathComponent>);

    Bind( AutoId("fade_out_component"), &CreateComponent<FadeOutComponent>);
}
