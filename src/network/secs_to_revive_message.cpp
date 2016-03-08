#include "platform/i_platform.h"
#include "network/secs_to_revive_message.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

SecsToReviveMessageSenderSystem::SecsToReviveMessageSenderSystem()
    : MessageSenderSystem()
{
}


void SecsToReviveMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnSecsToRevive = EventServer<engine::SecsToReviveEvent>::Get().Subscribe( boost::bind( &SecsToReviveMessageSenderSystem::OnSecsToRevive, this, _1 ) );
}


void SecsToReviveMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void SecsToReviveMessageSenderSystem::OnSecsToRevive( engine::SecsToReviveEvent const& Evt )
{
    std::auto_ptr<SecsToReviveMessage> secsToReviveMsg( new SecsToReviveMessage );
    secsToReviveMsg->mClientId = Evt.mClientId;
    secsToReviveMsg->mSecsToRevive = Evt.mSecsToRevive;
    mMessageHolder.AddOutgoingMessage( secsToReviveMsg );
}

SecsToReviveMessageHandlerSubSystem::SecsToReviveMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void SecsToReviveMessageHandlerSubSystem::Init()
{
}

void SecsToReviveMessageHandlerSubSystem::Update( double DeltaTime )
{
    MessageHandlerSubSystem::Update( DeltaTime );
}

void SecsToReviveMessageHandlerSubSystem::Execute( Message const& message )
{
    SecsToReviveMessage const& msg = static_cast<SecsToReviveMessage const&>( message );
    EventServer<engine::SecsToReviveEvent>::Get().SendEvent( engine::SecsToReviveEvent( msg.mClientId, msg.mSecsToRevive ) );
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__SecsToReviveMessage, network::SecsToReviveMessage );
