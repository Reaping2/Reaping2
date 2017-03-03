#ifndef INCLUDED_MAP_GROUP_H
#define INCLUDED_MAP_GROUP_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"

namespace map {

class GroupMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE(GroupMapElement)
    GroupMapElement( int32_t Id );
    virtual void Load( Json::Value const& setters );
    virtual void Save( Json::Value& Element ); 
    typedef std::set<int32_t> Targets_t;
    void SetTargets( Targets_t targets );
    Targets_t const& GetTargets() const;
private:
    Targets_t mTargets;
};

} // namespace map

#endif//INCLUDED_MAP_GROUP_H

//command:  "classgenerator.exe" -g "map_element" -c "group" -m "std::vector<int32_t>-targets"
