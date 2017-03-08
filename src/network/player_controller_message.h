#ifndef INCLUDED_NETWORK_PLAYER_CONTROLLER_MESSAGE_H
#define INCLUDED_NETWORK_PLAYER_CONTROLLER_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/trigger.h"
#include "platform/export.h"

namespace network {

class PlayerControllerMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( PlayerControllerMessage )
    int32_t mActorGUID;
    int32_t mOrientation;
    int32_t mHeading;
    bool mShoot;
    bool mShootAlt;
    Trigger mUseNormalItem;
    Trigger mUseReload;
    bool mMoving;
    Trigger mActivate;
    Trigger mSwitchWeapon;
    Trigger mSwitchNormalItem;
    PlayerControllerMessage()
        : mActorGUID( 0 )
        , mOrientation( 0 )
        , mHeading( 0 )
        , mShoot( false )
        , mShootAlt( false )
        , mUseNormalItem()
        , mUseReload()
        , mMoving( false )
        , mActivate()
        , mSwitchWeapon()
        , mSwitchNormalItem()
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
    bool operator==( PlayerControllerMessage const& other );
};

template<class Archive>
void PlayerControllerMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorGUID;
    ar& mOrientation;
    ar& mHeading;
    ar& mShoot;
    ar& mShootAlt;
    ar& mUseNormalItem;
    ar& mUseReload;
    ar& mMoving;
    ar& mActivate;
    ar& mSwitchWeapon;
    ar& mSwitchNormalItem;
}

class PlayerControllerMessageHandlerSubSystem: public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( PlayerControllerMessageHandlerSubSystem )
    PlayerControllerMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
};

class PlayerControllerMessageSenderSystem: public ActorTimerMessageSenderSystem<PlayerControllerMessage>
{
    virtual void AddUniqueMessage( Actor& actor );
    virtual void AddMandatoryMessage( Actor& actor );
public:
    DEFINE_SYSTEM_BASE( PlayerControllerMessageSenderSystem )
    PlayerControllerMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    static std::auto_ptr<PlayerControllerMessage> GenerateMessage( Actor& actor );
};

} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__PlayerControllerMessage, network::PlayerControllerMessage, "player_c" );
#endif//INCLUDED_NETWORK_PLAYER_CONTROLLER_MESSAGE_H
