#include "platform/i_platform.h"
#include "explode_on_hit_system.h"
#include "core/i_explode_on_hit_component.h"
#include "explode_on_death_system.h"
#include "core/i_health_component.h"
#include "items/weapon_item_sub_system.h"

namespace engine {

ExplodeOnHitSystem::ExplodeOnHitSystem()
    : mScene( Scene::Get() )
{
}


void ExplodeOnHitSystem::Init()
{
}


void ExplodeOnHitSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IExplodeOnHitComponent> explodeOnHitC = actor.Get<IExplodeOnHitComponent>();
        if ( !explodeOnHitC.IsValid() || !explodeOnHitC->IsExploded() )
        {
            continue;
        }
        WeaponItemSubSystem::Projectiles_t projectiles;
        ExplodeOnDeathSystem::FillExplosionProjectiles( *explodeOnHitC.Get(), actor, projectiles );
        Scatter scatter;
        WeaponItemSubSystem::Get()->AddProjectiles( actor, projectiles, scatter );
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if( healthC.IsValid() )
        {
            healthC->SetHP( 0 );
        }
    }
}


} // namespace engine

