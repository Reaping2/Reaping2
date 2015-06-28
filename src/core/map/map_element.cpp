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

void MapElement::Load(Json::Value& setters)
{
    std::string identifier;
    if (Json::GetStr(setters["identifier"],identifier))
    {
        mUID=AutoId(identifier);
    }
}

DefaultMapElement::DefaultMapElement()
    : MapElement()
{

}

} // namespace map
