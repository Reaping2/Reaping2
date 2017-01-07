#include "is_hit_condition.h"
#include "..\i_health_component.h"
namespace scriptedcontroller
{

void IsHitCondition::Update( Actor& actor, double Seconds )
{
    auto healthC( actor.Get<IHealthComponent>() );
    if (!healthC.IsValid())
    {
        return;
    }
    if (mLastDamageTime != healthC->GetLastDamageTime())
    {
        mLastDamageTime = healthC->GetLastDamageTime();
        mIsHit = true;
    }
}

bool IsHitCondition::IsSatisfied() const
{
    return mIsHit;
}

void IsHitCondition::Reset( Actor& actor )
{
    mIsHit = false;
}

void IsHitCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
}

} // namespace scriptedcontroller

