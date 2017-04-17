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
#include "items/flash_event.h"
#include "core/i_activatable_component.h"

namespace engine {

WaypointSystem::WaypointSystem()
    : mWaypointModel( "waypoint", &RootModel::Get() )
    , mWaypointNamesModel( (ModelValue::get_string_vec_t) RefTo( mWaypointsData.mWaypointNames ), "names", &mWaypointModel )
    , mWaypointSelectModel ( StringFunc( this, &WaypointSystem::OnWaypointSelect), "select", &mWaypointModel )
{
}

const std::string WaypointSystem::NextStage = "Next Stage";
const std::string WaypointSystem::Resume = "Resume";
const std::string WaypointSystem::WaypointName = "Waypoint";

void WaypointSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IWaypointComponent>().IsValid(); } );
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( std::bind( &WaypointSystem::OnMapStart, this, std::placeholders::_1 ) );
    mOnWaypointChanged = EventServer<WaypointChangedEvent>::Get().Subscribe( std::bind( &WaypointSystem::OnWaypointChanged, this, std::placeholders::_1 ) );
    mWaypointsData.mWaypointNames = { NextStage, Resume };
}


void WaypointSystem::Update(double DeltaTime)
{
    if (mProgramState.mMode != core::ProgramState::Client)
    {
        for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
        {
            auto&& players = GetCollidingPlayers( actor->GetGUID() );
            if (!players.empty())
            {
                auto found = std::find_if( mWaypointsData.mWaypointGUIDs.begin(), mWaypointsData.mWaypointGUIDs.end(), [&actor]( auto&& p ) { return p.second == actor->GetGUID(); } );
                if (found == mWaypointsData.mWaypointGUIDs.end())
                {
                    auto const name = WaypointName + " " + std::to_string( mWaypointsData.mNextWaypointCounter );
                    EventServer<WaypointChangedEvent>::Get().SendEvent( { name, actor->GetGUID(), (*players.begin())->GetGUID(), WaypointChangedEvent::Lit } );
                }
            }
            auto activatableC( actor->Get<IActivatableComponent>() );
            if (activatableC.IsValid() && activatableC->IsActivated())
            {
                EventServer<WaypointChangedEvent>::Get().SendEvent( { "", actor->GetGUID(), activatableC->GetActivatorGUID(),WaypointChangedEvent::Choose } );
            }
        }
    }
}


void WaypointSystem::OnWaypointSelect( std::string waypointName )
{
    EventServer<WaypointChangedEvent>::Get().SendEvent({ 
        waypointName, 
        waypointName == NextStage ? mWaypointsData.mPlayerToWaypoint[mProgramState.mControlledActorGUID] : mWaypointsData.mWaypointGUIDs[waypointName],
        mProgramState.mControlledActorGUID,
        WaypointChangedEvent::Selected } );
}

void WaypointSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if (Evt.mState == core::MapStartEvent::Started)
    {
        mWaypointsData.mNextWaypointCounter = 1;
        mWaypointsData.mWaypointNames = { NextStage, Resume };
        mWaypointsData.mWaypointGUIDs = { { NextStage, -1 }, { Resume,-1 } };
        mWaypointsData.mPlayerToWaypoint.clear();
    }
}


void WaypointSystem::OnWaypointChanged( WaypointChangedEvent const& Evt )
{
    if (Evt.mState == WaypointChangedEvent::Lit)
    {
        mWaypointsData.mWaypointNames.push_back( Evt.mWaypointName );
        mWaypointsData.mWaypointGUIDs[Evt.mWaypointName] = Evt.mWaypointGUID;
        ++mWaypointsData.mNextWaypointCounter;
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
                mWaypointsData.mPlayerToWaypoint[Evt.mPlayerGUID] = Evt.mWaypointGUID;
            }
        }
    }
    else if (Evt.mState == WaypointChangedEvent::Selected)
    {
        mWaypointsData.mPlayerToWaypoint.erase( Evt.mPlayerGUID );
        auto player( mScene.GetActor( Evt.mPlayerGUID ) );
        if (mProgramState.mMode != core::ProgramState::Client)
        {
            if (Evt.mWaypointName == NextStage)
            {
                auto&& players = GetCollidingPlayers( Evt.mWaypointGUID );

                if (players.size() >= GetAlivePlayerCount())
                {
                    mScene.Load( "rogue2" ); // for testing only 2
                    return;
                }
            }
            if (Evt.mWaypointName != Resume)
            {
                auto waypoint( mScene.GetActor( Evt.mWaypointGUID ) );
                if (waypoint.IsValid() && player.IsValid())
                {
                    auto wpPositionC( waypoint->Get<IPositionComponent>() );
                    auto positionC( player->Get<IPositionComponent>() );
                    auto originalX = positionC->GetX();
                    auto originalY = positionC->GetY();
                    if (wpPositionC.IsValid() && positionC.IsValid())
                    {
                        positionC->SetX( wpPositionC->GetX() );
                        positionC->SetY( wpPositionC->GetY() );
                    }
                    EventServer<FlashEvent>::Get().SendEvent( FlashEvent( player->GetGUID(), originalX, originalY, positionC->GetX(), positionC->GetY() ) );
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


std::set<Actor*> WaypointSystem::GetCollidingPlayers( int32_t waypointGUID )
{
    static auto collisionSystem( ::engine::Engine::Get().GetSystem<engine::CollisionSystem>() );

    auto waypoint( mScene.GetActor( waypointGUID ) );
    if (!waypoint.IsValid())
    {
        return std::set<Actor*>();
    }
    auto positionC( waypoint->Get<IPositionComponent>() );
    if (!positionC.IsValid())
    {
        return std::set<Actor*>();
    }
    auto collisionC( waypoint->Get<ICollisionComponent>() );
    if (!collisionC.IsValid())
    {
        return std::set<Actor*>();
    }
    return std::move(collisionSystem->GetAllCollidingActors(
        glm::vec2( positionC->GetX(), positionC->GetY() ),
        collisionC->GetRadius(),
        1 << CollisionClass::Player ));

}


int32_t WaypointSystem::GetAlivePlayerCount()
{
    return std::count_if( mProgramState.mClientDatas.begin(), mProgramState.mClientDatas.end(),
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
}

void WaypointSystem::SetWaypointsData( WaypointsData const& waypointsData )
{
    mWaypointsData = waypointsData;
}


WaypointsData& WaypointSystem::GetWaypointsData()
{
    return mWaypointsData;
}

} // namespace engine

