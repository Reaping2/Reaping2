#ifndef INCLUDED_NETWORK_MESSAGE_HOLDER_H
#define INCLUDED_NETWORK_MESSAGE_HOLDER_H

#include "message.h"
#include "boost/ptr_container/ptr_list.hpp"
#include <boost/ptr_container/serialize_ptr_list.hpp>
#include "boost/static_assert.hpp"

namespace network {

class MessageList
{
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mMessages;
    }
public:
    typedef ::boost::ptr_list<Message> Messages_t;
    Messages_t mMessages;
};

class MessageHolder : public platform::Singleton<MessageHolder>
{
public:
    friend class platform::Singleton<MessageHolder>;
    MessageList mOutgoingMessages;
    MessageList mIncomingMessages;
    MessageHolder();
    MessageList& GetOutgoingMessages();
    MessageList& GetIncomingMessages();
    template<typename MESSAGE>
    void AddOutgoingMessage(std::auto_ptr<MESSAGE> message);

    void AddIncomingMessage(std::auto_ptr<Message> message);
    void ClearOutgoingMessages();
    void ClearIncomingMessages();

};

template<typename MESSAGE>
void MessageHolder::AddOutgoingMessage(std::auto_ptr<MESSAGE> message)
{
    {
        if (message.get()==NULL)
        {
            return;
        }
        BOOST_STATIC_ASSERT_MSG(
            ( boost::is_base_of<Message, MESSAGE>::value ),
            "MESSAGE must be a descendant of Message!"
            );
        mOutgoingMessages.mMessages.push_back(std::auto_ptr<Message>(message.release()));
    }
}

} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_HOLDER_H
