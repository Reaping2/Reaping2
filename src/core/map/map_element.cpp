#include "platform/i_platform.h"
#include "core/map/map_element.h"

namespace map {

MapElement::~MapElement()
{
}

MapElement::MapElement( int32_t Id )
    : mIdentifier( 0 )
    , mId( Id )
    , mSpawnedActorGUID( -1 )
{
    static int32_t NextUID = 0;
    mUID = ++NextUID;
}

int32_t MapElement::GetIdentifier()
{
    return mIdentifier;
}

void MapElement::Load( Json::Value& setters )
{
    std::string identifier;
    if ( Json::GetStr( setters["identifier"], identifier ) )
    {
        mIdentifier = AutoId( identifier );
    }
}

void MapElement::Save( Json::Value& Element )
{
    std::string elementName;
    if ( IdStorage::Get().GetName( mId, elementName ) )
    {
        Element["name"] = Json::Value( elementName );
    }
    std::string identifierName;
    if ( IdStorage::Get().GetName( mIdentifier, identifierName ) )
    {
        Element["identifier"] = Json::Value( identifierName );
    }
}

void MapElement::SetIdentifier( int32_t uId )
{
    mIdentifier = uId;
}

void MapElement::SetSpawnedActorGUID( int32_t spawnedActorGUID )
{
    mSpawnedActorGUID = spawnedActorGUID;
}

int32_t MapElement::GetSpawnedActorGUID() const
{
    return mSpawnedActorGUID;
}


int32_t MapElement::GetUID() const
{
    return mUID;
}

DefaultMapElement::DefaultMapElement( int32_t Id )
    : MapElement( Id )
{

}

} // namespace map

