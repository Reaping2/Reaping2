#ifndef INCLUDED_CORE_MAP_LINK_MAP_ELEMENT_H
#define INCLUDED_CORE_MAP_LINK_MAP_ELEMENT_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"

namespace map {

class LinkMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE( LinkMapElement )
    LinkMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    void SetTargetInputUID( int32_t targetInputUID );
    int32_t GetTargetInputUID()const;
    void SetTargetInputNodeId( int32_t targetInputNodeId );
    int32_t GetTargetInputNodeId()const;
    void SetTargetInputNodeOrdinal( int32_t targetInputNodeOrdinal );
    int32_t GetTargetInputNodeOrdinal()const;
    void SetTargetOutputUID( int32_t targetOutputUID );
    int32_t GetTargetOutputUID()const;
    void SetTargetOutputNodeId( int32_t targetOutputNodeId );
    int32_t GetTargetOutputNodeId()const;
    void SetTargetOutputNodeOrdinal( int32_t targetOutputNodeOrdinal );
    int32_t GetTargetOutputNodeOrdinal()const;
private:
    int32_t mTargetInputUID;
    int32_t mTargetInputNodeId;
    int32_t mTargetInputNodeOrdinal;
    int32_t mTargetOutputUID;
    int32_t mTargetOutputNodeId;
    int32_t mTargetOutputNodeOrdinal;
};

} // namespace map
#endif//INCLUDED_CORE_MAP_LINK_MAP_ELEMENT_H
