#include "distance_to_aggressor_condition.h"
#include "core/i_health_component.h"
#include "core/scene.h"
namespace scriptedcontroller
{

void DistanceToAggressorCondition::Update( Actor& actor, double Seconds )
{
    auto healthC( actor.Get<IHealthComponent>() );
    if (!healthC.IsValid())
    {
        return;
    }
    auto& mScene = Scene::Get();
    if ((mTimer.IsTime() || mTimer.Update( Seconds )) && healthC->GetLastDamageOwnerGUID() != -1)
    {
        Opt<Actor> target( mScene.GetActor( healthC->GetLastDamageOwnerGUID() ) );
        mDistance = target.IsValid()
            ? core::GetDistance( actor, *target, mSeekPath )
            : std::numeric_limits<double>::max();
    }
}

void DistanceToAggressorCondition::Reset( Actor& actor )
{
    DistanceCondition::Reset( actor );
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void DistanceToAggressorCondition::Load( Json::Value const& setters )
{
    DistanceCondition::Load( setters );
    mTimer.Load( setters["frequency"] );
}

} // namespace scriptedcontroller

