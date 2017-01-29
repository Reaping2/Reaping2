#include "platform/i_platform.h"
#include "worm_body_system.h"
#include "core/i_worm_body_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/i_move_component.h"

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
       auto positionC( actor->Get<IPositionComponent>() );
       auto moveC( actor->Get<IMoveComponent>() );
       auto collisionC( actor->Get<ICollisionComponent>() );
       auto head( mScene.GetActor( wormBodyC->GetHeadGUID() ) );
       auto followed( mScene.GetActor( wormBodyC->GetFollowedGUID() ) );
       if (followed.IsValid()) // not dead yet
       {
           auto followedPositionC( followed->Get<IPositionComponent>() );
           auto followedCollisionC( followed->Get<ICollisionComponent>() );
           auto followedMoveC( followed->Get<IMoveComponent>() );
           moveC->SetMoving( followedMoveC->GetMoving() );
           moveC->SetSpeed( followedMoveC->GetSpeed().Get() );

           wormBodyC->GetPrevPositions().push_front(
                { glm::vec2( followedPositionC->GetX(),followedPositionC->GetY() )
                     ,followedPositionC->GetOrientation() } );
           double dist = 0;
           double maxDist = followedCollisionC->GetRadius() + collisionC->GetRadius()*wormBodyC->GetDistancePercent();

           {
               auto posIt = wormBodyC->GetPrevPositions().begin();
               auto prevPosIt = posIt++;
               double lastDist = 0.0;
               for (auto e = wormBodyC->GetPrevPositions().end(); 
                    posIt != e && dist < maxDist;
                    prevPosIt = (dist < maxDist? posIt++ : prevPosIt) )
               {
                   auto const distanceV = glm::vec2( prevPosIt->mPosition - posIt->mPosition );
                   lastDist = std::sqrt( glm::dot( distanceV, distanceV ) );
                   dist += lastDist;
               }
               if (dist >= maxDist)
               {
                   BOOST_ASSERT( posIt != wormBodyC->GetPrevPositions().end() );
                   double distDiff = dist-maxDist;
                   double diffRatio = lastDist == 0 ? 0.0 : distDiff / lastDist;
                   auto pos = (prevPosIt->mPosition - posIt->mPosition)*diffRatio + posIt->mPosition;
                   positionC->SetX( pos.x );
                   positionC->SetY( pos.y );
                   positionC->SetOrientation( posIt->mOrientation );

                   ++posIt;
                   wormBodyC->GetPrevPositions().erase( posIt, wormBodyC->GetPrevPositions().end() );
               }
           }
       }
       moveC->SetHeading( positionC->GetOrientation() );
    }
}


} // namespace engine

