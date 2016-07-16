#include "json_room.h"
#include "core/i_position_component.h"
#include "platform/settings.h"

namespace map {

JsonRoom::JsonRoom( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
    , mMapElementFactory( MapElementFactory::Get() )
{
    mRoomDesc.SetRoom( this );
}

void JsonRoom::Generate( RoomDesc& roomDesc, glm::vec2 pos )
{
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
}

} // namespace map

