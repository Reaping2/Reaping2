#ifndef INCLUDED_NETWORK_ROTATE_H
#define INCLUDED_NETWORK_ROTATE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "single_message_sender.h"
#include "actor_frequency_timer.h"

namespace network {

class RotateMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(RotateMessage)
    int32_t mActorGUID;
    double mSpeed;
    bool mRotating;
    RotateMessage()
        : mActorGUID( -1 )
        , mSpeed( 0.0 )
        , mRotating( false )
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    bool operator==( RotateMessage const& other );
};

template<class Archive>
void RotateMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mActorGUID;
    ar& mSpeed;
    ar& mRotating;
}

class RotateMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(RotateMessageHandlerSubSystem)
    RotateMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class RotateMessageSenderSystem : public ActorTimerMessageSenderSystem<RotateMessage>
{
    ActorFrequencyTimerHolder mActorFrequencyTimerHolder;
    virtual void AddUniqueMessage( Actor& actor );
    virtual void AddMandatoryMessage( Actor& actor );
public:
    DEFINE_SYSTEM_BASE(RotateMessageSenderSystem)
    RotateMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    static std::auto_ptr<RotateMessage> GenerateRotateMessage(Actor &actor);
};
} // namespace network

#endif//INCLUDED_NETWORK_ROTATE_H

REAPING2_CLASS_EXPORT_KEY2( network__RotateMessage, network::RotateMessage, "rotate" );

//command:  "classgenerator.exe" -g "message" -c "rotate" -t "rotate" -m "int32_t-actorGUID double-speed bool-rotating"
