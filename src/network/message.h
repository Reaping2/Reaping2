#ifndef INCLUDED_NETWORK_MESSAGE_H
#define INCLUDED_NETWORK_MESSAGE_H

#include "platform/i_platform.h"
#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/export.hpp>

#define DEFINE_MESSAGE_BASE( MessageType ) \
    static int32_t GetType_static() \
    { \
    static int32_t const typ = platform::AutoId( #MessageType ); \
    return typ; \
    } \
    virtual int32_t GetType() const \
    { \
        return MessageType::GetType_static(); \
    } \


namespace network {

class Message
{
    friend class ::boost::serialization::access;
public:
    virtual int32_t GetType() const=0;
    int32_t mSenderId;
    Message();
    virtual ~Message();
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void Message::serialize(Archive& ar, const unsigned int version)
{
}

class DefaultMessage : public Message
{
public:
    DEFINE_MESSAGE_BASE(DefaultMessage)
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void DefaultMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
}

} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_H
