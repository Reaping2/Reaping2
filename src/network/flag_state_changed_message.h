#ifndef INCLUDED_NETWORK_FLAG_STATE_CHANGED_H
#define INCLUDED_NETWORK_FLAG_STATE_CHANGED_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/flag_state_changed_event.h"
#include "core/ctf_program_state.h"
#include "platform/export.h"

namespace network {

class FlagStateChangedMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(FlagStateChangedMessage)
    ::ctf::FlagStateChangedEvent::Type mType;
    Team::Type mTeam;
    int32_t mCarrierGUID;
    int32_t mFlagGUID;
    FlagStateChangedMessage()
        : mType(::ctf::FlagStateChangedEvent::Dropped)
        , mTeam(Team::None)
        , mCarrierGUID(-1)
        , mFlagGUID(-1)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void FlagStateChangedMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mType;
    ar & mTeam;
    ar & mCarrierGUID;
    ar & mFlagGUID;
}

class FlagStateChangedMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(FlagStateChangedMessageHandlerSubSystem)
    FlagStateChangedMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class FlagStateChangedMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnFlagStateChanged;
    void OnFlagStateChanged(::ctf::FlagStateChangedEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(FlagStateChangedMessageSenderSystem)
    FlagStateChangedMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2(network__FlagStateChangedMessage, network::FlagStateChangedMessage,"flag_state_changed");
#endif//INCLUDED_NETWORK_FLAG_STATE_CHANGED_H

//command:  "classgenerator.exe" -g "message" -c "flag_state_changed" -e "ctf-flagStateChanged" -m "ctf::FlagStateChangedEvent::Type-type Team::Type-team int32_t-carrirGUID int32_t-flagGUID"
