#include "distance_to_players_condition.h"
#include "core/i_health_component.h"
#include "core/scene.h"
#include "core/program_state.h"
namespace scriptedcontroller
{

void DistanceToPlayersCondition::Update( Actor& actor, double Seconds )
{
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
                double const distance = core::GetDistance( actor, *target, mSeekPath );
                if (distance < mDistance)
                {
                    mDistance = distance;
                    L2( "found player satisfied %d.\n", mDistance );
                }
            }
        }
    }
}

void DistanceToPlayersCondition::Reset( Actor& actor )
{
    DistanceCondition::Reset( actor );
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void DistanceToPlayersCondition::Load( Json::Value const& setters )
{
    DistanceCondition::Load( setters );
    mTimer.Load( setters["frequency"] );
}

} // namespace scriptedcontroller

