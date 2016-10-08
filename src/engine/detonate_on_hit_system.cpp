#include "platform/i_platform.h"
#include "detonate_on_hit_system.h"
#include "core/i_detonate_on_hit_component.h"
#include "core/i_explode_on_hit_component.h"
#include "core/box_collision_model.h"
#include "core/i_collision_component.h"
#include "core/i_health_component.h"
#include "explode_on_hit_system.h"

namespace engine {

DetonateOnHitSystem::DetonateOnHitSystem()
    : mScene( Scene::Get() )
{
}


void DetonateOnHitSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IDetonateOnHitComponent>().IsValid()
            && actor.Get<ICollisionComponent>().IsValid(); } );
}


void DetonateOnHitSystem::Update( double DeltaTime )
{
    typedef std::list< Opt<Actor> > ExplodeActorList_t;
    ExplodeActorList_t explodeActors;
    for( auto actor : mScene.GetActorsFromMap( ExplodeOnHitSystem::GetType_static() ) )
    {
        Opt<IExplodeOnHitComponent> explodeOnHitC = actor->Get<IExplodeOnHitComponent>();
        if ( !explodeOnHitC.IsValid() )
        {
            continue;
        }
        explodeActors.push_back( actor );
    }
    if ( explodeActors.empty() )
    {
        return;
    }
    for ( auto actor : mScene.GetActorsFromMap( GetType_static() ) )
    {
        Opt<IDetonateOnHitComponent> detonateOnHitC = actor->Get<IDetonateOnHitComponent>();
        if ( !detonateOnHitC.IsValid() )
        {
            continue;
        }
        Opt<ICollisionComponent> collisionC = actor->Get<ICollisionComponent>();
        if ( !collisionC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if ( healthC.IsValid() && !healthC->IsAlive() )
        {
            continue;
        }
        for( auto explodeIt : explodeActors )
        {
            Actor& explodeActor = *explodeIt;
            Opt<ICollisionComponent> explodeCollisionC = explodeActor.Get<ICollisionComponent>();
            Opt<IExplodeOnHitComponent> explodeOnHitC = explodeActor.Get<IExplodeOnHitComponent>();
            if ( !explodeCollisionC.IsValid() || !explodeOnHitC.IsValid() || explodeOnHitC->IsExploded() )
            {
                continue;
            }
            if ( explodeOnHitC->GetDetonatorMaterial() == detonateOnHitC->GetMaterial() &&
                 BoxCollisionModel::AreActorsColliding( *actor, explodeActor
                         , collisionC->GetRadius() + detonateOnHitC->GetAddRadius()
                         , explodeCollisionC->GetRadius() + explodeOnHitC->GetAddRadius(), DeltaTime ) )
            {
                explodeOnHitC->SetExploded( true );
                if( healthC.IsValid() && detonateOnHitC->IsRemoveOnHit() )
                {
                    healthC->SetHP( 0 );
                }
            }
        }
    }
}


} // namespace engine

