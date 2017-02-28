#include "platform/i_platform.h"
#include "network/suppress_message.h"

namespace network {

bool SuppressMessage::operator==( SuppressMessage const& other )
{
    return mState == other.mState
        && mSuppressed == other.mSuppressed;
}
SuppressMessageSenderSystem::SuppressMessageSenderSystem()
    : MessageSenderSystem()
{
}


void SuppressMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnSuppress=EventServer<engine::SuppressEvent>::Get().Subscribe( boost::bind( &SuppressMessageSenderSystem::OnSuppress, this, _1 ) );
}


void SuppressMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void SuppressMessageSenderSystem::OnSuppress(engine::SuppressEvent const& Evt)
{
    std::auto_ptr<SuppressMessage> suppressMsg(new SuppressMessage);
    suppressMsg->mState=Evt.mState;
    suppressMsg->mSuppressed=Evt.mSuppressed;
    mMessageHolder.AddOutgoingMessage(suppressMsg);
}

SuppressMessageHandlerSubSystem::SuppressMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void SuppressMessageHandlerSubSystem::Init()
{
}

void SuppressMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void SuppressMessageHandlerSubSystem::Execute(Message const& message)
{
    SuppressMessage const& msg=static_cast<SuppressMessage const&>(message);
    if (msg.mSuppressed)
    {
        bool succ = engine::SystemSuppressor::Get().Suppress( msg.mState );
    }
    else
    {
        bool succ = engine::SystemSuppressor::Get().Resume( msg.mState );
    }
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__SuppressMessage, network::SuppressMessage );

