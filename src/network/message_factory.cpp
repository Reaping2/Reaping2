#include "platform/i_platform.h"
#include "network/message_factory.h"
#include "platform/auto_id.h"
#include "my_name_message.h"
#include "client_id_message.h"


using platform::AutoId;
namespace network {

MessageFactory::MessageFactory()
{
    Bind( AutoId( "default_message" ), &CreateMessage<DefaultMessage> );
    SetDefault( AutoId( "default_message" ) );

    Bind( AutoId( "my_name_message" ), &CreateMessage<MyNameMessage> );
    Bind( AutoId( "client_id_message" ), &CreateMessage<ClientIdMessage> );
}

} // namespace engine
