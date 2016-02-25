#ifndef INCLUDED_NETWORK_CLOAK_CHANGED_H
#define INCLUDED_NETWORK_CLOAK_CHANGED_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/cloak_changed_event.h"
#include <boost/serialization/export.hpp>

namespace network {

class CloakChangedMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(CloakChangedMessage)
    int32_t mActorGUID;
    bool mActivated;
    CloakChangedMessage()
        : mActorGUID(-1)
        , mActivated(false)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void CloakChangedMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mActivated;
}

class CloakChangedMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(CloakChangedMessageHandlerSubSystem)
    CloakChangedMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class CloakChangedMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnCloakChanged;
    void OnCloakChanged(engine::CloakChangedEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(CloakChangedMessageSenderSystem)
    CloakChangedMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::CloakChangedMessage,"cloak_changed");
#endif//INCLUDED_NETWORK_CLOAK_CHANGED_H

//command:  "classgenerator.exe" -g "message" -c "cloak_changed" -m "int32_t-actorGUID bool-activated" -e "engine-cloakChanged"
