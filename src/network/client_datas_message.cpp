#include "platform/i_platform.h"
#include "network/client_datas_message.h"

namespace network {

ClientDatasMessageSenderSystem::ClientDatasMessageSenderSystem()
    : MessageSenderSystem()
{
        //This one is not used yet you should wire it before use
}


void ClientDatasMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
        //This one is not used yet you should wire it before use
}


void ClientDatasMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
        //This one is not used yet you should wire it before use
}

ClientDatasMessageHandlerSubSystem::ClientDatasMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void ClientDatasMessageHandlerSubSystem::Init()
{
}

void ClientDatasMessageHandlerSubSystem::Execute(Message const& message)
{
    ClientDatasMessage const& msg=static_cast<ClientDatasMessage const&>(message);
    L1("executing ClientDatasMessageHandlerSubSystem from id: %d \n",msg.mSenderId );
    mProgramState.mClientDatas=msg.mClientDatas;
    for (core::ProgramState::ClientDatas_t::iterator i=mProgramState.mClientDatas.begin(), e=mProgramState.mClientDatas.end();i!=e;++i)
    {
        L1("**** %s properties arrived. **** from id: %d \n", i->mClientName.c_str(),msg.mSenderId );
        L1("   MoveSpeed:%d\n   Health:%d\n   Accuracy:%d\n", i->mSoldierProperties.mMoveSpeed, i->mSoldierProperties.mHealth, i->mSoldierProperties.mAccuracy );
    }
}


} // namespace network

