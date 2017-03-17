#include "platform/i_platform.h"
#include "network/waypoints_data_message.h"
#include "engine/waypoint_system.h"
#include "engine/engine.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

bool WaypointsDataMessage::operator==( WaypointsDataMessage const& other )
{
    return mWaypointsData == other.mWaypointsData
        && mClientId == other.mClientId;
}

WaypointsDataMessage::WaypointsDataMessage( engine::WaypointsData * waypointsData /*= nullptr*/, int32_t clientId /*= -1 */ )
    : mWaypointsData()
    , mClientId( clientId )
{
    if (waypointsData != nullptr)
    {
        std::ostringstream oss;
        eos::portable_oarchive oa( oss );
        oa& waypointsData;
        mWaypointsData = oss.str();
    }
}

WaypointsDataMessageSenderSystem::WaypointsDataMessageSenderSystem()
    : MessageSenderSystem()
{
}


void WaypointsDataMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void WaypointsDataMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

WaypointsDataMessageHandlerSubSystem::WaypointsDataMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void WaypointsDataMessageHandlerSubSystem::Init()
{
}

void WaypointsDataMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void WaypointsDataMessageHandlerSubSystem::Execute(Message const& message)
{
    WaypointsDataMessage const& msg=static_cast<WaypointsDataMessage const&>(message);
    if (msg.mClientId == mProgramState.mClientId || msg.mClientId == -1)
    {
        L2("Waypoints data arrived for me!\n" );
        if (!msg.mWaypointsData.empty())
        {
            static auto waypointS( engine::Engine::Get().GetSystem<engine::WaypointSystem>() );
            if (waypointS.IsValid())
            {
                std::istringstream iss( msg.mWaypointsData );
                eos::portable_iarchive ia( iss );
                engine::WaypointsData * waypointsData;
                ia >> waypointsData;
                waypointS->SetWaypointsData( *waypointsData );
            }
        }
    }
    
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__WaypointsDataMessage, network::WaypointsDataMessage );

