#include "platform/i_platform.h"
#include "worm_head_system.h"
#include "core/i_worm_head_component.h"
#include "core/i_position_component.h"

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
    bodyParts.push_back( part.GetGUID() );
}

} // namespace engine

