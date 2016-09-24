#ifndef INCLUDED_MAP_NEIGHBORS_H
#define INCLUDED_MAP_NEIGHBORS_H
#include "platform/i_platform.h"
#include "../border_type.h"
#include "../i_border_component.h"

namespace map {

struct Neighbor
{
    int32_t mActorGUID;
    Neighbor( int32_t actorGUID );
};

class Neighbors
{
public:
    enum Mapping
    {
        Same = 0, //neighbor is the same
        Other,  //neighbor is something else
    };
    typedef std::map<BorderType::Type, Mapping> NeighborMap_t;
    Neighbors();
    typedef std::vector<Neighbor> Neighbors_t;
    Neighbors_t mNeighbors;
    bool IsMatching( NeighborMap_t const& neighborMap );
    typedef std::vector<Neighbors::NeighborMap_t> NeighborDirMap_t;
    NeighborDirMap_t mNeighborDirMap;
    NeighborDirMap_t mNeighborOuterDirMap;
    static NeighborDirMap_t const& GetNeighborDirs();
    static NeighborDirMap_t const& GetNeighborOuterDirs();
    IBorderComponent::Borders_t GetBorders( NeighborDirMap_t const& neighborDirs );
};

} // namespace map

#endif//INCLUDED_MAP_NEIGHBORS_H


//command:  "classgenerator.exe" -g "enum" -n "map" -c "neighbors" -m "a-a"
