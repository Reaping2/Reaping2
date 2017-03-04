#include "platform/i_platform.h"
#include "network/accuracy_message.h"
#include "core/i_accuracy_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

AccuracyMessageSenderSystem::AccuracyMessageSenderSystem()
    : MessageSenderSystem()
{
    //This one is not used yet you should wire it before use
}


void AccuracyMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    //This one is not used yet you should wire it before use
}


void AccuracyMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
    //This one is not used yet you should wire it before use
}

AccuracyMessageHandlerSubSystem::AccuracyMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void AccuracyMessageHandlerSubSystem::Init()
{
}

void AccuracyMessageHandlerSubSystem::Update( double DeltaTime )
{
    PendingMessageHandlerSubSystem::Update( DeltaTime );
}

bool AccuracyMessageHandlerSubSystem::ProcessPending( Message const& message )
{
    AccuracyMessage const& msg = static_cast<AccuracyMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID ); //guaranteed

    Opt<IAccuracyComponent> accuracyC = actor->Get<IAccuracyComponent>();
    if ( !accuracyC.IsValid() )
    {
        L1( "no accuracy_component found guid:%s\n", msg.mActorGUID );
        return true;
    }
    L2( "executing %s: actorGUID %d\n", __FUNCTION__, msg.mActorGUID );
    accuracyC->GetAccuracy().mBase.Set( msg.mAccuracy );
    accuracyC->GetAccuracy().mPercent.Set( msg.mAccuracyPercent / PRECISION );
    accuracyC->GetAccuracy().mFlat.Set( msg.mAccuracyFlat );
    return true;
}

std::auto_ptr<AccuracyMessage> AccuracyMessageSenderSystem::GenerateAccuracyMessage( Actor& actor )
{
    Opt<IAccuracyComponent> accuracyC = actor.Get<IAccuracyComponent>();
    if ( !accuracyC.IsValid() )
    {
        return std::auto_ptr<AccuracyMessage>();
    }
    std::auto_ptr<AccuracyMessage> accuracyMsg( new AccuracyMessage );
    accuracyMsg->mActorGUID = actor.GetGUID();
    accuracyMsg->mAccuracy = accuracyC->GetAccuracy().mBase.Get();
    accuracyMsg->mAccuracyPercent = std::floor( accuracyC->GetAccuracy().mPercent.Get() * PRECISION );
    accuracyMsg->mAccuracyFlat = accuracyC->GetAccuracy().mFlat.Get();
    return accuracyMsg;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__AccuracyMessage, network::AccuracyMessage );
