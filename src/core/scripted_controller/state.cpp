#include "state.h"
#include "act_factory.h"

namespace scriptedcontroller {

State::State( int32_t Id )
    : mIdentifier(Id)
{

}

void State::Update( double Seconds )
{

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

} // namespace scriptedcontroller
