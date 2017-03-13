#include "json_room.h"
#include "core/i_position_component.h"
#include "platform/settings.h"
#include "platform/id_storage.h"
#include "room_property.h"

namespace map {

JsonRoom::JsonRoom( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
    , mMapElementFactory( MapElementFactory::Get() )
    , mPropertyFactory( PropertyFactory::Get() )
{
    mRoomDesc.SetRoom( this );
}

void JsonRoom::Generate( RoomDesc& roomDesc, glm::vec2 pos, bool editor /*= false*/ )
{
    auto& idStorage = IdStorage::Get();
    std::string targetName;
    if (idStorage.GetName( mId, targetName ))
    {
        LL() << "room generate: " << targetName;
    }

    for (auto&& prop : mProperties)
    {
        prop.Generate( roomDesc, mMapElementHolder, pos, editor );
    }
    LL() << "room generate end!";
}

void JsonRoom::ClearMapElements()
{
    for (auto& mapElement : mMapElementHolder.mAllMapElements)
    {
        delete mapElement.Get();
    }
    mMapElementHolder.mAllMapElements.clear();
}

void JsonRoom::Load( Json::Value const& setters )
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
        for (auto&& prop : props)
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


void JsonRoom::Save( Json::Value& setters, RoomDesc const& roomDesc )
{
    IdStorage& idStorage = IdStorage::Get();
    std::string name;
    if (idStorage.GetName( mId, name ))
    {
        setters["name"] = name;
    }
    roomDesc.Save( setters );
    Json::Value propArray( Json::arrayValue );
    for (auto&& prop : mProperties)
    {
        Json::Value propObject( Json::objectValue );
        prop.Save( propObject );
        propArray.append( propObject );
    }
    setters["properties"] = propArray;
}

} // namespace map

