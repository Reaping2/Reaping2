#ifndef INCLUDED_NETWORK_MESSAGE_HOLDER_H
#define INCLUDED_NETWORK_MESSAGE_HOLDER_H

#include "message.h"
#include "boost/ptr_container/ptr_list.hpp"
#include <boost/ptr_container/serialize_ptr_list.hpp>
#include "boost/static_assert.hpp"
#include <mutex>              
#include <condition_variable> 

namespace network {

class MessageList
{
public:
    typedef ::boost::ptr_list<Message> Messages_t;
    Messages_t mMessages;
    Messages_t mPublishedMessages;

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version )
    {
        ar& mPublishedMessages;
    }
    // no lock needed only the producer thread should access
    void Add( std::auto_ptr<Message> message );
    // no lock needed only the producer thread should access
    void TransferFrom( Messages_t& messages );

    // needs lock Publish and TransferPublishedMessagesTo can race
    void Publish();
    // needs lock Publish and TransferPublishedMessagesTo can race
    void TransferPublishedMessagesTo( Messages_t& messages);
    // needs lock. It's just a size ofc.
    bool HasPublishedMessages() const;
    std::mutex& GetMutex();
    std::condition_variable& GetCV();
private:
    std::mutex mMutex;
    std::condition_variable mCV;
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
    void AddOutgoingMessage( std::auto_ptr<MESSAGE> message );
private:

};

template<typename MESSAGE>
void MessageHolder::AddOutgoingMessage( std::auto_ptr<MESSAGE> message )
{
    {
        if ( message.get() == NULL )
        {
            return;
        }
        BOOST_STATIC_ASSERT_MSG(
            ( boost::is_base_of<Message, MESSAGE>::value ),
            "MESSAGE must be a descendant of Message!"
        );
        mOutgoingMessages.Add( std::auto_ptr<Message>(message.release()) );
    }
}



} // namespace network

#endif//INCLUDED_NETWORK_MESSAGE_HOLDER_H
