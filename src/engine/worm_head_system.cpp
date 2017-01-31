#include "platform/i_platform.h"
#include "worm_head_system.h"
#include "core/i_worm_head_component.h"
#include "core/i_position_component.h"
#include "core/i_worm_body_component.h"
#include "core/i_collision_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"

namespace engine {

WormHeadSystem::WormHeadSystem()
    : mScene( Scene::Get() )
{
}


void WormHeadSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IWormHeadComponent>().IsValid(); } );
}


void WormHeadSystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
       auto wormHeadC(actor->Get<IWormHeadComponent>());
       if (!wormHeadC.IsValid())
       {
           continue;
       }
       auto positionC( actor->Get<IPositionComponent>() );
       if (!positionC.IsValid())
       {
           continue;
       }
       if (wormHeadC->IsFirstBuild())
       {
           for (int i = 0; i < wormHeadC->GetSpawnCount()-1; ++i)
           {
               auto bodyPart(mActorFactory( wormHeadC->GetBodyId() ));
               InitWormPart( *actor, *bodyPart );
               mScene.AddActor( bodyPart.release() );
           }
           auto tailPart(mActorFactory( wormHeadC->GetTailId() ));
           InitWormPart( *actor, *tailPart );
           mScene.AddActor( tailPart.release() );

           wormHeadC->SetFirstBuild( false );
           wormHeadC->SetSpawnCount( 0 );
       }
       //wormHeadC->SetSpawnCount( std::max( wormHeadC->GetSpawnCount() - 1, 0 ) );

       HandleDeath( actor, wormHeadC, DeltaTime );

       AddPrevPositions( positionC, wormHeadC );
       SetBodyPartPositions( actor, wormHeadC );
       SyncMove( actor, wormHeadC );

    }
}


void WormHeadSystem::SyncMove( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC )
{
    for (auto& partGUID : wormHeadC->GetBodyParts())
    {
        auto part( mScene.GetActor( partGUID ) );
        auto moveC( actor->Get<IMoveComponent>() );
        auto partMoveC( part->Get<IMoveComponent>() );
        if (partMoveC.IsValid())
        {
            partMoveC->SetMoving( moveC->GetMoving() );
            partMoveC->SetSpeed( 0.0 );
        }
    }
}

void WormHeadSystem::SetBodyPartPositions( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC )
{
    auto healthC( actor->Get<IHealthComponent>() );
    if (healthC->IsAlive() && wormHeadC->GetWaitDistance() > 0.0)
    {
        return;
    }
    double const gapSize = GetGapSize( actor, wormHeadC );

    auto posE = wormHeadC->GetPrevPositions().end();
    auto posIt = wormHeadC->GetPrevPositions().begin();
    auto prevPosIt = posIt++;

    auto& bodyParts = wormHeadC->GetBodyParts();
    auto bodyPartsE = bodyParts.end();
    auto bodyPartsIt = bodyParts.begin();

    double dist = 0;
    while (posIt != posE && bodyPartsIt != bodyPartsE)
    {
        dist += posIt->mDistance;
        if (dist >= gapSize)
        {
            auto actor( mScene.GetActor( *bodyPartsIt ) );
            if (actor.IsValid())
            {
                auto partPositionC( actor->Get<IPositionComponent>() );
                partPositionC->SetX( prevPosIt->mPosition.x );
                partPositionC->SetY( prevPosIt->mPosition.y );
                partPositionC->SetOrientation( prevPosIt->mOrientation );
            }
            ++bodyPartsIt;
            dist -= gapSize;
        }
        prevPosIt = posIt++;
    }
    L1( "PrevPositions length: %d\n", wormHeadC->GetPrevPositions().size() );
    wormHeadC->GetPrevPositions().erase( posIt, wormHeadC->GetPrevPositions().end() );

}


double WormHeadSystem::GetGapSize( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC ) const
{
    auto collisionC( actor->Get<ICollisionComponent>() );
    return (collisionC->GetRadius() * 2)*wormHeadC->GetGapPercent();
}

void WormHeadSystem::AddPrevPositions( Opt<IPositionComponent> positionC, Opt<IWormHeadComponent> wormHeadC )
{
    auto const posV = glm::vec2( positionC->GetX(), positionC->GetY() );
    if (!wormHeadC->GetPrevPositions().empty())
    {
        double const minDist = 2.0;

        auto const& front = wormHeadC->GetPrevPositions().front();
        auto frontV = front.mPosition;
        auto const distanceV = posV - frontV;
        auto dist = std::sqrt( glm::dot( distanceV, distanceV ) );
        auto const normV = glm::normalize( distanceV )*minDist;
        auto orientation = front.mOrientation;
        auto const orientationAdd = (positionC->GetOrientation() - orientation) / std::floor( dist / minDist );
        while (dist > minDist)
        {
            dist -= minDist;
            frontV += normV;
            orientation += orientationAdd;
            wormHeadC->GetPrevPositions().push_front( { frontV, orientation, minDist } );
            wormHeadC->SetWaitDistance( std::max(wormHeadC->GetWaitDistance() - minDist, 0.0) );
        }
    }
    else
    {
        wormHeadC->GetPrevPositions().push_front( { posV, positionC->GetOrientation(), 0.0 } );
    }
}

void WormHeadSystem::InitWormPart( Actor& head, Actor& part )
{
    auto positionC( head.Get<IPositionComponent>() );
    auto partPositionC( part.Get<IPositionComponent>() );
    if (!partPositionC.IsValid())
    {
        return;
    }

    partPositionC->SetX( positionC->GetX() );
    partPositionC->SetY( positionC->GetY() );
    auto wormHeadC( head.Get<IWormHeadComponent>() );
    auto& bodyParts = wormHeadC->GetBodyParts();

    auto partBodyC( part.Get<IWormBodyComponent>() );
    partBodyC->SetHeadGUID( head.GetGUID() );
    partBodyC->SetFollowedGUID( bodyParts.empty()?head.GetGUID():bodyParts.back() );

    bodyParts.push_back( part.GetGUID() );
}


void WormHeadSystem::InitNewHead( Actor& oldHead, Actor& newHead )
{
    auto oldWormHeadC( oldHead.Get<IWormHeadComponent>() );
    auto newWormHeadC( newHead.Get<IWormHeadComponent>() );
    auto newGUID = newHead.GetGUID();
    
    newWormHeadC->Copy( oldWormHeadC.Get() );
    newWormHeadC->SetActorGUID( newGUID );

    if (!newWormHeadC->GetBodyParts().empty())
    {
        auto front(mScene.GetActor(newWormHeadC->GetBodyParts().front()));
        auto wormBodyC( front->Get<IWormBodyComponent>() );
        if (wormBodyC.IsValid())
        {
            wormBodyC->SetFollowedGUID( newGUID );
        }
        auto frontPositionC( front->Get<IPositionComponent>() );
        auto newPositionC( newHead.Get<IPositionComponent>() );
        newPositionC->SetX( frontPositionC->GetX() );
        newPositionC->SetY( frontPositionC->GetY() );
    }
    for (auto partGUID : newWormHeadC->GetBodyParts())
    {
        auto actor( mScene.GetActor( partGUID ) );
        if (actor.IsValid())
        {
            auto wormBodyC( actor->Get<IWormBodyComponent>() );
            if (wormBodyC.IsValid())
            {
                wormBodyC->SetHeadGUID( newGUID );
            }
        }
    }

    double const gapSize = newWormHeadC->GetWaitDistance();
    double dist = 0;
    auto posIt = newWormHeadC->GetPrevPositions().begin();
    auto posE = newWormHeadC->GetPrevPositions().end();
    while (dist < gapSize && posIt != posE)
    {
        dist += posIt->mDistance;
        ++posIt;
    }
    newWormHeadC->GetPrevPositions().erase( newWormHeadC->GetPrevPositions().begin(), posIt );
}

void WormHeadSystem::HandleDeath( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC, double DeltaTime )
{
    auto healthC(actor->Get<IHealthComponent>());
    if (!healthC->IsAlive() && wormHeadC->GetSpawnDelay() <= 0.0)
    {
        wormHeadC->SetSpawnDelay( wormHeadC->GetSpawnDelayMax() );
    }
    else
    {
        wormHeadC->SetSpawnDelay( std::max( wormHeadC->GetSpawnDelay() - DeltaTime, 0.0 ) );
    }

    if (!healthC->IsAlive() && wormHeadC->GetSpawnDelay() <= 0.0)
    {
        wormHeadC->SetWaitDistance( GetGapSize( actor, wormHeadC ) );
        auto newHead( mActorFactory( actor->GetId() ) );
        InitNewHead( *actor, *newHead );
        mScene.AddActor( newHead.release() );
        mScene.RemoveActor( actor->GetGUID() );

    }
}

} // namespace engine

