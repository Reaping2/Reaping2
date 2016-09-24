#include "platform/i_platform.h"
#include "engine/health_system.h"
#include "core/collision_model.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/renderable_component.h"
#include "core/i_controller_component.h"
#include "core/i_move_component.h"
#include "core/damage_taken_event.h"
#include "core/i_collision_component.h"
#include "core/heal_taken_event.h"
#include "core/actor_event.h"
#include "core/i_audible_component.h"

namespace engine {

HealthSystem::HealthSystem()
    : mScene( Scene::Get() )
{

}

void HealthSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IHealthComponent>().IsValid(); } );
}

void HealthSystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if ( !healthC.IsValid() )
        {
            continue;
        }
        if ( !healthC->IsAlive() )
        {
            continue;
        }
        int32_t currHp = healthC->GetHP();
        int32_t newHp = currHp;

        int32_t heal = healthC->GetHeal();
        newHp += heal;

        newHp = std::min( healthC->GetMaxHP().Get(), newHp );
        if ( heal > 0 )
        {
            Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
            if( positionC.IsValid() )
            {
                EventServer<core::HealTakenEvent>::Get().SendEvent( core::HealTakenEvent( positionC->GetX(), positionC->GetY(), healthC->GetHeal(), actor->GetGUID() ) );
            }
        }
        healthC->ResetHeal();

        int32_t damage = healthC->GetDamage();
        newHp -= damage;
        if ( damage > 0 )
        {
            //TODO: thats not enough. This way walls bleed too.
            Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
            if( positionC.IsValid() )
            {
                core::DamageTakenEvent damageTakeEvent = core::DamageTakenEvent( positionC->GetX(), positionC->GetY() );
                damageTakeEvent.ActorGUID = actor->GetGUID();
                damageTakeEvent.Damage = healthC->GetDamage();
                damageTakeEvent.type = core::DamageTakenEvent::Health;
                EventServer<core::DamageTakenEvent>::Get().SendEvent( damageTakeEvent );
            }
        }
        healthC->ResetDamage();

        if ( newHp <= 0 )
        {
            newHp = 0;
            healthC->SetAlive( false );
            Opt<ICollisionComponent> collisionC = actor->Get<ICollisionComponent>();
            if( collisionC.IsValid() )
            {
                collisionC->SetCollisionClass( CollisionClass::No_Collision );
            }
            healthC->SetTimeOfDeath( glfwGetTime() );
            Scene::Get().ModifyActor( actor, RenderableComponentModifier( RenderableLayer::Corpses, healthC->GetTimeOfDeath(), 0, -1, 1 ) );
            EventServer<ActorEvent>::Get().SendEvent( ActorEvent( ( actor ), ActorEvent::Died ) );
        }
        healthC->SetHP( newHp );

    }

}

} // namespace engine

