#ifndef INCLUDED_NETWORK_SHOT_H
#define INCLUDED_NETWORK_SHOT_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/shot_event.h"

namespace network {

class ShotMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(ShotMessage)
    int32_t mActorGUID;
    int32_t mX;
    int32_t mY;
    bool mIsAlt;
    ShotMessage()
        : mActorGUID(0)
        , mX(0)
        , mY(0)
        , mIsAlt(0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ShotMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mX;
    ar & mY;
    ar & mIsAlt;
}

class ShotMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(ShotMessageHandlerSubSystem)
    ShotMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class ShotMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnShot;
    void OnShot(core::ShotEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(ShotMessageSenderSystem)
    ShotMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_SHOT_H

//command:  "classgenerator.exe" -g "message" -c "shot" -m "int32_t-actorGUID int32_t-x int32_t-y bool-isAlt" -e "core-shot"
