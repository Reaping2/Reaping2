#include "platform/i_platform.h"
#include "network/client_score_message.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

ClientScoreMessageSenderSystem::ClientScoreMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ClientScoreMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnClientScore=EventServer<engine::ClientScoreEvent>::Get().Subscribe( boost::bind( &ClientScoreMessageSenderSystem::OnClientScore, this, _1 ) );
}


void ClientScoreMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void ClientScoreMessageSenderSystem::OnClientScore(engine::ClientScoreEvent const& Evt)
{
    std::auto_ptr<ClientScoreMessage> clientScoreMsg(new ClientScoreMessage);
    clientScoreMsg->mClientID=Evt.mClientID;
    clientScoreMsg->mScore=Evt.mScore;
    mMessageHolder.AddOutgoingMessage(clientScoreMsg);
}

ClientScoreMessageHandlerSubSystem::ClientScoreMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ClientScoreMessageHandlerSubSystem::Init()
{
}

void ClientScoreMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void ClientScoreMessageHandlerSubSystem::Execute(Message const& message)
{
    ClientScoreMessage const& msg=static_cast<ClientScoreMessage const&>(message);
    Opt<core::ClientData> clientData=mProgramState.FindClientDataByClientId(msg.mClientID);    
    if (clientData.IsValid())
    {
        clientData->mScore=msg.mScore;
    }
}

} // namespace network


BOOST_CLASS_EXPORT_IMPLEMENT(network::ClientScoreMessage);
