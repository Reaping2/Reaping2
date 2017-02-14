#include "platform/i_platform.h"
#include "explosion_system.h"
#include "core/i_explosion_component.h"
#include "core/i_collision_component.h"
#include "system_suppressor.h"

namespace engine {

ExplosionSystem::ExplosionSystem()
    : mScene( Scene::Get() )
{
}


void ExplosionSystem::Init()
{
    SystemSuppressor::Get().Add( SystemSuppressor::SceneLoad, GetType_static() );

    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IExplosionComponent>().IsValid()
            && actor.Get<ICollisionComponent>().IsValid(); } );
}


void ExplosionSystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IExplosionComponent> explosionC = actor->Get<IExplosionComponent>();
        if ( !explosionC.IsValid() )
        {
            continue;
        }
        Opt<ICollisionComponent> collisionC = actor->Get<ICollisionComponent>();
        if ( !collisionC.IsValid() )
        {
            continue;
        }
        double radius = collisionC->GetRadius();
        radius += DeltaTime * explosionC->GetScaleSpeed();
        collisionC->SetRadius( std::min( radius, explosionC->GetMaxRadius() ) );
    }
}


} // namespace engine

