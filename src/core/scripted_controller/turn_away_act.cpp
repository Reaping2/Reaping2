#include "turn_away_act.h"
#include "engine/path_system.h"
#include "engine/engine.h"
#include "../i_target_holder_component.h"
#include "../i_position_component.h"
#include "../i_move_component.h"
#include "../../engine/collision_system.h"
#include "../i_collision_component.h"

namespace {
    template <typename T> int sgn( T val ) {
        return (T( 0 ) < val) - (val < T( 0 ));
    }
}

namespace scriptedcontroller
{

void TurnAwayAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
    auto positionC( actor.Get<IPositionComponent>() );
    if (!positionC.IsValid())
    {
        return;
    }

    static double const pi = boost::math::constants::pi<double>();

    if (std::abs(mTurn) - std::abs(mTurnCurrent) > 0.01)
    {
        double const turn = sgn( mTurn )*mSpeed*Seconds;
        mTurnCurrent += turn;
        positionC->SetOrientation( positionC->GetOrientation() + turn );
    }
    else
    {
        mDurationCurrent = 0.0;
    }
}

void TurnAwayAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
    double deg = 100;
    Json::GetDouble( setters["speed"], deg );
    mSpeed = deg / 180 * boost::math::constants::pi<double>();
    deg = 90;
    Json::GetDouble( setters["degree"], deg );
    mTurnMax = deg / 180 * boost::math::constants::pi<double>();
    deg = 0;
    Json::GetDouble( setters["degree_variance"], deg );
    mTurnVariance = deg / 180 * boost::math::constants::pi<double>();
}


void TurnAwayAct::Start( Actor& actor )
{
    IAct::Start( actor );
    mTurnCurrent = 0.0;
    mTurn = std::max<int>( 0.0, mTurnMax + 2 * mTurnVariance * (RandomGenerator::global()() % 100) / 100 - mTurnVariance )
        * (RandomGenerator::global()() % 2==0?1:-1);
}

void TurnAwayAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
    mTurnCurrent = 0.0;
}


} // namespace scriptedcontroller

