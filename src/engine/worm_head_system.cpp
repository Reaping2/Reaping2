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
       HandleFirstBuild( actor, wormHeadC, positionC );
       HandleDeath( actor, wormHeadC, DeltaTime );
       HandleLengthIncrease( actor, wormHeadC, positionC, DeltaTime );
       AddPrevPositions( actor, positionC, wormHeadC );
       SetBodyPartPositions( actor, wormHeadC );
       SyncMove( actor, wormHeadC );

    }
}


void WormHeadSystem::HandleFirstBuild( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC, Opt<IPositionComponent> positionC )
{
    if (wormHeadC->IsFirstBuild())
    {
        for (int i = 0; i < wormHeadC->GetSpawnCount() - 1; ++i)
        {
            auto bodyPart( mActorFactory( wormHeadC->GetBodyId() ) );
            InitWormPart( actor, bodyPart.get(), positionC );
            mScene.AddActor( bodyPart.release() );
        }
        auto tailPart( mActorFactory( wormHeadC->GetTailId() ) );
        InitWormPart( actor, tailPart.get(), positionC );
        mScene.AddActor( tailPart.release() );

        wormHeadC->SetFirstBuild( false );
        wormHeadC->SetSpawnCount( 0 );
        wormHeadC->SetLength( std::max( wormHeadC->GetLength() - wormHeadC->GetLengthDecrease(), 0 ) );
    }
}

void WormHeadSystem::HandleLengthIncrease( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC, Opt<IPositionComponent> positionC, double DeltaTime )
{
    auto healthC( actor->Get<IHealthComponent>() );
    if (!healthC->IsAlive())
    {
        return;
    }
    if (wormHeadC->GetBodyParts().size() < wormHeadC->GetLength())
    {
        if (wormHeadC->GetSpawnDelay() <= 0.0)
        {
            wormHeadC->SetSpawnDelay( wormHeadC->GetSpawnDelayMax() );
            if (wormHeadC->GetSpawnCount() == 0)
            {
                wormHeadC->SetSpawnCount( wormHeadC->GetLength() - wormHeadC->GetBodyParts().size() );
            }
        }
        else
        {
            wormHeadC->SetSpawnDelay( std::max( wormHeadC->GetSpawnDelay() - DeltaTime, 0.0 ) );
        }
    }
    if (wormHeadC->GetSpawnDelay() <= 0.0 && wormHeadC->GetSpawnCount() > 0)
    {
        auto posC = positionC;
        if (!wormHeadC->GetBodyParts().empty())
        {
            posC = mScene.GetActor( wormHeadC->GetBodyParts().back() )->Get<IPositionComponent>();
        }
        auto tailPart( mActorFactory( wormHeadC->GetSpawnCount()>1 ? wormHeadC->GetBodyId() : wormHeadC->GetTailId() ) );
        InitWormPart( actor, tailPart.get(), posC );
        mScene.AddActor( tailPart.release() );
        wormHeadC->SetSpawnCount( wormHeadC->GetSpawnCount() - 1 );
        if (wormHeadC->GetSpawnCount() == 0)
        {
            wormHeadC->SetLength( std::max( wormHeadC->GetLength() - wormHeadC->GetLengthDecrease(), 0 ) );
        }
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
    if (wormHeadC->GetWaitDistance() > 0.0)
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
    wormHeadC->GetPrevPositions().erase( posIt, wormHeadC->GetPrevPositions().end() );

}


double WormHeadSystem::GetGapSize( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC )
{
    auto collisionC( actor->Get<ICollisionComponent>() );
    return (collisionC->GetRadius() * 2)*wormHeadC->GetGapPercent();
}

void WormHeadSystem::AddPrevPositions( Opt<Actor> actor, Opt<IPositionComponent> positionC, Opt<IWormHeadComponent> wormHeadC )
{
    auto healthC( actor->Get<IHealthComponent>() );
    if (!healthC->IsAlive())
    {
        return;
    }
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

void WormHeadSystem::InitWormPart( Opt<Actor> head, Opt<Actor> part, Opt<IPositionComponent> positionC )
{
    auto partPositionC( part->Get<IPositionComponent>() );
    if (!partPositionC.IsValid())
    {
        return;
    }

    partPositionC->SetX( positionC->GetX() );
    partPositionC->SetY( positionC->GetY() );
    partPositionC->SetOrientation( positionC->GetOrientation() );
    auto wormHeadC( head->Get<IWormHeadComponent>() );
    auto& bodyParts = wormHeadC->GetBodyParts();

    auto partBodyC( part->Get<IWormBodyComponent>() );
    partBodyC->SetHeadGUID( head->GetGUID() );
    partBodyC->SetFollowedGUID( bodyParts.empty()?head->GetGUID():bodyParts.back() );

    bodyParts.push_back( part->GetGUID() );
}


void WormHeadSystem::InitNewHead( Opt<Actor> newHead )
{
    static auto& mScene = Scene::Get();

    auto newWormHeadC( newHead->Get<IWormHeadComponent>() );

    auto newGUID = newHead->GetGUID();
    if (!newWormHeadC->GetBodyParts().empty())
    {
        auto front(mScene.GetActor(newWormHeadC->GetBodyParts().front()));
        auto wormBodyC( front->Get<IWormBodyComponent>() );
        if (wormBodyC.IsValid())
        {
            wormBodyC->SetFollowedGUID( newGUID );
        }
        auto frontPositionC( front->Get<IPositionComponent>() );
        auto newPositionC( newHead->Get<IPositionComponent>() );
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
        if (!wormHeadC->GetBodyParts().empty())
        {
            auto newWormHead( mActorFactory( actor->GetId() ) );
            auto newWormHeadC( newWormHead->Get<IWormHeadComponent>() );
            newWormHeadC->CopyFrom( wormHeadC.Get() );
            InitNewHead( newWormHead.get() );
            newWormHeadC->SetWaitDistance( GetGapSize( newWormHead.get(), newWormHeadC ) );
            ErasePrevPositions( newWormHeadC, newWormHeadC->GetWaitDistance() );
            mScene.AddActor( newWormHead.release() );
        }
        mScene.RemoveActor( actor->GetGUID() ); // TODO: handle by setting a "handled" flag + remove_on_death comp! or hack with a spawn_on_death comp + actor with fade_out for rendering death

    }
}

void WormHeadSystem::ErasePrevPositions( Opt<IWormHeadComponent> wormHeadC, double eraseDistance )
{
    double dist = 0;
    auto posIt = wormHeadC->GetPrevPositions().begin();
    auto posE = wormHeadC->GetPrevPositions().end();
    while (dist < eraseDistance && posIt != posE)
    {
        dist += posIt->mDistance;
        ++posIt;
    }

    wormHeadC->GetPrevPositions().erase( wormHeadC->GetPrevPositions().begin(), posIt );
}

} // namespace engine

