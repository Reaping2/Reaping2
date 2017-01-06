#include "distance_to_aggressor_condition.h"
#include "..\i_health_component.h"
#include "..\..\engine\engine.h"
#include "..\..\engine\path_system.h"
#include "..\scene.h"
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
    mTimer.Update( Seconds );
    if (mTimer.IsTime() && healthC->GetLastDamageOwnerGUID() != -1)
    {
        Opt<Actor> currentTarget( mScene.GetActor( healthC->GetLastDamageOwnerGUID() ) );
        mDistance = mPathSystem->GetDistance( actor, *currentTarget );
    }
}

bool DistanceToAggressorCondition::IsSatisfied() const
{
    return mGreaterThan<=mDistance && mDistance<=mLessThan;
}

void DistanceToAggressorCondition::Reset( Actor& actor )
{
    mDistance = std::numeric_limits<int32_t>::max();
    mTimer.Reset();
}

void DistanceToAggressorCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    double frequency = 0.1;
    Json::GetDouble( setters["frequency"], frequency );
    mTimer.SetFrequency( frequency * 1000 );

    Json::GetInt( setters["less"], mLessThan );
    Json::GetInt( setters["greater"], mGreaterThan );
}

} // namespace scriptedcontroller

