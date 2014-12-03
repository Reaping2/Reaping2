#include "platform/i_platform.h"
#include "core/shot.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/damage_action.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"
#include "core/shot_collision_component.h"

Shot::Shot( std::string const& Name )
    : Actor( AutoId(Name) )
{
    AddComponent( mComponentFactory(AutoId("shot_collision_component")));
    Opt<ShotCollisionComponent> collisionC = Get<ShotCollisionComponent>();
    collisionC->SetRadius(0.02);
    collisionC->SetCollisionClass(CollisionClass::Projectile);
    collisionC->SetDamage(10);

    Get<IMoveComponent>()->SetSpeed( 2 );
    AddAction( AutoId( "move" ) );
    AddComponent(ComponentFactory::Get()(AutoId("health_component")));
    AddComponent(ComponentFactory::Get()(AutoId("remove_on_death_component")));
}

