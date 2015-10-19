#include "platform/i_platform.h"
#include "network/ctf_client_datas_message.h"
#include "core/ctf_program_state.h"

namespace network {
namespace ctf {

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
    L1("executing ctf::ClientDatasMessageHandlerSubSystem from id: %d \n",msg.mSenderId );
    ::ctf::ProgramState& ctfProgramState=::ctf::ProgramState::Get();
    ctfProgramState.mClientDatas=msg.mClientDatas;
    for (::ctf::ProgramState::ClientDatas_t::iterator i=ctfProgramState.mClientDatas.begin(), e=ctfProgramState.mClientDatas.end();i!=e;++i)
    {
        L1("**** ctf arrived. **** from id: %d \n", i->mClientId );
        L1("   team:%d\n", i->mTeam );
    }
}

} // namespace ctf
} // namespace network

