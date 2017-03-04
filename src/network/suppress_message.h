#ifndef INCLUDED_NETWORK_SUPPRESS_H
#define INCLUDED_NETWORK_SUPPRESS_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "engine/system_suppressor.h"

namespace network {

class SuppressMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(SuppressMessage)
    engine::SystemSuppressor::Type mState;
    bool mSuppressed;
    SuppressMessage()
        : mState( engine::SystemSuppressor::None )
        , mSuppressed(false)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    bool operator==( SuppressMessage const& other );
};

template<class Archive>
void SuppressMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mState;
    ar& mSuppressed;
}

class SuppressMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(SuppressMessageHandlerSubSystem)
    SuppressMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class SuppressMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnSuppress;
    void OnSuppress(engine::SuppressEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(SuppressMessageSenderSystem)
    SuppressMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_SUPPRESS_H

REAPING2_CLASS_EXPORT_KEY2( network__SuppressMessage, network::SuppressMessage, "suppress" );

//command:  "classgenerator.exe" -g "message" -e "engine-suppress" -m " SystemSuppressor::Type-state bool-suppressed" -c "suppress"
