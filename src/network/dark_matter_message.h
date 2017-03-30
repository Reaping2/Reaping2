#ifndef INCLUDED_NETWORK_DARK_MATTER_H
#define INCLUDED_NETWORK_DARK_MATTER_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"

namespace network {

class DarkMatterMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(DarkMatterMessage)
    int32_t mActorGUID;
    int32_t mDarkMatters;
    DarkMatterMessage()
        : mActorGUID(-1)
        , mDarkMatters(0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    bool operator==( DarkMatterMessage const& other );
};

template<class Archive>
void DarkMatterMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mActorGUID;
    ar& mDarkMatters;
}

class DarkMatterMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(DarkMatterMessageHandlerSubSystem)
    DarkMatterMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class DarkMatterMessageSenderSystem : public ActorTimerMessageSenderSystem<DarkMatterMessage>
{
    virtual void AddUniqueMessage( Actor& actor );
    virtual void AddMandatoryMessage( Actor& actor );
public:
    DEFINE_SYSTEM_BASE(DarkMatterMessageSenderSystem)
    DarkMatterMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    static std::auto_ptr<DarkMatterMessage> GenerateDarkMatterMessage(Actor &actor);
};
} // namespace network

#endif//INCLUDED_NETWORK_DARK_MATTER_H

REAPING2_CLASS_EXPORT_KEY2( network__DarkMatterMessage, network::DarkMatterMessage, "dark_matter" );

//command:  "classgenerator.exe" -g "message" -c "dark_matter" -t "inventory" -m "int32_t-actorGUID int32_t-darkMatters"
