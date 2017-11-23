#include "platform/i_platform.h"
#include "network/chest_message.h"

namespace network {

ChestMessageSenderSystem::ChestMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ChestMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnChestChanged=EventServer<engine::ChestChangedEvent>::Get().Subscribe( boost::bind( &ChestMessageSenderSystem::OnChestChanged, this, _1 ) );
}


void ChestMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void ChestMessageSenderSystem::OnChestChanged(engine::ChestChangedEvent const& Evt)
{
    if (mProgramState.mMode == core::ProgramState::Server)
    {
        std::auto_ptr<ChestMessage> chestMsg( new ChestMessage );
        chestMsg->mEvent = Evt;
        mMessageHolder.AddOutgoingMessage( chestMsg );
    }
}

ChestMessageHandlerSubSystem::ChestMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ChestMessageHandlerSubSystem::Init()
{
}

void ChestMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void ChestMessageHandlerSubSystem::Execute(Message const& message)
{
    ChestMessage const& msg=static_cast<ChestMessage const&>(message);
    if (mProgramState.mMode == core::ProgramState::Client)
    {
        EventServer<engine::ChestChangedEvent>::Get().SendEvent( msg.mEvent );
    }
    
}

} // namespace network

REAPING2_CLASS_EXPORT_IMPLEMENT( network__ChestMessage, network::ChestMessage );

