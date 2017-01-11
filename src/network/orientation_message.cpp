#include "network/orientation_message.h"
#include "core/i_position_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
namespace network {

OrientationMessageSenderSystem::OrientationMessageSenderSystem()
    : MessageSenderSystem()
{

}

void OrientationMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    SetFrequency( 0.01 );
    //mSendOrientations.insert(platform::AutoId("player"));
    if ( mProgramState.mMode == ProgramState::Server )
    {
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.3, platform::AutoId( "spider1" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.3, platform::AutoId( "spider2" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.3, platform::AutoId( "spider1target" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.3, platform::AutoId( "spider2target" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.0, platform::AutoId( "player" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.0, platform::AutoId( "ctf_player" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.0, platform::AutoId( "flag" ) ) );
    }
    else if ( mProgramState.mMode == ProgramState::Client )
    {
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.0, platform::AutoId( "player" ) ) );
        mActorFrequencyTimerHolder.Add( ActorFrequencyTimer( 0.0, platform::AutoId( "ctf_player" ) ) );
    }
}
void OrientationMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
    mActorFrequencyTimerHolder.Update( DeltaTime );
    if ( !IsTime() )
    {
        return;
    }
    if ( mProgramState.mMode == ProgramState::Server )
    {
        mSendOrientations = mActorFrequencyTimerHolder.GetActorIds();
        //TODO: might need optimization
        for ( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
        {
            Actor& actor = **it;
            if ( mSendOrientations.find( actor.GetId() ) == mSendOrientations.end() )
            {
                continue;
            }
            std::auto_ptr<OrientationMessage> orientationMessage( GenerateOrientationMessage( actor ) );
            if ( orientationMessage.get() != NULL )
            {
                mSingleMessageSender.Add( actor.GetGUID(), orientationMessage );
            }
        }
    }
    else if ( mProgramState.mMode == ProgramState::Client )
    {
        Opt<Actor> player( mScene.GetActor( mProgramState.mControlledActorGUID ) );
        if ( player.IsValid() )
        {
            std::auto_ptr<OrientationMessage> orientationMessage( GenerateOrientationMessage( *player ) );
            if ( orientationMessage.get() != NULL )
            {
                mSingleMessageSender.Add( player->GetGUID(), orientationMessage );
            }
        }
    }
}

std::auto_ptr<OrientationMessage> OrientationMessageSenderSystem::GenerateOrientationMessage( Actor& actor )
{
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    if ( !positionC.IsValid() )
    {
        return std::auto_ptr<OrientationMessage>();
    }
    std::auto_ptr<OrientationMessage> orientationMessage( new OrientationMessage );
    orientationMessage->mOrientation = std::floor( positionC->GetOrientation() * PRECISION );
    orientationMessage->mActorGUID = actor.GetGUID();
    return orientationMessage;
}

OrientationMessageHandlerSubSystem::OrientationMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void OrientationMessageHandlerSubSystem::Init()
{

}

void OrientationMessageHandlerSubSystem::Execute( Message const& message )
{
    OrientationMessage const& msg = static_cast<OrientationMessage const&>( message );

    if ( mProgramState.mMode == ProgramState::Server
         || ( mProgramState.mMode == ProgramState::Client && msg.mActorGUID != mProgramState.mControlledActorGUID ) )
    {
        Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
        if ( !actor.IsValid() )
        {
            L1( "cannot find actor with GUID: (that is not possible) %d \n", msg.mActorGUID );
            return;
        }

        Opt<IPositionComponent> positionC( actor->Get<IPositionComponent>() );
        if ( !positionC.IsValid() )
        {
            L1( "position is called on an actor that has no position_component \n" );
            return;
        }
        positionC->SetOrientation( msg.mOrientation / PRECISION );
    }
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__OrientationMessage, network::OrientationMessage );
