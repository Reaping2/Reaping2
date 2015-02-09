#ifndef INCLUDED_NETWORK_DAMAGE_TAKEN_MESSAGE_H
#define INCLUDED_NETWORK_DAMAGE_TAKEN_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/damage_taken_event.h"
namespace network {

    class DamageTakenMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(DamageTakenMessage)
        int32_t mActorGUID;
        double mX;
        double mY;
        int32_t mDamage;
        DamageTakenMessage()
            : mActorGUID(0)
            , mX(0.0)
            , mY(0.0)
            , mDamage(0)
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
#endif//INCLUDED_NETWORK_DAMAGE_TAKEN_MESSAGE_H
