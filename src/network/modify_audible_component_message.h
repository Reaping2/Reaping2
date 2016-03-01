#ifndef INCLUDED_NETWORK_MODIFY_AUDIBLE_COMPONENT_H
#define INCLUDED_NETWORK_MODIFY_AUDIBLE_COMPONENT_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/audible_event.h"
#include "platform/export.h"

namespace network {

class ModifyAudibleComponentMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(ModifyAudibleComponentMessage)
    int32_t mActorGUID;
    int32_t mId;
    bool mIsOneShot;
    bool mIsAddition;
    ModifyAudibleComponentMessage()
        : mActorGUID(0)
        , mId(0)
        , mIsOneShot(false)
        , mIsAddition(false)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ModifyAudibleComponentMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mId;
    ar & mIsOneShot;
    ar & mIsAddition;
}

class ModifyAudibleComponentMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<ModifyAudibleComponentMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE(ModifyAudibleComponentMessageHandlerSubSystem)
    ModifyAudibleComponentMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    virtual bool ProcessPending(Message const& message);
};

class ModifyAudibleComponentMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnAudible;
    void OnAudible(core::AudibleEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(ModifyAudibleComponentMessageSenderSystem)
    ModifyAudibleComponentMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2(network__ModifyAudibleComponentMessage, network::ModifyAudibleComponentMessage,"modify_audible_component");
#endif//INCLUDED_NETWORK_MODIFY_AUDIBLE_COMPONENT_H

//command:  "classgenerator" -g "message" -c "modify_audible_component" -m "int32_t-actorGUID int32_t-id bool-isOneShot bool-isAddition" -e "core-audible" -p "pending"
