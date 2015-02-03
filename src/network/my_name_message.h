#ifndef INCLUDED_NETWORK_MY_NAME_MESSAGE_H
#define INCLUDED_NETWORK_MY_NAME_MESSAGE_H

#include "network/message.h"
namespace network {

class MyNameMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(MyNameMessage)
    std::string mName;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void MyNameMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mName;
}

} // namespace network

#endif//INCLUDED_NETWORK_MY_NAME_MESSAGE_H
