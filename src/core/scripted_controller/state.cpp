#include "state.h"
#include "act_factory.h"

namespace scriptedcontroller {

State::State( int32_t Id )
    : mIdentifier(Id)
{

}

void State::UpdateTransitions( Actor& actor, double Seconds )
{
    bool isActInterruptible = mCurrActIndex == -1 
        || mActs[mCurrActIndex].IsInterruptible() 
        || !mActs[mCurrActIndex].IsRunning();
    for (auto&& transition : mTransitions)
    {
        transition.Update( actor, Seconds );
        if (isActInterruptible&&transition.IsConditionsSatisfied())
        {
            mNextIdentifier = transition.GetTargetStateIdentifier();
            break;
        }
    }
}

int32_t State::GetNextStateIdentifier() const
{
    return mNextIdentifier;
}

void State::Update( Actor& actor, double Seconds )
{
    if (mNextIdentifier == -1)
    {
        mNextIdentifier = mIdentifier;
    }
    if (mActs.empty())
    {
        return;
    }
    if (mCurrActIndex == -1 || !mActs[mCurrActIndex].IsRunning())
    {
        mActs[mCurrActIndex].Stop( actor );
        ShuffleActIndices();
        mCurrActIndex = *mActIndices.begin(); // guaranteed
        mActs[mCurrActIndex].Start( actor );
    }
    mActs[mCurrActIndex].Update( actor, Seconds );
}

void State::Reset( Actor& actor )
{
    for (auto&& transition : mTransitions)
    {
        transition.Reset( actor );
    }
    if (mCurrActIndex == -1)
    {
        mActs[mCurrActIndex].Stop( actor );
    }
    mCurrActIndex = -1;
}

void State::Load( Json::Value const& setters )
{
    int32_t i;
    if (Json::GetInt( setters["start"], i ))
    {
        mIsStart = i != 0;
    }
    {
        Json::Value const& json = setters["transitions"];
        if (json.isArray())
        {
            for (auto& part : json)
            {
                Transition transition;
                transition.Load( part );
                mTransitions.push_back( transition );
            }
        }
    }
    {
        Json::Value const& json = setters["acts"];
        if (json.isArray())
        {
            for (auto& part : json)
            {
                static auto& mActFactory( ActFactory::Get() );
                std::string str;
                int32_t id = -1;
                if (Json::GetStr( part["name"], str ))
                {
                    id = AutoId( str );
                    auto act = mActFactory( id );
                    act->Load( part["params"] );
                    int32_t weight = 1;
                    Json::GetInt( part["weight"], weight );
                    std::fill_n( std::back_inserter( mActIndices ), weight, mActs.size() );
                    mActs.push_back( act );
                }
            }
        }
    }
}

bool State::IsStart()
{
    return mIsStart;
}

int32_t State::GetIdentifier() const
{
    return mIdentifier;
}

void State::ShuffleActIndices()
{
    std::random_shuffle( mActIndices.begin(), mActIndices.end() );
}

} // namespace scriptedcontroller
