#include "platform/i_platform.h"
#include "network/client_id_message_handler_sub_system.h"
#include "client_id_message.h"
#include "core/program_state.h"
namespace network {

ClientIdMessageHandlerSubSystem::ClientIdMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void ClientIdMessageHandlerSubSystem::Init()
{

}

void ClientIdMessageHandlerSubSystem::Execute( Message const& message )
{
    ClientIdMessage const& msg = static_cast<ClientIdMessage const&>( message );
    L2( "executing clientid: name %s name's id: %d \n", msg.mName.c_str(), msg.mClientId );
    if ( msg.mName == mProgramState.mClientName )
    {
        L1( "that is my id\n" );
        mProgramState.mClientId = msg.mClientId;
    }
}

} // namespace engine

