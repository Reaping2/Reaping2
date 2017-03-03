#ifndef INCLUDED_MAP_RECREATE_BORDERS_H
#define INCLUDED_MAP_RECREATE_BORDERS_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "base_input.h"

namespace map {

class RecreateBordersMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE(RecreateBordersMapElement)
    RecreateBordersMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    static int32_t RecreateNodeId();
private:
};

} // namespace map

#endif//INCLUDED_MAP_RECREATE_BORDERS_H

//command:  "classgenerator.exe" -g "map_element" -c "recreate_borders"
