#include "i_core.h"
#include "move_component.h"
#include "platform/auto_id.h"
#include "core/player_controller_component.h"
#include "core/target_player_controller_component.h"
#include "core/random_controller_component.h"
#include "core/inventory_component.h"
#include "core/health_component.h"
#include "core/health_delete_component.h"
#include "core/collision_component.h"
#include "core/shot_collision_component.h"
#include "core/wall_collision_component.h"
#include "core/pickup_collision_component.h"
#include "core/renderable_component.h"

using platform::AutoId;

ComponentFactory::ComponentFactory()
{
    Bind( AutoId("default_component"), &CreateComponent<DefaultComponent> );
    SetDefault( AutoId("default_component") );

    Bind( AutoId("position_component"), &CreateComponent<PositionComponent>);
    Bind( AutoId("move_component"), &CreateComponent<MoveComponent>);
    Bind( AutoId("player_controller_component"), &CreateComponent<PlayerControllerComponent>);
    Bind( AutoId("target_player_controller_component"), &CreateComponent<TargetPlayerControllerComponent>);
    Bind( AutoId("random_controller_component"), &CreateComponent<RandomControllerComponent>);
    Bind( AutoId("inventory_component"), &CreateComponent<InventoryComponent>);
    
    Bind( AutoId("health_component"), &CreateComponent<HealthComponent>);
    Bind( AutoId("health_delete_component"), &CreateComponent<HealthDeleteComponent>);

    Bind( AutoId("collision_component"), &CreateComponent<CollisionComponent>);
    Bind( AutoId("shot_collision_component"), &CreateComponent<ShotCollisionComponent>);
    Bind( AutoId("wall_collision_component"), &CreateComponent<WallCollisionComponent>);
    Bind( AutoId("pickup_collision_component"), &CreateComponent<PickupCollisionComponent>);

    Bind( AutoId("renderable_component"), &CreateComponent<RenderableCompononent>);

}
