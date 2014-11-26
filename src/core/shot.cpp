#include "i_core.h"
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
    collisionC->SetActor( this );

    Get<IMoveComponent>()->SetSpeed( 2 );
    AddAction( AutoId( "move" ) );
    AddComponent(ComponentFactory::Get()(AutoId("health_delete_component")));
    Get<IHealthComponent>()->SetActor(this);
}

