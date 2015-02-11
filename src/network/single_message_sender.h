#ifndef INCLUDED_NETWORK_SINGLE_MESSAGE_SENDER_H
#define INCLUDED_NETWORK_SINGLE_MESSAGE_SENDER_H

#include "platform/i_platform.h"
#include <map>
#include "message.h"
#include "messsage_holder.h"

namespace network {

template<class MESSAGE_ID, class MESSAGE>
class SingleMessageSender
{
    BOOST_STATIC_ASSERT_MSG(
        ( boost::is_base_of<Message, MESSAGE>::value ),
        "MESSAGE must be a descendant of Message!"
        );
    typedef std::map<MESSAGE_ID,MESSAGE> MessageMap_t;
    MessageMap_t mMessageMap;
    MessageHolder& mMessageHolder;
public:
    SingleMessageSender();
    void Add(MESSAGE_ID messageId, std::auto_ptr<MESSAGE> message);
    void Remove(MESSAGE_ID messageId);
};

template<class MESSAGE_ID, class MESSAGE>
network::SingleMessageSender<MESSAGE_ID, MESSAGE>::SingleMessageSender()
    : mMessageHolder(MessageHolder::Get())
{

}

template<class MESSAGE_ID, class MESSAGE>
void network::SingleMessageSender<MESSAGE_ID, MESSAGE>::Remove(MESSAGE_ID messageId)
{
    mMessageMap.erase(messageId);
}

template<class MESSAGE_ID, class MESSAGE>
void network::SingleMessageSender<MESSAGE_ID, MESSAGE>::Add(MESSAGE_ID messageId, std::auto_ptr<MESSAGE> message)
{
    typename MessageMap_t::iterator it = mMessageMap.find(messageId);
    if (it!=mMessageMap.end())
    {
        if (it->second==*message.get())
        {
            //L1("wont send message its the same as sent before!\n");
        }
        else
        {
            it->second=*message.get();
            mMessageHolder.AddOutgoingMessage(message);
        }
    }
    else
    {
        mMessageMap[messageId]=*message.get();
        mMessageHolder.AddOutgoingMessage(message);
    }
}
} // namespace network

#endif//INCLUDED_NETWORK_SINGLE_MESSAGE_SENDER_H
