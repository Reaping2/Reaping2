#include "distance_to_target_condition.h"
#include "../i_health_component.h"
#include "../../engine/engine.h"
#include "../../engine/path_system.h"
#include "../scene.h"
#include "../i_target_holder_component.h"
namespace scriptedcontroller
{

void DistanceToTargetCondition::Update( Actor& actor, double Seconds )
{
    auto targetHolderC( actor.Get<ITargetHolderComponent>() );
    if (!targetHolderC.IsValid())
    {
        return;
    }
    auto mPathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
    auto& mScene = Scene::Get();
    if (mTimer.IsTime() || mTimer.Update( Seconds ))
    {
        Opt<Actor> currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
        if (currentTarget.IsValid())
        {
            mDistance = mPathSystem->GetDistance( actor, *currentTarget );
        }
        else
        {
            mDistance = std::numeric_limits<double>::max();
        }
    }
}

bool DistanceToTargetCondition::IsSatisfied() const
{
    if (mGreaterThan <= mDistance && mDistance <= mLessThan)
    {
        L2( "distance_to_target satisfied\n" );
    }
    return mGreaterThan <= mDistance && mDistance <= mLessThan;
}

void DistanceToTargetCondition::Reset( Actor& actor )
{
    mDistance = std::numeric_limits<double>::max();
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void DistanceToTargetCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    mTimer.Load( setters["frequency"] );

    Json::GetDouble( setters["less"], mLessThan );
    Json::GetDouble( setters["greater"], mGreaterThan );
}

} // namespace scriptedcontroller

