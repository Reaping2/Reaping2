#include "platform/i_platform.h"
#include "core/map/map_element.h"

namespace map {
MapElement::~MapElement()
{
}

MapElement::MapElement(int32_t Id)
    : mUID(0)
    , mId(Id)
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

void MapElement::Save(Json::Value& Element)
{
    std::string elementName;
    if (IdStorage::Get().GetName(mId,elementName))
    {
        Element["name"]=Json::Value(elementName);
    }
    std::string identifierName;
    if (IdStorage::Get().GetName(mUID,identifierName))
    {
        Element["identifier"]=Json::Value(identifierName);
    }
}

void MapElement::SetUID(int32_t uId)
{
    mUID=uId;
}

DefaultMapElement::DefaultMapElement(int32_t Id)
    : MapElement(Id)
{

}

} // namespace map
