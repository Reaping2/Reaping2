#ifndef INCLUDED_NETWORK_CLIENT_ID_MESSAGE_H
#define INCLUDED_NETWORK_CLIENT_ID_MESSAGE_H

#include "network/message.h"
namespace network {

    class ClientIdMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(ClientIdMessage)
        std::string mName;
        int32_t mClientId;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void ClientIdMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mName;
        ar & mClientId;
    }

} // namespace network
#endif//INCLUDED_NETWORK_CLIENT_ID_MESSAGE_H
