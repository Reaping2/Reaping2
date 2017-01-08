#include "is_not_hit_condition.h"
#include "../i_health_component.h"
#include "platform/game_clock.h"
namespace scriptedcontroller
{

void IsNotHitCondition::Update( Actor& actor, double Seconds )
{
    auto healthC( actor.Get<IHealthComponent>() );
    if (!healthC.IsValid())
    {
        return;
    }
    mLastDamageTime = healthC->GetLastDamageTime();
}

bool IsNotHitCondition::IsSatisfied() const
{
    return platform::Clock::Now()-mLastDamageTime>mTimeToSatisfy;
}

void IsNotHitCondition::Reset( Actor& actor )
{
    mSatisfied = false;
}

void IsNotHitCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    Json::GetDouble( setters["for"], mTimeToSatisfy );
}

} // namespace scriptedcontroller

