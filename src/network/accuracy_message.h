#ifndef INCLUDED_NETWORK_ACCURACY_H
#define INCLUDED_NETWORK_ACCURACY_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"

namespace network {

class AccuracyMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( AccuracyMessage )
    int32_t mActorGUID;
    int32_t mAccuracy;
    int32_t mAccuracyPercent;
    int32_t mAccuracyFlat;
    AccuracyMessage()
        : mActorGUID( 0 )
        , mAccuracy( 0 )
        , mAccuracyPercent( 0 )
        , mAccuracyFlat( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void AccuracyMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorGUID;
    ar& mAccuracy;
    ar& mAccuracyPercent;
    ar& mAccuracyFlat;
}

class AccuracyMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<AccuracyMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE( AccuracyMessageHandlerSubSystem )
    AccuracyMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    virtual bool ProcessPending( Message const& message );
};

class AccuracyMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE( AccuracyMessageSenderSystem )
    AccuracyMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    static std::auto_ptr<AccuracyMessage> GenerateAccuracyMessage( Actor& actor );
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__AccuracyMessage, network::AccuracyMessage, "accuracy" );
#endif//INCLUDED_NETWORK_ACCURACY_H

//TODO: to main.cpp:
//Eng.AddSystem(AutoId("accuracy_message_sender_system"));

