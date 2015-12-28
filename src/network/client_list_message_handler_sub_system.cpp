#include "client_list_message_handler_sub_system.h"
#include "client_list_message.h"
#include "client_list_changed_event.h"

namespace network {

ClientListMessageHandlerSubSystem::ClientListMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}

void ClientListMessageHandlerSubSystem::Init()
{
}

void ClientListMessageHandlerSubSystem::Execute( Message const & message )
{
    ClientListMessage const& msg = static_cast<ClientListMessage const&>(message);
    EventServer<ClientListChangedEvent>::Get().SendEvent( ClientListChangedEvent( msg.mClientList ) );
}

}
