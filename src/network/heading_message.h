#ifndef INCLUDED_NETWORK_HEADING_MESSAGE_H
#define INCLUDED_NETWORK_HEADING_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "single_message_sender.h"
#include "platform/export.h"
#include "actor_frequency_timer.h"
namespace network {

class HeadingMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( HeadingMessage )
    int32_t mActorGUID;
    int32_t mHeading;
    HeadingMessage()
        : mActorGUID( 0 )
        , mHeading( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
    bool operator==( HeadingMessage const& other )
    {
        return mActorGUID == other.mActorGUID
               && mHeading == other.mHeading;
    }
};

template<class Archive>
void HeadingMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorGUID;
    ar& mHeading;
}

class HeadingMessageHandlerSubSystem: public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( HeadingMessageHandlerSubSystem )
    HeadingMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
};

class HeadingMessageSenderSystem: public ActorTimerMessageSenderSystem<HeadingMessage>
{
    typedef std::set<int32_t> SendHeadings_t;
    SendHeadings_t mSendHeadings;
    ActorFrequencyTimerHolder mActorFrequencyTimerHolder;
    virtual void AddUniqueMessage( Actor& actor );
    virtual void AddMandatoryMessage( Actor& actor );
public:
    DEFINE_SYSTEM_BASE( HeadingMessageSenderSystem )
    HeadingMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    static std::auto_ptr<HeadingMessage> GenerateHeadingMessage( Actor& actor );

};

} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__HeadingMessage, network::HeadingMessage, "heading" );
#endif//INCLUDED_NETWORK_HEADING_MESSAGE_H
