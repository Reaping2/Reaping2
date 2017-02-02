#include "platform/i_platform.h"
#include "worm_body_system.h"
#include "core/i_worm_body_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/i_worm_head_component.h"
#include "worm_head_system.h"

namespace engine {

WormBodySystem::WormBodySystem()
    : mScene( Scene::Get() )
{
}


void WormBodySystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IWormBodyComponent>().IsValid(); } );
}


void WormBodySystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        auto wormBodyC( actor->Get<IWormBodyComponent>() );
        if (!wormBodyC.IsValid())
        {
            continue;
        }
        auto wormHead( mScene.GetActor( wormBodyC->GetHeadGUID() ) );
        auto wormHeadC( wormHead->Get<IWormHeadComponent>() );
        auto healthC( actor->Get<IHealthComponent>() );
        if (!healthC->IsAlive())
        {
            CreateNewHead( wormHead, wormHeadC, actor );
            auto foundBodyIt = std::find(
                wormHeadC->GetBodyParts().begin(),
                wormHeadC->GetBodyParts().end(),
                actor->GetGUID() );
            wormHeadC->GetBodyParts().erase( foundBodyIt, wormHeadC->GetBodyParts().end() );

            mScene.RemoveActor( actor->GetGUID() ); // TODO: handle by setting a "handled" flag + remove_on_death comp! or hack with a spawn_on_death comp + actor with fade_out for rendering death
        }
    }
}


void WormBodySystem::CreateNewHead( Opt<Actor> wormHead, Opt<IWormHeadComponent> wormHeadC, Opt<Actor> actor )
{
    auto newWormHead( mActorFactory( wormHead->GetId() ) );
    auto newWormHeadC( newWormHead->Get<IWormHeadComponent>() );
    auto newGUID = newWormHead->GetGUID();

    newWormHeadC->CopyFrom( wormHeadC.Get() );
    auto foundBodyIt = std::find(
        newWormHeadC->GetBodyParts().begin(),
        newWormHeadC->GetBodyParts().end(),
        actor->GetGUID() );
    if (foundBodyIt != newWormHeadC->GetBodyParts().end()
        && ++foundBodyIt != newWormHeadC->GetBodyParts().end())
    {
        newWormHeadC->SetWaitDistance( WormHeadSystem::GetGapSize( newWormHead.get(), newWormHeadC ) );
        // positions before head are irrelevant, so they should be deleted
        auto const distance = std::distance( newWormHeadC->GetBodyParts().begin(), foundBodyIt );
        WormHeadSystem::ErasePrevPositions(
            newWormHeadC, 
            newWormHeadC->GetWaitDistance() * distance );
        newWormHeadC->GetBodyParts().erase( newWormHeadC->GetBodyParts().begin(), foundBodyIt );
        WormHeadSystem::InitNewHead( newWormHead.get() );
        auto newHealthC( newWormHead->Get<IHealthComponent>() );
        newHealthC->SetAlive( false );
        auto collisionC = newWormHead->Get<ICollisionComponent>();
        if (collisionC.IsValid())
        {
            collisionC->SetCollisionClass( CollisionClass::No_Collision );
        }
        auto renderableC = newWormHead->Get<IRenderableComponent>();
        if (renderableC.IsValid())
        {
            renderableC->SetLayerPriority( AutoId( "hide" ) );
        }
        mScene.AddActor( newWormHead.release() );
    }
}

} // namespace engine

