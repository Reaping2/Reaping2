#include "distance_to_aggressor_condition.h"
#include "../i_health_component.h"
#include "../../engine/engine.h"
#include "../../engine/path_system.h"
#include "../scene.h"
namespace scriptedcontroller
{

void DistanceToAggressorCondition::Update( Actor& actor, double Seconds )
{
    auto healthC( actor.Get<IHealthComponent>() );
    if (!healthC.IsValid())
    {
        return;
    }
    auto mPathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
    auto& mScene = Scene::Get();
    if ((mTimer.IsTime() || mTimer.Update( Seconds )) && healthC->GetLastDamageOwnerGUID() != -1)
    {
        Opt<Actor> currentTarget( mScene.GetActor( healthC->GetLastDamageOwnerGUID() ) );
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

bool DistanceToAggressorCondition::IsSatisfied() const
{
    return mGreaterThan<=mDistance && mDistance<=mLessThan;
}

void DistanceToAggressorCondition::Reset( Actor& actor )
{
    mDistance = std::numeric_limits<double>::max();
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void DistanceToAggressorCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    mTimer.Load( setters["frequency"] );

    Json::GetDouble( setters["less"], mLessThan );
    Json::GetDouble( setters["greater"], mGreaterThan );
}

} // namespace scriptedcontroller

