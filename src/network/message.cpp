#include "network/message.h"

namespace network {



Message::Message()
    : mSenderId( -1 )
{

}

Message::~Message()
{

}

} // namespace network
BOOST_CLASS_EXPORT_GUID( network::DefaultMessage, "default" )
