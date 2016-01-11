#include "core/map/neighbors.h"
#include "../border_type.h"

namespace map {

Neighbors::Neighbors()
{
}

bool Neighbors::IsMatching(NeighborMap_t const& neighborMap)
{
    if (mNeighbors.size()<BorderType::Num_Classes)
    {
        return false;
    }
    for(NeighborMap_t::const_iterator i=neighborMap.begin(), e=neighborMap.end();i!=e;++i)
    {
        Neighbor& neighbor=mNeighbors[(*i).first];
        if (neighbor.mActorGUID==-1&&(*i).second!=Other)
        {
            return false;
        }
        if (neighbor.mActorGUID!=-1&&(*i).second!=Same)
        {
            return false;
        }
    }
    return true;
}


Neighbor::Neighbor(int32_t actorGUID)
    : mActorGUID(actorGUID)
{
}

} // namespace map

