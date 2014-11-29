#include "core/damage_action.h"
#include "core/i_health_component.h"
#include "platform/i_platform.h"
#include "core/actor.h" //TODO : this sucks

#include <algorithm>

DamageAction::DamageAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mAreCancelledActionsExcluded = false;
    mIsSelfDestruct = true;
    mSecsToEnd = 0;
}

bool DamageAction::Activate()
{
    if(!Action::Activate())
    {
        return false;
    }
    Opt<IHealthComponent> healthC = mActor->Get<IHealthComponent>();
    if ( healthC.IsValid() )
    {
        healthC->SetHP(healthC->GetHP()-std::min<int32_t>(healthC->GetHP(),mDamage));
    }
    return true;
}

void DamageAction::SetDamage(int32_t Damage)
{
    mDamage = Damage;
}