#include "platform/i_platform.h"
#include "network/map_load_message.h"

namespace network {

bool MapLoadMessage::operator==( MapLoadMessage const& other )
{
    return mMapName == other.mMapName;
}
MapLoadMessageSenderSystem::MapLoadMessageSenderSystem()
    : MessageSenderSystem()
{
}


void MapLoadMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnMapLoad=EventServer<core::MapLoadEvent>::Get().Subscribe( boost::bind( &MapLoadMessageSenderSystem::OnMapLoad, this, _1 ) );
}


void MapLoadMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void MapLoadMessageSenderSystem::OnMapLoad(core::MapLoadEvent const& Evt)
{
    std::auto_ptr<MapLoadMessage> mapLoadMsg(new MapLoadMessage);
    mapLoadMsg->mMapName=Evt.mMapName;
    mMessageHolder.AddOutgoingMessage(mapLoadMsg);
}

MapLoadMessageHandlerSubSystem::MapLoadMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void MapLoadMessageHandlerSubSystem::Init()
{
}

void MapLoadMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void MapLoadMessageHandlerSubSystem::Execute(Message const& message)
{
    MapLoadMessage const& msg=static_cast<MapLoadMessage const&>(message);
    EventServer<core::MapLoadEvent>::Get().SendEvent( { msg.mMapName } );
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__MapLoadMessage, network::MapLoadMessage );

