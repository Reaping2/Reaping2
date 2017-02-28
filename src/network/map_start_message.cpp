#include "platform/i_platform.h"
#include "network/map_start_message.h"

namespace network {

bool MapStartMessage::operator==( MapStartMessage const& other )
{
    return mState == other.mState;
}
MapStartMessageSenderSystem::MapStartMessageSenderSystem()
    : MessageSenderSystem()
{
}


void MapStartMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnMapStart=EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &MapStartMessageSenderSystem::OnMapStart, this, _1 ) );
}


void MapStartMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void MapStartMessageSenderSystem::OnMapStart(core::MapStartEvent const& Evt)
{
    std::auto_ptr<MapStartMessage> mapStartMsg(new MapStartMessage);
    mapStartMsg->mState=Evt.mState;
    mMessageHolder.AddOutgoingMessage(mapStartMsg);
}

MapStartMessageHandlerSubSystem::MapStartMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void MapStartMessageHandlerSubSystem::Init()
{
}

void MapStartMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void MapStartMessageHandlerSubSystem::Execute(Message const& message)
{
    MapStartMessage const& msg=static_cast<MapStartMessage const&>(message);
    L2( "Handle Map start %d!\n", msg.mState );
    EventServer<core::MapStartEvent>::Get().SendEvent( { msg.mState } );
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__MapStartMessage, network::MapStartMessage );

