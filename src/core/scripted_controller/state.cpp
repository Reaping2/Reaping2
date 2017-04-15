#include "state.h"
#include "act_factory.h"
#include "platform/random.h"
#include "core/map/level_generator/random.h"

namespace scriptedcontroller {

State::State( int32_t Id )
    : mIdentifier(Id)
{

}

void State::UpdateTransitions( Actor& actor, double Seconds )
{
    bool isActInterruptible = false;
    for (int i = 0; i < mActGroups.size(); ++i)
    {
        auto& index = mCurrActIndices[i];
        auto& acts = mActGroups[i];
        isActInterruptible |= index == -1
            || acts[index].IsInterruptible()
            || !acts[index].IsRunning();
        if (!isActInterruptible)
        {
            break;
        }
    }
    for (auto&& transition : mTransitions)
    {
        transition.Update( actor, Seconds );
        if (isActInterruptible&&transition.IsConditionsSatisfied())
        {
            mNextIdentifier = transition.GetTargetStateIdentifier();
            static IdStorage& idStorage = IdStorage::Get();
            std::string nextState;
            if (idStorage.GetName( mNextIdentifier, nextState ))
            {
                L2( "Transition hit! next state: %s, id: %d \n", nextState.c_str(), mNextIdentifier );
            }
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
    for (int i = 0; i < mActGroups.size(); ++i)
    {
        auto& index = mCurrActIndices[i];
        auto& acts = mActGroups[i];
        if (acts.empty())
        {
            continue;
        }
        if (index == -1 || !acts[index].IsRunning())
        {
            if (index != -1)
            {
                acts[index].Stop( actor );
            }
            ShuffleActIndices(i);
            index = mWeightedActIndicesGroups[i].front(); // guaranteed
            acts[index].Start( actor );
        }
        acts[index].Update( actor, Seconds );
    }

}

void State::Reset( Actor& actor )
{
    for (auto&& transition : mTransitions)
    {
        transition.Reset( actor );
    }
    for (int i = 0; i < mActGroups.size(); ++i)
    {
        auto& index = mCurrActIndices[i];
        auto& acts = mActGroups[i];
        if (index != -1)
        {
            acts[index].Stop( actor );
        }
        index = -1;
    }
    mNextIdentifier = mIdentifier;
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
            mActGroups.resize( json.size() );
            mCurrActIndices.resize( json.size(), -1 );
            mWeightedActIndicesGroups.resize( json.size() );
            for (int i = 0; i < json.size();++i)
            {
                auto& acts = mActGroups[i];
                auto& weightedActIndices = mWeightedActIndicesGroups[i];
                auto const& group = json[i];
                if (group.isArray())
                {
                    acts.reserve( group.size() );
                    for (auto& part : group)
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
                            std::fill_n( std::back_inserter( weightedActIndices ), weight, acts.size() );
                            acts.push_back( act );
                        }
                    }
                }
            }
        }
    }
    mNextIdentifier = mIdentifier;
}

bool State::IsStart()
{
    return mIsStart;
}

int32_t State::GetIdentifier() const
{
    return mIdentifier;
}

void State::ShuffleActIndices( int32_t ind )
{
    // TODO: see pickup_profiles::Roll()
    std::shuffle( mWeightedActIndicesGroups[ind].begin(), mWeightedActIndicesGroups[ind].end(), map::mRand );
}

} // namespace scriptedcontroller
