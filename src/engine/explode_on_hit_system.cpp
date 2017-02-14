#include "platform/i_platform.h"
#include "explode_on_hit_system.h"
#include "core/i_explode_on_hit_component.h"
#include "explode_on_death_system.h"
#include "core/i_health_component.h"
#include "items/weapon_item_sub_system.h"
#include "system_suppressor.h"

namespace engine {

ExplodeOnHitSystem::ExplodeOnHitSystem()
    : mScene( Scene::Get() )
{
}


void ExplodeOnHitSystem::Init()
{
    SystemSuppressor::Get().Add( SystemSuppressor::SceneLoad, GetType_static() );

    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IExplodeOnHitComponent>().IsValid(); } );
}


void ExplodeOnHitSystem::Update( double DeltaTime )
{
    for( auto actor : mScene.GetActorsFromMap( GetType_static() ) )
    {
        Opt<IExplodeOnHitComponent> explodeOnHitC = actor->Get<IExplodeOnHitComponent>();
        if ( !explodeOnHitC.IsValid() || !explodeOnHitC->IsExploded() )
        {
            continue;
        }
        WeaponItemSubSystem::Projectiles_t projectiles;
        ExplodeOnDeathSystem::FillExplosionProjectiles( *explodeOnHitC.Get(), *actor, projectiles );
        Scatter scatter;
        WeaponItemSubSystem::Get()->AddProjectiles( *actor, projectiles, scatter );
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if( healthC.IsValid() )
        {
            healthC->SetHP( 0 );
        }
    }
}


} // namespace engine

