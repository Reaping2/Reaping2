#include "platform/i_platform.h"
#include "network/fade_out_message.h"
#include "core/i_fade_out_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

FadeOutMessageSenderSystem::FadeOutMessageSenderSystem()
    : MessageSenderSystem()
{
}


void FadeOutMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void FadeOutMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

FadeOutMessageHandlerSubSystem::FadeOutMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void FadeOutMessageHandlerSubSystem::Init()
{
}

void FadeOutMessageHandlerSubSystem::Update( double DeltaTime )
{
    PendingMessageHandlerSubSystem::Update( DeltaTime );
}

bool FadeOutMessageHandlerSubSystem::ProcessPending( Message const& message )
{
    FadeOutMessage const& msg = static_cast<FadeOutMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID ); //guaranteed
    L2( "executing %s: actorGUID %d \n", __FUNCTION__, msg.mActorGUID );
    Opt<IFadeOutComponent> fadeOutC = actor->Get<IFadeOutComponent>();
    if ( fadeOutC.IsValid() )
    {
        fadeOutC->SetSecsToEnd( msg.mSecsToEnd );
    }
    return true;
}

std::auto_ptr<FadeOutMessage> FadeOutMessageSenderSystem::GenerateFadeOutMessage( Actor& actor )
{
    Opt<IFadeOutComponent> fadeOutC = actor.Get<IFadeOutComponent>();
    if ( !fadeOutC.IsValid() )
    {
        return std::auto_ptr<FadeOutMessage>();
    }
    std::auto_ptr<FadeOutMessage> fadeOutMsg( new FadeOutMessage );
    fadeOutMsg->mActorGUID = actor.GetGUID();
    fadeOutMsg->mSecsToEnd = fadeOutC->GetSecsToEnd();
    return fadeOutMsg;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__FadeOutMessage, network::FadeOutMessage );
