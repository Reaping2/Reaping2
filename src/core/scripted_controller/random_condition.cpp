#include "random_condition.h"
namespace scriptedcontroller
{

void RandomCondition::Update( Actor& actor, double Seconds )
{
    mTimer.Update( Seconds );
    if (mTimer.IsTime()&&!mSatisfied)
    {
        mSatisfied = RandomGenerator::global()() % 10000 < mPercent * 100;
    }
}

bool RandomCondition::IsSatisfied() const
{
    return mSatisfied;
}

void RandomCondition::Reset( Actor& actor )
{
    mTimer.Reset();
    mSatisfied = false;
}

void RandomCondition::Load( Json::Value const& setters )
{
    ICondition::Load( setters );
    mTimer.Load( setters["frequency"] );
    Json::GetDouble( setters["percent"], mPercent );
}

} // namespace scriptedcontroller

