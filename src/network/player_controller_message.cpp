#include "network/player_controller_message.h"
#include "core/player_controller_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "platform/settings.h"

namespace network {


void PlayerControllerMessageSenderSystem::AddUniqueMessage( Actor& actor )
{
    mUniqueMessageSender.Add( actor.GetGUID(), GenerateMessage( actor ) );
}


void PlayerControllerMessageSenderSystem::AddMandatoryMessage( Actor& actor )
{
    mMessageHolder.AddOutgoingMessage( GenerateMessage( actor ) );
}

PlayerControllerMessageSenderSystem::PlayerControllerMessageSenderSystem()
    : ActorTimerMessageSenderSystem( AutoId( "player_controller" ) )
{

}

void PlayerControllerMessageSenderSystem::Init()
{
    ActorTimerMessageSenderSystem::Init();
    SetFrequency( Settings::Get().GetDouble( "network.frequency.player_controller", 0.01 ) );
}

void PlayerControllerMessageSenderSystem::Update( double DeltaTime )
{
    ActorTimerMessageSenderSystem::Update( DeltaTime );
    if (!IsTime())
    {
        return;
    }
}


std::auto_ptr<PlayerControllerMessage> PlayerControllerMessageSenderSystem::GenerateMessage( Actor& actor )
{
    Opt<PlayerControllerComponent> playerControllerC = actor.Get<IControllerComponent>();
    if (!playerControllerC.IsValid())
    {
        return std::auto_ptr<PlayerControllerMessage>();
    }
    std::auto_ptr<PlayerControllerMessage> playerControllerMsg( new PlayerControllerMessage );
    playerControllerMsg->mActorGUID = actor.GetGUID();
    playerControllerMsg->mOrientation = std::floor( playerControllerC->mOrientation * PRECISION );
    playerControllerMsg->mHeading = std::floor( playerControllerC->mHeading * PRECISION );
    playerControllerMsg->mShoot = playerControllerC->mShoot;
    playerControllerMsg->mShootAlt = playerControllerC->mShootAlt;
    playerControllerMsg->mUseNormalItem = playerControllerC->mUseNormalItem;
    playerControllerMsg->mUseReload = playerControllerC->mUseReload;
    playerControllerMsg->mMoving = playerControllerC->mMoving;
    playerControllerMsg->mActivate = playerControllerC->mActivate;
    playerControllerMsg->mSwitchWeapon = playerControllerC->mSwitchWeapon;
    playerControllerMsg->mSwitchNormalItem = playerControllerC->mSwitchNormalItem;
    return playerControllerMsg;
    
}

PlayerControllerMessageHandlerSubSystem::PlayerControllerMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void PlayerControllerMessageHandlerSubSystem::Init()
{

}

void PlayerControllerMessageHandlerSubSystem::Execute( Message const& message )
{
    PlayerControllerMessage const& msg = static_cast<PlayerControllerMessage const&>( message );
    //        L1("executing PlayerController: %d \n",msg.mSenderId );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
    if ( !actor.IsValid() )
    {
        L1( "cannot find actor with GUID: (that is not possible) %d \n", msg.mActorGUID );
        return;
    }

    Opt<PlayerControllerComponent> playerControllerC = actor->Get<IControllerComponent>();
    if ( !playerControllerC.IsValid() )
    {
        L1( "playercontroller is called on an actor that has no player_controller_component \n" );
        return;
    }
    playerControllerC->mOrientation = msg.mOrientation / PRECISION;
    playerControllerC->mHeading = msg.mHeading / PRECISION;
    playerControllerC->mShoot = msg.mShoot;
    playerControllerC->mShootAlt = msg.mShootAlt;
    if (msg.mUseNormalItem.IsActive())
    {
        playerControllerC->mUseNormalItem.Activate();
    }
    else
    {
        playerControllerC->mUseNormalItem.Deactivate();
    }
    if (msg.mUseReload.IsActive())
    {
        playerControllerC->mUseReload.Activate();
    }
    else
    {
        playerControllerC->mUseReload.Deactivate();
    }
    playerControllerC->mMoving = msg.mMoving;
    if (msg.mActivate.IsActive())
    {
        playerControllerC->mActivate.Activate();
    }
    else
    {
        playerControllerC->mActivate.Deactivate();
    }
    if (msg.mSwitchWeapon.IsActive())
    {
        playerControllerC->mSwitchWeapon.Activate();
    }
    else
    {
        playerControllerC->mSwitchWeapon.Deactivate();
    }
    if (msg.mSwitchNormalItem.IsActive())
    {
        playerControllerC->mSwitchNormalItem.Activate();
    }
    else
    {
        playerControllerC->mSwitchNormalItem.Deactivate();
    }
}


bool PlayerControllerMessage::operator==( PlayerControllerMessage const& other )
{
    return mActorGUID == other.mActorGUID
        && mOrientation == other.mOrientation
        && mHeading == other.mHeading
        && mShoot == other.mShoot
        && mShootAlt == other.mShootAlt
        && mUseNormalItem == other.mUseNormalItem
        && mUseReload == other.mUseReload
        && mMoving == other.mMoving
        && mActivate == other.mActivate
        && mSwitchWeapon == other.mSwitchWeapon
        && mSwitchNormalItem == other.mSwitchNormalItem;
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__PlayerControllerMessage, network::PlayerControllerMessage );
