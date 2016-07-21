#include "core/map/level_generator/i_property.h"

namespace map {

IProperty::IProperty( int32_t Id )
    : mId( Id )
{
}

void IProperty::Load( Json::Value& setters )
{
}


void IProperty::Save( Json::Value& setters ) const
{
    auto& idStorage = IdStorage::Get();
    std::string propName;
    if (idStorage.GetName( mId, propName ))
    {
        Json::Value jName = Json::Value( propName );
        setters["name"] = jName;
    }

}

int32_t IProperty::GetId() const
{
    return mId;
}


void IProperty::Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos, bool editor/*= false*/)
{
}

DefaultProperty::DefaultProperty( int32_t Id )
    : IProperty( Id )
{

}

} // namespace map
