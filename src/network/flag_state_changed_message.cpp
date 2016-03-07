#include "platform/i_platform.h"
#include "network/flag_state_changed_message.h"
#include "core/i_attachable_component.h"
#include "position_message_sender_system.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

FlagStateChangedMessageSenderSystem::FlagStateChangedMessageSenderSystem()
    : MessageSenderSystem()
{
}


void FlagStateChangedMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnFlagStateChanged = EventServer< ::ctf::FlagStateChangedEvent>::Get().Subscribe( boost::bind( &FlagStateChangedMessageSenderSystem::OnFlagStateChanged, this, _1 ) );
}


void FlagStateChangedMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void FlagStateChangedMessageSenderSystem::OnFlagStateChanged( ::ctf::FlagStateChangedEvent const& Evt )
{
    std::auto_ptr<FlagStateChangedMessage> flagStateChangedMsg( new FlagStateChangedMessage );
    flagStateChangedMsg->mType = Evt.mType;
    flagStateChangedMsg->mTeam = Evt.mTeam;
    flagStateChangedMsg->mCarrierGUID = Evt.mCarrierGUID;
    flagStateChangedMsg->mFlagGUID = Evt.mFlagGUID;
    mMessageHolder.AddOutgoingMessage( flagStateChangedMsg );
    if ( Evt.mType ==::ctf::FlagStateChangedEvent::Scored
         || Evt.mType ==::ctf::FlagStateChangedEvent::Returned )
    {
        Opt<Actor> actor( mScene.GetActor( Evt.mFlagGUID ) );
        if ( actor.IsValid() )
        {
            mMessageHolder.AddOutgoingMessage( PositionMessageSenderSystem::GeneratePositionMessage( *actor.Get() ) );
        }
    }
}

FlagStateChangedMessageHandlerSubSystem::FlagStateChangedMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void FlagStateChangedMessageHandlerSubSystem::Init()
{
}

void FlagStateChangedMessageHandlerSubSystem::Update( double DeltaTime )
{
    MessageHandlerSubSystem::Update( DeltaTime );
}

void FlagStateChangedMessageHandlerSubSystem::Execute( Message const& message )
{
    FlagStateChangedMessage const& msg = static_cast<FlagStateChangedMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mFlagGUID );
    if ( !actor.IsValid() )
    {
        L1( "cannot find actor with GUID: (%s) %d \n", __FUNCTION__, msg.mFlagGUID );
        return;
    }
    Opt<ctf::IAttachableComponent> actorAttachableC = actor->Get<ctf::IAttachableComponent>();
    if ( !actorAttachableC.IsValid() )
    {
        return;
    }
    if ( msg.mType ==::ctf::FlagStateChangedEvent::Scored
         || msg.mType ==::ctf::FlagStateChangedEvent::Returned
         || msg.mType ==::ctf::FlagStateChangedEvent::Dropped )
    {
        actorAttachableC->SetAttachedGUID( -1 );
    }
    else
    {
        actorAttachableC->SetAttachedGUID( msg.mCarrierGUID );
    }
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__FlagStateChangedMessage, network::FlagStateChangedMessage );
