#ifndef INCLUDED_CORE_MAP_LINK_MAP_ELEMENT_H
#define INCLUDED_CORE_MAP_LINK_MAP_ELEMENT_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"

namespace map {

class LinkMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE(LinkMapElement)
    LinkMapElement();
    void Load(Json::Value& setters);
private:
    int32_t mTargetInputUID;
    int32_t mTargetInputOrdinal;

    int32_t mTargetOutputUID;
    int32_t mTargetOutputOrdinal;
};
} // namespace map
#endif//INCLUDED_CORE_MAP_LINK_MAP_ELEMENT_H
