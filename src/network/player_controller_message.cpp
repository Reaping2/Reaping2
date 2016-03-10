#include "network/player_controller_message.h"
#include "core/player_controller_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
namespace network {

PlayerControllerMessageSenderSystem::PlayerControllerMessageSenderSystem()
    : MessageSenderSystem()
{

}

void PlayerControllerMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    SetFrequency( 10 );
}

void PlayerControllerMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
    if ( !IsTime() )
    {
        return;
    }
    //TODO: might need optimization
    Opt<Actor> actor = mScene.GetActor( mProgramState.mControlledActorGUID );
    if ( actor.IsValid() )
    {
        Opt<PlayerControllerComponent> playerControllerC = actor->Get<PlayerControllerComponent>();
        if ( playerControllerC.IsValid() )
        {
            std::auto_ptr<PlayerControllerMessage> playerControllerMsg( new PlayerControllerMessage );
            playerControllerMsg->mActorGUID = actor->GetGUID();
            playerControllerMsg->mOrientation = std::floor( playerControllerC->mOrientation * PRECISION );
            playerControllerMsg->mHeading = std::floor( playerControllerC->mHeading * PRECISION );
            playerControllerMsg->mShoot = playerControllerC->mShoot;
            playerControllerMsg->mShootAlt = playerControllerC->mShootAlt;
            playerControllerMsg->mUseNormalItem = playerControllerC->mUseNormalItem;
            playerControllerMsg->mUseReload = playerControllerC->mUseReload;
            playerControllerMsg->mMoving = playerControllerC->mMoving;
            mMessageHolder.AddOutgoingMessage( playerControllerMsg );
        }
    }
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

    Opt<PlayerControllerComponent> playerControllerC = actor->Get<PlayerControllerComponent>();
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
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__PlayerControllerMessage, network::PlayerControllerMessage );
