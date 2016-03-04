#include "remove_components_on_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

RemoveComponentsOnDeathComponent::RemoveComponentsOnDeathComponent()
{
}

void RemoveComponentsOnDeathComponent::SetComponents(std::vector<int32_t> const& components)
{
    mComponents=components;
}

std::vector<int32_t> const& RemoveComponentsOnDeathComponent::GetComponents()const
{
    return mComponents;
}



void RemoveComponentsOnDeathComponentLoader::BindValues()
{
    Json::Value const& json = (*mSetters)["components"];
    if( json.isArray() )
    {
        std::vector<int32_t> components;
        for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
        {
            Json::Value& part = *i;
            int32_t typ = AutoId( part.asString() );
            components.push_back( typ );
        }
        Bind< std::vector<int32_t> >( &RemoveComponentsOnDeathComponent::SetComponents, components );
    }
}

RemoveComponentsOnDeathComponentLoader::RemoveComponentsOnDeathComponentLoader()
{
}

BOOST_CLASS_EXPORT_IMPLEMENT( RemoveComponentsOnDeathComponent )

