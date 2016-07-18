#include "core/map/level_generator/i_property.h"

namespace map {

IProperty::IProperty( int32_t Id )
    : mId( Id )
{
}

void IProperty::Load( Json::Value& setters )
{
}

int32_t IProperty::GetId() const
{
    return mId;
}


void IProperty::Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos )
{
}

DefaultProperty::DefaultProperty( int32_t Id )
    : IProperty( Id )
{

}

} // namespace map
