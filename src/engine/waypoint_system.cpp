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
#include "core/i_health_component.h"

namespace engine {

WaypointSystem::WaypointSystem()
    : mWaypointModel( "waypoint", &RootModel::Get() )
    , mWaypointNamesModel( (ModelValue::get_string_vec_t) RefTo(mWaypointNames), "names", &mWaypointModel)
    , mWaypointSelectModel ( StringFunc( this, &WaypointSystem::OnWaypointSelect), "select", &mWaypointModel )
{
}


void WaypointSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IWaypointComponent>().IsValid(); } );
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &WaypointSystem::OnMapStart, this, _1 ) );
    mWaypointNames = { "Waypoint 1", "Waypoint 2", "Next Stage" };
}


void WaypointSystem::Update(double DeltaTime)
{
    if (mProgramState.mMode != core::ProgramState::Client)
    {
        for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
        {
            auto waypointC = actor->Get<IWaypointComponent>();
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
                glm::vec2( positionC->GetX(), positionC->GetY() ),
                collisionC->GetRadius(),
                1 << CollisionClass::Player ) );
            auto cnt = std::count_if( mProgramState.mClientDatas.begin(), mProgramState.mClientDatas.end(),
                [&]( core::ClientData const& clientData )
                {
                    auto player( mScene.GetActor( clientData.mClientActorGUID ) );
                    if (!player.IsValid())
                    {
                        return false;
                    }
                    auto healthC( player->Get<IHealthComponent>() );
                    if (!healthC.IsValid())
                    {
                        return false;
                    }

                    return healthC->IsAlive();
                } );
            if (!players.empty())
            {
                auto found = std::find_if( mWaypointGUIDs.begin(), mWaypointGUIDs.end(), [&actor]( auto&& p ) { return p.second == actor->GetGUID(); } );
                if (found == mWaypointGUIDs.end())
                {
                    auto name = "Waypoint " + std::to_string( mNextWaypointCounter );
                    mWaypointNames.push_back( name );
                    mWaypointGUIDs[name] = actor->GetGUID();
                    ++mNextWaypointCounter;
                    waypointC->SetLit( true );
                }
            }
            if (players.size() >= cnt) //there could be dead players on the waypoint too
            {
                for (auto&& player : players)
                {
                    Opt<PlayerControllerComponent> pcc( player->Get<IControllerComponent>() );
                    if (pcc.IsValid() && pcc->mActivate.GetValue())
                    {
                        pcc->mActivate.Handled();
                        pcc->SetEnabled( false );
                        Ui::Get().Load( "select_waypoint" );
                        //mScene.Load( "rogue2" ); // for testing only
                        //L2( "Player used activate on a waypoint!\n" );
                        //return;
                    }
                }
            }
        }
    }
    for (auto& clientData : mProgramState.mClientDatas)
    {
        auto player( mScene.GetActor( clientData.mClientActorGUID ) );
        if (!player.IsValid())
        {
            continue;
        }
        Opt<PlayerControllerComponent> pcc( player->Get<IControllerComponent>() );
        if (pcc.IsValid() && pcc->mActivate.GetValue())
        {
            pcc->mActivate.Handled();
        }
    }

}


void WaypointSystem::OnWaypointSelect( std::string waypointName )
{
    auto player( mScene.GetActor( mProgramState.mControlledActorGUID ) );
    if (waypointName == "Next Stage")
    {
        mScene.Load( "rogue2" ); // for testing only 2
        return;
    }
    if (waypointName == "Resume")
    {
    }
    else
    {
        auto waypoint( mScene.GetActor( mWaypointGUIDs[waypointName] ) );
        if (waypoint.IsValid()&&player.IsValid())
        {
            auto wpPositionC( waypoint->Get<IPositionComponent>() );
            auto positionC( player->Get<IPositionComponent>() );
            if (wpPositionC.IsValid() && positionC.IsValid())
            {
                positionC->SetX( wpPositionC->GetX() );
                positionC->SetY( wpPositionC->GetY() );
            }

        }
    }
    Ui::Get().Load( mProgramState.mHUD );
    auto pcc( player->Get<IControllerComponent>() );
    if (pcc.IsValid())
    {
        pcc->SetEnabled( true );
    }
}

void WaypointSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if (Evt.mState == core::MapStartEvent::Started)
    {
        mNextWaypointCounter = 1;
        mWaypointNames = { "Next Stage", "Resume" };
        mWaypointGUIDs = { { "Nest Stage", -1 }, { "Resume",-1 } };
    }
}

} // namespace engine

