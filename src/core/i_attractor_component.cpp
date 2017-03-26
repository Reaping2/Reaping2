#include "i_attractor_component.h"

void IAttractorComponent::AttractorDesc::Load( Json::Value const& setters )
{
    {
        auto const& json = setters["collision_classes"];
        if (json.isArray())
        {
            for (auto& part : json)
            {
                static auto& collisionClass = CollisionClass::Get();
                std::string istr;
                mCollisionClassMask = 0;
                if (Json::GetStr( part, istr ))
                {
                    mCollisionClassMask |= 1 << collisionClass( AutoId( istr ) );
                }
                
            }
        }
    }
    {
        auto const& json = setters["attract_types"];
        if (json.isArray())
        {
            for (auto& part : json)
            {
                std::string istr;
                if (Json::GetStr( part, istr ))
                {
                    mAttractTypes.insert( AutoId( istr ) );
                }
            }
        }
    }
    Json::GetDouble( setters["range"], mRange );
    Json::GetDouble( setters["acceleration"], mAcceleration );
}
