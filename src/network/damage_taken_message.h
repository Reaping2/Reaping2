#ifndef INCLUDED_NETWORK_DAMAGE_TAKEN_MESSAGE_H
#define INCLUDED_NETWORK_DAMAGE_TAKEN_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/damage_taken_event.h"
#include "platform/export.h"
namespace network {

    class DamageTakenMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(DamageTakenMessage)
        int32_t mActorGUID;
        int32_t mX;
        int32_t mY;
        int32_t mDamage;
        core::DamageTakenEvent::Type mType;
        DamageTakenMessage()
            : mActorGUID(0)
            , mX(0)
            , mY(0)
            , mDamage(0)
            , mType(core::DamageTakenEvent::Health)
        {
        }
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void DamageTakenMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorGUID;
        ar & mX;
        ar & mY;
        ar & mDamage;
        ar & mType;
    }

    class DamageTakenMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(DamageTakenMessageHandlerSubSystem)
        DamageTakenMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

    class DamageTakenMessageSenderSystem: public MessageSenderSystem
    {
        AutoReg mOnDamageTaken;
        void OnDamageTaken( core::DamageTakenEvent const& Evt );
    public:
        DEFINE_SYSTEM_BASE(DamageTakenMessageSenderSystem)
        DamageTakenMessageSenderSystem();
        virtual void Init();
        virtual void Update( double DeltaTime );
    };

} // namespace network

REAPING2_CLASS_EXPORT_KEY2(network__DamageTakenMessage, network::DamageTakenMessage,"damage_taken");
#endif//INCLUDED_NETWORK_DAMAGE_TAKEN_MESSAGE_H
