#include "distance_to_target_condition.h"
#include "core/i_health_component.h"
#include "core/scene.h"
#include "core/i_target_holder_component.h"
#include "core/i_position_component.h"
namespace scriptedcontroller
{

void DistanceToTargetCondition::Update( Actor& actor, double Seconds )
{
    auto targetHolderC( actor.Get<ITargetHolderComponent>() );
    if (!targetHolderC.IsValid())
    {
        return;
    }
    auto& mScene = Scene::Get();
    if (mTimer.IsTime() || mTimer.Update( Seconds ))
    {
        Opt<Actor> target( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
        mDistance = target.IsValid() 
            ? core::GetDistance( actor, *target, mSeekPath ) 
            : std::numeric_limits<double>::max();
    }
}

void DistanceToTargetCondition::Reset( Actor& actor )
{
    DistanceCondition::Reset( actor );
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void DistanceToTargetCondition::Load( Json::Value const& setters )
{
    DistanceCondition::Load( setters );
    mTimer.Load( setters["frequency"] );
}

} // namespace scriptedcontroller

