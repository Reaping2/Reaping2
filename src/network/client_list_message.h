#ifndef INCLUDED_NETWORK_CLIENT_LIST_MESSAGE_H
#define INCLUDED_NETWORK_CLIENT_LIST_MESSAGE_H

#include "network/message.h"
#include <vector>

namespace network {

class ClientListMessage : public Message
{

friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(ClientListMessage)
    std::vector<std::string> mClientList;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ClientListMessage::serialize( Archive & ar, unsigned int version )
{
	ar & boost::serialization::base_object<Message>(*this);
	ar & mClientList;
}

}


#endif
