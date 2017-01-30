#include "platform/i_platform.h"
#include "worm_head_system.h"
#include "core/i_worm_head_component.h"
#include "core/i_position_component.h"
#include "core/i_worm_body_component.h"
#include "core/i_collision_component.h"
#include "core/i_move_component.h"

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
       wormHeadC->SetSpawnCount( std::max( wormHeadC->GetSpawnCount() - 1, 0 ) );

       AddPrevPositions( positionC, wormHeadC );

       SetBodyPartPositions( actor, wormHeadC );


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
}


void WormHeadSystem::SetBodyPartPositions( Opt<Actor> actor, Opt<IWormHeadComponent> wormHeadC )
{
    auto collisionC( actor->Get<ICollisionComponent>() );
    double const maxDist = (collisionC->GetRadius() + collisionC->GetRadius())*0.8;/*wormHeadC->GetDistancePercent();*/

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
        if (dist >= maxDist)
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
            dist -= maxDist;
        }
        prevPosIt = posIt++;
    }
    L1( "PrevPositions length: %d\n", wormHeadC->GetPrevPositions().size() );
    wormHeadC->GetPrevPositions().erase( posIt, wormHeadC->GetPrevPositions().end() );

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
    partBodyC->GetPrevPositions().push_front(
        {glm::vec2(positionC->GetX(),positionC->GetY())
            , positionC->GetOrientation(), 0.0 } );

    bodyParts.push_back( part.GetGUID() );
}

} // namespace engine

