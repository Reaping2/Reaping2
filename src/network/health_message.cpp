#include "platform/i_platform.h"
#include "network/health_message.h"
#include "core/i_health_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

HealthMessageSenderSystem::HealthMessageSenderSystem()
    : MessageSenderSystem()
{
}


void HealthMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnHealthChanged = EventServer<core::HealthChangedEvent>::Get().Subscribe( boost::bind( &HealthMessageSenderSystem::OnHealthChanged, this, _1 ) );

}

void HealthMessageSenderSystem::OnHealthChanged( core::HealthChangedEvent const& Evt )
{
    mMessageHolder.AddOutgoingMessage( GenerateHealthMessage( Evt.mActor ) );
}


void HealthMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

HealthMessageHandlerSubSystem::HealthMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void HealthMessageHandlerSubSystem::Init()
{
}

void HealthMessageHandlerSubSystem::Update( double DeltaTime )
{
    PendingMessageHandlerSubSystem::Update( DeltaTime );
}

bool HealthMessageHandlerSubSystem::ProcessPending( Message const& message )
{
    HealthMessage msg = static_cast<HealthMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
    Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
    if ( !healthC.IsValid() )
    {
        L1( "no health_component found guid:%s\n", msg.mActorGUID );
        return true;
    }
    L2( "executing %s: actorGUID %d \n", __FUNCTION__, msg.mActorGUID );

    healthC->SetHP( msg.mHP );
    healthC->GetMaxHP().mBase.Set( msg.mMaxHP );
    healthC->GetMaxHP().mPercent.Set( msg.mMaxHPPercent / PRECISION );
    healthC->GetMaxHP().mFlat.Set( msg.mMaxHPFlat );
    return true;
}


std::auto_ptr<HealthMessage> HealthMessageSenderSystem::GenerateHealthMessage( Actor& actor )
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( !healthC.IsValid() )
    {
        return std::auto_ptr<HealthMessage>();
    }
    std::auto_ptr<HealthMessage> healthMsg( new HealthMessage );
    healthMsg->mActorGUID = actor.GetGUID();
    healthMsg->mHP = healthC->GetHP();
    healthMsg->mMaxHP = healthC->GetMaxHP().mBase.Get();
    healthMsg->mMaxHPPercent = std::floor( healthC->GetMaxHP().mPercent.Get() * PRECISION );
    healthMsg->mMaxHPFlat = healthC->GetMaxHP().mFlat.Get();
    return healthMsg;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__HealthMessage, network::HealthMessage );
