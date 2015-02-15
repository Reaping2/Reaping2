#ifndef INCLUDED_NETWORK_SINGLE_MESSAGE_SENDER_H
#define INCLUDED_NETWORK_SINGLE_MESSAGE_SENDER_H

#include "platform/i_platform.h"
#include <map>
#include "message.h"
#include "messsage_holder.h"
#include "platform/event.h"
#include "core/actor_event.h"
#include "platform/register.h"

namespace network {

template<class MESSAGE_ID, class MESSAGE>
class SingleMessageSender
{
    BOOST_STATIC_ASSERT_MSG(
        ( boost::is_base_of<Message, MESSAGE>::value ),
        "MESSAGE must be a descendant of Message!"
        );
protected:
    typedef std::map<MESSAGE_ID,MESSAGE> MessageMap_t;
    MessageMap_t mMessageMap;
    MessageHolder& mMessageHolder;
public:
    SingleMessageSender();
    void Add(MESSAGE_ID messageId, std::auto_ptr<MESSAGE> message);
    void Remove(MESSAGE_ID messageId);
    virtual ~SingleMessageSender();
};

template<class MESSAGE_ID, class MESSAGE>
network::SingleMessageSender<MESSAGE_ID, MESSAGE>::~SingleMessageSender()
{

}

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

template<class MESSAGE>
class AutoActorGUIDSingleMessageSender : public SingleMessageSender<int32_t, MESSAGE>
{
    platform::AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
public:
    AutoActorGUIDSingleMessageSender();
};

template<class MESSAGE>
void AutoActorGUIDSingleMessageSender<MESSAGE>::OnActorEvent(ActorEvent const& Evt)
{
    if(Evt.mState==ActorEvent::Removed)
    {
        AutoActorGUIDSingleMessageSender<MESSAGE>::Remove(Evt.mActor->GetGUID());
    }
}

template<class MESSAGE>
AutoActorGUIDSingleMessageSender<MESSAGE>::AutoActorGUIDSingleMessageSender()
{
    mOnActorEvent = platform::EventServer<ActorEvent>::Get().Subscribe( boost::bind( &AutoActorGUIDSingleMessageSender::OnActorEvent, this, _1 ) );
}

} // namespace network

#endif//INCLUDED_NETWORK_SINGLE_MESSAGE_SENDER_H
