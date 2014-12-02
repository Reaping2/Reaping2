#include "platform/i_platform.h"
#include "core/component_loader_factory.h"
#include "platform/auto_id.h"
#include "core/move_component.h"
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
#include "core/position_component.h"
#include "core/renderable_component.h"


ComponentLoaderFactory::ComponentLoaderFactory()
{
    Bind( AutoId( "default_component" ), &CreateComponentLoader<DefaultComponentLoader>);
    SetDefault( AutoId( "default_component" ) );

    Bind( AutoId("position_component"), &CreateComponentLoader<PositionComponentLoader>);
    Bind( AutoId("move_component"), &CreateComponentLoader<MoveComponentLoader>);
    Bind( AutoId("player_controller_component"), &CreateComponentLoader<PlayerControllerComponentLoader>);
    Bind( AutoId("target_player_controller_component"), &CreateComponentLoader<TargetPlayerControllerComponentLoader>);
    Bind( AutoId("random_controller_component"), &CreateComponentLoader<RandomControllerComponentLoader>);
    Bind( AutoId("inventory_component"), &CreateComponentLoader<InventoryComponentLoader>);

    Bind( AutoId("health_component"), &CreateComponentLoader<HealthComponentLoader>);
    Bind( AutoId("health_delete_component"), &CreateComponentLoader<HealthDeleteComponentLoader>);

    Bind( AutoId("collision_component"), &CreateComponentLoader<CollisionComponentLoader>);
    Bind( AutoId("shot_collision_component"), &CreateComponentLoader<ShotCollisionComponentLoader>);
    Bind( AutoId("wall_collision_component"), &CreateComponentLoader<WallCollisionComponentLoader>);
    Bind( AutoId("pickup_collision_component"), &CreateComponentLoader<PickupCollisionComponentLoader>);
 
    Bind( AutoId("renderable_component"), &CreateComponentLoader<RenderableComponentLoader>);
    
}

void DefaultComponentLoader::BindValues()
{

}
DefaultComponentLoader::DefaultComponentLoader()
{
}



