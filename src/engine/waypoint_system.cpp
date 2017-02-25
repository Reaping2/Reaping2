#include "platform/i_platform.h"
#include "waypoint_system.h"
#include "core/i_waypoint_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "collision_system.h"
#include "engine.h"
#include "core/i_controller_component.h"
#include "core/player_controller_component.h"
#include "ui/ui.h"

namespace engine {

WaypointSystem::WaypointSystem()
    : mScene( Scene::Get() )
{
}


void WaypointSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IWaypointComponent>().IsValid(); } );
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &WaypointSystem::OnMapStart, this, _1 ) );
}


void WaypointSystem::Update(double DeltaTime)
{
    if (mProgramState.mMode == core::ProgramState::Client)
    {
        return;
    }
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        auto waypointC= actor->Get<IWaypointComponent>();
        auto positionC( actor->Get<IPositionComponent>() );
        if (!positionC.IsValid())
        {
            return;
        }
        auto collisionC( actor->Get<ICollisionComponent>() );
        if (!collisionC.IsValid())
        {
            return;
        }
        static auto collisionSystem( ::engine::Engine::Get().GetSystem<engine::CollisionSystem>() );
        auto&& players( collisionSystem->GetAllCollidingActors( 
            glm::vec2(positionC->GetX(),positionC->GetY()), 
            collisionC->GetRadius(), 
            1 << CollisionClass::Player ) );
        for (auto&& player : players)
        {
            Opt<PlayerControllerComponent> pcc( player->Get<IControllerComponent>() );
            if (pcc.IsValid()&&pcc->mActivate.GetValue())
            {
                pcc->mActivate.Handled();
                mScene.Load( "rogue2" ); // for testing only
                L2( "Player used activate on a waypoint!\n" );
            }
        }
    }
}


void WaypointSystem::OnMapStart( core::MapStartEvent const& Evt )
{
}

} // namespace engine

