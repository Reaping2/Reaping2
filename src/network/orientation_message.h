#ifndef INCLUDED_NETWORK_ORIENTATION_MESSAGE_H
#define INCLUDED_NETWORK_ORIENTATION_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "single_message_sender.h"
#include "platform/export.h"
namespace network {

class OrientationMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( OrientationMessage )
    int32_t mActorGUID;
    int32_t mOrientation;
    OrientationMessage()
        : mActorGUID( 0 )
        , mOrientation( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
    bool operator==( OrientationMessage const& other )
    {
        return mActorGUID == other.mActorGUID
               && mOrientation == other.mOrientation;
    }
};

template<class Archive>
void OrientationMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorGUID;
    ar& mOrientation;
}

class OrientationMessageHandlerSubSystem: public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( OrientationMessageHandlerSubSystem )
    OrientationMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
};

class OrientationMessageSenderSystem: public ActorTimerMessageSenderSystem<OrientationMessage>
{
    typedef std::set<int32_t> SendOrientations_t;
    SendOrientations_t mSendOrientations;
    ActorFrequencyTimerHolder mActorFrequencyTimerHolder;
    virtual void AddUniqueMessage( Actor& actor );
    virtual void AddMandatoryMessage( Actor& actor );
public:
    DEFINE_SYSTEM_BASE( OrientationMessageSenderSystem )
    OrientationMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    static std::auto_ptr<OrientationMessage> GenerateOrientationMessage( Actor& actor );

};

} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__OrientationMessage, network::OrientationMessage, "orientation" );
#endif//INCLUDED_NETWORK_ORIENTATION_MESSAGE_H
