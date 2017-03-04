#include "platform/i_platform.h"
#include "network/border_message.h"
#include "core/i_border_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

BorderMessageSenderSystem::BorderMessageSenderSystem()
    : MessageSenderSystem()
{
}


void BorderMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void BorderMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

BorderMessageHandlerSubSystem::BorderMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void BorderMessageHandlerSubSystem::Init()
{
}

void BorderMessageHandlerSubSystem::Update( double DeltaTime )
{
    PendingMessageHandlerSubSystem::Update( DeltaTime );
}

bool BorderMessageHandlerSubSystem::ProcessPending( Message const& message )
{
    BorderMessage const& msg = static_cast<BorderMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID ); //guaranteed
    L2( "executing %s: actorGUID %d \n", __FUNCTION__, msg.mActorGUID );
    Opt<IBorderComponent> borderC = actor->Get<IBorderComponent>();
    if ( !borderC.IsValid() )
    {
        L1( "borderC not valid" );
        return true;
    }
    borderC->SetBorders( msg.mBorders );
    borderC->SetOuterBorders( msg.mOuterBorders );
    return true;
}

std::auto_ptr<BorderMessage> BorderMessageSenderSystem::GenerateBorderMessage( Actor& actor )
{
    Opt<IBorderComponent> borderC = actor.Get<IBorderComponent>();
    if ( !borderC.IsValid() )
    {
        return std::auto_ptr<BorderMessage>();
    }
    std::auto_ptr<BorderMessage> borderMsg( new BorderMessage );
    borderMsg->mActorGUID = actor.GetGUID();
    borderMsg->mBorders = borderC->GetBorders();
    borderMsg->mOuterBorders = borderC->GetOuterBorders();
    return borderMsg;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__BorderMessage, network::BorderMessage );
