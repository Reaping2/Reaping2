#include "platform/i_platform.h"
#include "core/map/map_element.h"

namespace map {
MapElement::~MapElement()
{
}

MapElement::MapElement()
    : mUID(0)
{
//     static int32_t NextUID = 0;
//     mUID = ++NextUID;
}

int32_t MapElement::GetUID()
{
    return mUID;
}

DefaultMapElement::DefaultMapElement()
    : MapElement()
{

}

void DefaultMapElement::Load(Json::Value& setters)
{

}
} // namespace map
