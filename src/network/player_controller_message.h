#ifndef INCLUDED_NETWORK_PLAYER_CONTROLLER_MESSAGE_H
#define INCLUDED_NETWORK_PLAYER_CONTROLLER_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
namespace network {

    class PlayerControllerMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(PlayerControllerMessage)
        int32_t mActorGUID;
        double mOrientation;
        double mHeading;
        bool mShoot;
        bool mShootAlt;
        uint32_t mCurrentMovement;
        PlayerControllerMessage()
            : mActorGUID(0)
            , mOrientation(0.0)
            , mHeading(0.0)
            , mShoot(false)
            , mShootAlt(false)
            , mCurrentMovement(0)
        {
        }
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void PlayerControllerMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mActorGUID;
        ar & mOrientation;
        ar & mHeading;
        ar & mShoot;
        ar & mShootAlt;
        ar & mCurrentMovement;
    }

    class PlayerControllerMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(PlayerControllerMessageHandlerSubSystem)
        PlayerControllerMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

    class PlayerControllerMessageSenderSystem: public MessageSenderSystem
    {
    public:
        DEFINE_SYSTEM_BASE(PlayerControllerMessageSenderSystem)
        PlayerControllerMessageSenderSystem();
        virtual void Init();
        virtual void Update( double DeltaTime );
    };

} // namespace network
#endif//INCLUDED_NETWORK_PLAYER_CONTROLLER_MESSAGE_H
