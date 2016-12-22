#include "platform/i_platform.h"
#include "detonate_on_hit_system.h"
#include "core/i_detonate_on_hit_component.h"
#include "core/i_explode_on_hit_component.h"
#include "core/box_collision_model.h"
#include "core/i_collision_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
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
        Opt<IPositionComponent> const positionC = actor->Get<IPositionComponent>();
        if ( !positionC.IsValid() )
        {
            continue;
        }
        CollisionModel::Object exploder{ glm::vec2( positionC->GetX(), positionC->GetY() ),
            glm::vec2(),
            collisionC->GetRadius() + detonateOnHitC->GetAddRadius() };
        Opt<IMoveComponent> moveC = actor->Get<IMoveComponent>();
        if( moveC.IsValid() )
        {
            exploder.speed = glm::vec2( moveC->GetSpeedX(), moveC->GetSpeedY() );
        }
        for( auto explodeIt : explodeActors )
        {
            Actor& explodeActor = *explodeIt;
            Opt<ICollisionComponent> explodeCollisionC = explodeActor.Get<ICollisionComponent>();
            Opt<IExplodeOnHitComponent> explodeOnHitC = explodeActor.Get<IExplodeOnHitComponent>();
            Opt<IPositionComponent> explodePositionC = explodeActor.Get<IPositionComponent>();
            if ( !explodeCollisionC.IsValid() || !explodeOnHitC.IsValid()
              || explodeOnHitC->IsExploded() || !explodePositionC.IsValid() )
            {
                continue;
            }
            CollisionModel::Object explodee{ glm::vec2( explodePositionC->GetX(), explodePositionC->GetY() ),
                glm::vec2(),
                explodeCollisionC->GetRadius() + explodeOnHitC->GetAddRadius() };
            Opt<IMoveComponent> explodeMoveC = explodeActor.Get<IMoveComponent>();
            if( explodeMoveC.IsValid() )
            {
                explodee.speed = glm::vec2( explodeMoveC->GetSpeedX(), explodeMoveC->GetSpeedY() );
            }
            if ( explodeOnHitC->GetDetonatorMaterial() == detonateOnHitC->GetMaterial() &&
                 BoxCollisionModel::AreActorsColliding_static( exploder, explodee, DeltaTime ) )
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

