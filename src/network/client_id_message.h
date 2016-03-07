#ifndef INCLUDED_NETWORK_CLIENT_ID_MESSAGE_H
#define INCLUDED_NETWORK_CLIENT_ID_MESSAGE_H

#include "network/message.h"
#include "platform/export.h"
namespace network {

class ClientIdMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( ClientIdMessage )
    std::string mName;
    int32_t mClientId;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ClientIdMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mName;
    ar& mClientId;
}

} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__ClientIdMessage, network::ClientIdMessage, "client_id" );
#endif//INCLUDED_NETWORK_CLIENT_ID_MESSAGE_H
