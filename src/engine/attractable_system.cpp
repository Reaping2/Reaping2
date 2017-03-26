#include "platform/i_platform.h"
#include "attractable_system.h"
#include "core/i_attractable_component.h"
#include "core/i_position_component.h"
#include "core/i_target_holder_component.h"
#include "core/i_acceleration_component.h"
#include "core/i_move_component.h"
#include "../core/i_health_component.h"

namespace engine {

AttractableSystem::AttractableSystem()
    : mScene( Scene::Get() )
{
}


void AttractableSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IAttractableComponent>().IsValid()
            && actor.Get<ITargetHolderComponent>().IsValid()
            && actor.Get<IAccelerationComponent>().IsValid()
            && actor.Get<IMoveComponent>().IsValid(); } );
}


void AttractableSystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        auto attractableC( actor->Get<IAttractableComponent>() );
        auto accelerationC( actor->Get<IAccelerationComponent>() );
        auto targetHolderC( actor->Get<ITargetHolderComponent>() );
        auto moveC( actor->Get<IMoveComponent>() );
        if (!attractableC.IsValid() || !accelerationC.IsValid() || !targetHolderC.IsValid() || !moveC.IsValid())
        {
            continue;
        }

        auto const accel = accelerationC->GetAcceleration();
        auto currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
        targetHolderC->SetTargetGUID( -1 );
        accelerationC->SetAcceleration( -1 * attractableC->GetDeceleration() );

        if (currentTarget.IsValid())
        {
            auto healthC( currentTarget->Get<IHealthComponent>() );
            if (healthC.IsValid() && healthC->IsAlive())
            {
                accelerationC->SetAcceleration( accel );
                targetHolderC->SetTargetGUID( currentTarget->GetGUID() );
                moveC->SetMoving( true );
                attractableC->GetTurnToTargetAct().Update( *actor, DeltaTime );
            }
        }
    }
}


} // namespace engine

