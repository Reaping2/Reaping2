#include "distance_to_players_condition.h"
#include "../i_health_component.h"
#include "../../engine/engine.h"
#include "../../engine/path_system.h"
#include "../scene.h"
#include "../program_state.h"
namespace scriptedcontroller
{

void DistanceToPlayersCondition::Update( Actor& actor, double Seconds )
{
    static auto mPathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
    static auto& mProgramState = core::ProgramState::Get();
    static auto& mScene = Scene::Get();
    if (mTimer.IsTime() || mTimer.Update( Seconds ))
    {
        mDistance = std::numeric_limits<double>::max();
        for (auto const& clientData : mProgramState.mClientDatas)
        {
            auto target( mScene.GetActor( clientData.mClientActorGUID ) );
            if (target.IsValid())
            {
                double const distance = mPathSystem->GetDistance( actor, *target );
                if (distance < mDistance)
                {
                    mDistance = distance;
                    L1( "found player satisfied %d.\n", mDistance );
                }
            }
        }
    }
}

bool DistanceToPlayersCondition::IsSatisfied() const
{
    if (mGreaterThan <= mDistance && mDistance <= mLessThan)
    {
        L1( "distance_to_players satisfied less: %d, dist: %d.\n",mLessThan,mDistance );
    }
    return mGreaterThan<=mDistance && mDistance<=mLessThan;
}

void DistanceToPlayersCondition::Reset( Actor& actor )
{
    mDistance = std::numeric_limits<double>::max();
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void DistanceToPlayersCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    mTimer.Load( setters["frequency"] );

    Json::GetDouble( setters["less"], mLessThan );
    Json::GetDouble( setters["greater"], mGreaterThan );
}

} // namespace scriptedcontroller

