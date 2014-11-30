#include "platform/i_platform.h"
#include "core/component_loader_factory.h"
#include "core/health_component.h"
#include "core/collision_component.h"
#include "core/pickup_collision_component.h"

ComponentLoaderFactory::ComponentLoaderFactory()
{
    Bind( AutoId( "default_component" ), &CreateComponentLoader<DefaultComponentLoader>);
    SetDefault( AutoId( "default_component" ) );
    Bind( AutoId( "health_component" ), &CreateComponentLoader<HealthComponentLoader>);
    Bind( AutoId( "collision_component" ), &CreateComponentLoader<CollisionComponentLoader>);
    Bind( AutoId( "wall_collision_component" ), &CreateComponentLoader<CollisionComponentLoader>);
    Bind( AutoId( "pickup_collision_component" ), &CreateComponentLoader<PickupCollisionComponentLoader>);
    
}

void DefaultComponentLoader::BindValues()
{

}
DefaultComponentLoader::DefaultComponentLoader()
{
}



