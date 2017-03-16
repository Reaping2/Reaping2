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
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( std::bind( &WaypointSystem::OnMapStart, this, std::placeholders::_1 ) );
    mOnWaypointChanged = EventServer<WaypointChangedEvent>::Get().Subscribe( std::bind( &WaypointSystem::OnWaypointChanged, this, std::placeholders::_1 ) );
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
            if (!players.empty())
            {
                auto found = std::find_if( mWaypointGUIDs.begin(), mWaypointGUIDs.end(), [&actor]( auto&& p ) { return p.second == actor->GetGUID(); } );
                if (found == mWaypointGUIDs.end())
                {
                    auto const name = "Waypoint " + std::to_string( mNextWaypointCounter );
                    EventServer<WaypointChangedEvent>::Get().SendEvent( { name, actor->GetGUID(), (*players.begin())->GetGUID(), WaypointChangedEvent::Lit } );
                }
            }
            for (auto&& player : players)
            {
                Opt<PlayerControllerComponent> pcc( player->Get<IControllerComponent>() );
                if (pcc.IsValid() && pcc->mActivate.GetValue())
                {
                    EventServer<WaypointChangedEvent>::Get().SendEvent( { "", actor->GetGUID(), player->GetGUID(),WaypointChangedEvent::Choose } );
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
    EventServer<WaypointChangedEvent>::Get().SendEvent({ 
        waypointName, 
        waypointName == "Next Stage" ? mPlayerToWaypoint[mProgramState.mControlledActorGUID] : mWaypointGUIDs[waypointName],
        mProgramState.mControlledActorGUID,
        WaypointChangedEvent::Selected } );
}

void WaypointSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if (Evt.mState == core::MapStartEvent::Started)
    {
        mNextWaypointCounter = 1;
        mWaypointNames = { "Next Stage", "Resume" };
        mWaypointGUIDs = { { "Nest Stage", -1 }, { "Resume",-1 } };
        mPlayerToWaypoint.clear();
    }
}


void WaypointSystem::OnWaypointChanged( WaypointChangedEvent const& Evt )
{
    if (Evt.mState == WaypointChangedEvent::Lit)
    {
        mWaypointNames.push_back( Evt.mWaypointName );
        mWaypointGUIDs[Evt.mWaypointName] = Evt.mWaypointGUID;
        ++mNextWaypointCounter;
        auto waypoint( mScene.GetActor( Evt.mWaypointGUID ) );
        if (waypoint.IsValid())
        {
            auto waypointC( waypoint->Get<IWaypointComponent>());
            if (waypointC.IsValid())
            {
                waypointC->SetLit( true );
            }
        }
    }
    else if (Evt.mState == WaypointChangedEvent::Choose)
    {
        auto player( mScene.GetActor( Evt.mPlayerGUID ) );
        if (player.IsValid())
        {
            Opt<PlayerControllerComponent> pcc( player->Get<IControllerComponent>() );
            if (pcc.IsValid())
            {
                pcc->mActivate.Handled();
                pcc->SetEnabled( false );
                Ui::Get().Load( "select_waypoint" );
                mPlayerToWaypoint[Evt.mPlayerGUID] = Evt.mWaypointGUID;
            }
        }
    }
    else if (Evt.mState == WaypointChangedEvent::Selected)
    {
        mPlayerToWaypoint.erase( Evt.mPlayerGUID );
        static auto collisionSystem( ::engine::Engine::Get().GetSystem<engine::CollisionSystem>() );
        auto player( mScene.GetActor( Evt.mPlayerGUID ) );
        if (Evt.mWaypointName == "Next Stage"
            && mProgramState.mMode != core::ProgramState::Client)
        {
            auto waypoint( mScene.GetActor( Evt.mWaypointGUID ) );
            if (!waypoint.IsValid())
            {
                return;
            }
            auto positionC( waypoint->Get<IPositionComponent>() );
            if (!positionC.IsValid())
            {
                return;
            }
            auto collisionC( waypoint->Get<ICollisionComponent>() );
            if (!collisionC.IsValid())
            {
                return;
            }
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

            if (players.size() >= cnt)
            {
                mScene.Load( "rogue2" ); // for testing only 2
                return;
            }
        }
        if (mProgramState.mMode != core::ProgramState::Client)
        {
            if (Evt.mWaypointName == "Resume")
            {
            }
            else 
            {
                auto waypoint( mScene.GetActor( Evt.mWaypointGUID ) );
                if (waypoint.IsValid() && player.IsValid())
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
        }
        Ui::Get().Load( mProgramState.mHUD );
        auto pcc( player->Get<IControllerComponent>() );
        if (pcc.IsValid())
        {
            pcc->SetEnabled( true );
        }
    }
}

} // namespace engine

