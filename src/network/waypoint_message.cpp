#include "platform/i_platform.h"
#include "network/waypoint_message.h"

namespace network {

WaypointMessageSenderSystem::WaypointMessageSenderSystem()
    : MessageSenderSystem()
{
}


void WaypointMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnWaypointChanged=EventServer<engine::WaypointChangedEvent>::Get().Subscribe( boost::bind( &WaypointMessageSenderSystem::OnWaypointChanged, this, _1 ) );
}


void WaypointMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void WaypointMessageSenderSystem::OnWaypointChanged(engine::WaypointChangedEvent const& Evt)
{
    if (mProgramState.mMode == core::ProgramState::Client
        &&Evt.mState == engine::WaypointChangedEvent::Selected)
    {
        CreateMessage( Evt );
    }
    if (mProgramState.mMode == core::ProgramState::Server
        && (Evt.mState == engine::WaypointChangedEvent::Lit
            || Evt.mState == engine::WaypointChangedEvent::Choose))
    {
        CreateMessage( Evt );
    }
}

void WaypointMessageSenderSystem::CreateMessage( engine::WaypointChangedEvent const& Evt )
{
    std::auto_ptr<WaypointMessage> waypointMsg( new WaypointMessage );
    waypointMsg->mEvent = Evt;
    mMessageHolder.AddOutgoingMessage( waypointMsg );
}

WaypointMessageHandlerSubSystem::WaypointMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void WaypointMessageHandlerSubSystem::Init()
{
}

void WaypointMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void WaypointMessageHandlerSubSystem::Execute(Message const& message)
{
    WaypointMessage const& msg=static_cast<WaypointMessage const&>(message);
    if (mProgramState.mMode == core::ProgramState::Client)
    {
        if (msg.mEvent.mState == engine::WaypointChangedEvent::Lit)
        {
            EventServer<engine::WaypointChangedEvent>::Get().SendEvent( msg.mEvent );
        }
        else if (msg.mEvent.mState == engine::WaypointChangedEvent::Choose
            && msg.mEvent.mPlayerGUID == mProgramState.mControlledActorGUID)
        {
            EventServer<engine::WaypointChangedEvent>::Get().SendEvent( msg.mEvent );
        }
    }
    if (mProgramState.mMode == core::ProgramState::Server
        &&msg.mEvent.mState == engine::WaypointChangedEvent::Selected)
    {
        EventServer<engine::WaypointChangedEvent>::Get().SendEvent( msg.mEvent );
    }
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__WaypointMessage, network::WaypointMessage );

