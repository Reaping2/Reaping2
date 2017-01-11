#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/collisions/aoe_collision_sub_system.h"
#include "core/aoe_collision_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/buffs/buff_factory.h"
#include "core/buffs/heal_over_time_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/i_owner_component.h"
#include "platform/game_clock.h"

namespace engine {

AoeCollisionSubSystem::AoeCollisionSubSystem()
    : CollisionSubSystem()
{
}


void AoeCollisionSubSystem::Init()
{
}


void AoeCollisionSubSystem::Update( Actor& actor, double DeltaTime )
{
}


void AoeCollisionSubSystem::ClipScene( Actor& actor )
{
}

void AoeCollisionSubSystem::Collide( Actor& actor, Actor& other )
{
    Opt<AoeCollisionComponent> aoeCC = actor.Get<ICollisionComponent>();
    Opt<IOwnerComponent> ownerC = actor.Get<IOwnerComponent>();
    Opt<IHealthComponent> otherHealthC = other.Get<IHealthComponent>();

    if( otherHealthC.IsValid() && otherHealthC->IsAlive() )
    {
        AoeCollisionComponent::Damaged_Actor_Ids_t const& actorIds = aoeCC->GetDamagedActorIds();
        if ( actorIds.find( other.GetGUID() ) == actorIds.end() )
        {
            otherHealthC->TakeDamage( aoeCC->GetDamage() );
            if ( ownerC.IsValid() )
            {
                otherHealthC->SetLastDamageOwnerGUID( ownerC->GetOwnerGUID() );
                otherHealthC->SetLastDamageTime( platform::Clock::Now() );
            }
            aoeCC->AddDamagedActorId( other.GetGUID() );
        }
    }
}


} // namespace engine

