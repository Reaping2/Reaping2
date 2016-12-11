#include "transition.h"
#include "condition_factory.h"
namespace scriptedcontroller {

void Transition::Update( double Seconds )
{

}

void Transition::Load( Json::Value const& setters )
{
    {
        Json::Value const& json = setters["conditions"];
        if (json.isArray())
        {
            for (auto& part : json)
            {
                static auto& mConditionFactory( ConditionFactory::Get() );
                std::string str;
                int32_t id = 0;
                if (Json::GetStr( part["name"], str ))
                {
                    id = AutoId( str );
                    auto condition = mConditionFactory( id );
                    condition->Load( part );
                    mConditions.push_back( condition );
                }
            }
        }
    }
    {
        std::string str;
        if (Json::GetStr( setters["state_id"], str ))
        {
            mTargetStateId = AutoId( str );
        }
    }
}

} // namespace scriptedcontroller
