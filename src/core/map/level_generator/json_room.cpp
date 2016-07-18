#include "json_room.h"
#include "core/i_position_component.h"
#include "platform/settings.h"

namespace map {

JsonRoom::JsonRoom( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
    , mMapElementFactory( MapElementFactory::Get() )
    , mPropertyFactory( PropertyFactory::Get() )
{
    mRoomDesc.SetRoom( this );
}

void JsonRoom::Generate( RoomDesc& roomDesc, glm::vec2 pos )
{
    for (auto&& prop : mProperties)
    {
        prop.Generate( roomDesc, mMapElementHolder, pos );
    }
}

void JsonRoom::ClearMapElements()
{
    for (auto& mapElement : mMapElementHolder.mAllMapElements)
    {
        delete mapElement.Get();
    }
    mMapElementHolder.mAllMapElements.clear();
}

void JsonRoom::Load( Json::Value& setters )
{
    ClearMapElements();
    mRoomDesc.Load( setters );
    auto& mapElements = setters["map_elements"];
    if (mapElements.isArray())
    {
        for (auto& mapElementDesc : mapElements)
        {
            std::string name;
            if (Json::GetStr( mapElementDesc["name"], name ))
            {
                auto mapElement( mMapElementFactory( AutoId( name ) ) );
                mapElement->Load( mapElementDesc );
                mMapElementHolder.mAllMapElements.insert( mapElement.release() );
            }
        }
    }
    mProperties.clear();
    auto& props = setters["properties"];
    if (props.isArray())
    {
        for (auto& prop : props)
        {
            std::string str;
            if (Json::GetStr( prop["name"], str ))
            {
                auto Property = mPropertyFactory( AutoId( str ) );
                Property->Load( prop );
                mProperties.push_back( Property );
            }
        }
    }

}


JsonRoom::Properties_t const& JsonRoom::GetProperties() const
{
    return mProperties;
}

} // namespace map

