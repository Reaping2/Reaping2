#include "platform/i_platform.h"
#include "armor_system.h"
#include "core/i_armor_component.h"
#include "core/i_health_component.h"
#include "core/damage_taken_event.h"
#include "core/i_position_component.h"

namespace engine {

ArmorSystem::ArmorSystem()
    : mScene( Scene::Get() )
{
}


void ArmorSystem::Init()
{
}


void ArmorSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IArmorComponent> armorC = actor.Get<IArmorComponent>();
        if ( !armorC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if ( !healthC.IsValid() || !healthC->IsAlive()
             || healthC->GetDamage() <= 0 )
        {
            continue;
        }
        int32_t damage = healthC->GetDamage();
        int32_t newArmor = armorC->GetCurrentArmor() - damage;
        healthC->ResetDamage();
        if ( newArmor < 0 )
        {
            healthC->TakeDamage( -1 * newArmor );
            newArmor = 0;
        }
        int32_t armorDiff = armorC->GetCurrentArmor() - newArmor;
        armorC->SetCurrentArmor( newArmor );
        Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
        if( positionC.IsValid() )
        {
            core::DamageTakenEvent damageTakeEvent = core::DamageTakenEvent( positionC->GetX(), positionC->GetY() );
            damageTakeEvent.ActorGUID = actor.GetGUID();
            damageTakeEvent.Damage = armorDiff;
            damageTakeEvent.type = core::DamageTakenEvent::Armor;
            EventServer<core::DamageTakenEvent>::Get().SendEvent( damageTakeEvent );
        }
    }
}


} // namespace engine

