#include "core/map/neighbors.h"
#include "../border_type.h"

namespace map {

Neighbors::Neighbors()
{
}

bool Neighbors::IsMatching( NeighborMap_t const& neighborMap )
{
    if ( mNeighbors.size() < BorderType::Num_Classes )
    {
        return false;
    }
    for( NeighborMap_t::const_iterator i = neighborMap.begin(), e = neighborMap.end(); i != e; ++i )
    {
        Neighbor& neighbor = mNeighbors[( *i ).first];
        if ( neighbor.mActorGUID == -1 && ( *i ).second != Other )
        {
            return false;
        }
        if ( neighbor.mActorGUID != -1 && ( *i ).second != Same )
        {
            return false;
        }
    }
    return true;
}


Neighbors::NeighborDirMap_t const& Neighbors::GetNeighborDirs()
{
    static NeighborDirMap_t mNeighborDirMap;
    if (mNeighborDirMap.empty())
    {
        Neighbors::NeighborMap_t neighborMap;
        neighborMap[BorderType::Top] = Neighbors::Other;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //top

        neighborMap[BorderType::Top] = Neighbors::Same;
        neighborMap[BorderType::TopRight] = Neighbors::Other;
        neighborMap[BorderType::Right] = Neighbors::Same;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //topright

        neighborMap[BorderType::Right] = Neighbors::Other;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //right

        neighborMap[BorderType::Right] = Neighbors::Same;
        neighborMap[BorderType::BottomRight] = Neighbors::Other;
        neighborMap[BorderType::Bottom] = Neighbors::Same;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //bottomright

        neighborMap[BorderType::Bottom] = Neighbors::Other;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //bottom

        neighborMap[BorderType::Left] = Neighbors::Same;
        neighborMap[BorderType::BottomLeft] = Neighbors::Other;
        neighborMap[BorderType::Bottom] = Neighbors::Same;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //bottomleft

        neighborMap[BorderType::Left] = Neighbors::Other;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //left

        neighborMap[BorderType::Top] = Neighbors::Same;
        neighborMap[BorderType::TopLeft] = Neighbors::Other;
        neighborMap[BorderType::Left] = Neighbors::Same;
        mNeighborDirMap.push_back( neighborMap );
        neighborMap.clear();    //topleft
    }
    return mNeighborDirMap;
}

Neighbors::NeighborDirMap_t const& Neighbors::GetNeighborOuterDirs()
{
    static NeighborDirMap_t mNeighborOuterDirMap;
    if (mNeighborOuterDirMap.empty())
    {
        Neighbors::NeighborMap_t neighborMap;
        neighborMap[BorderType::Top] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //top

        neighborMap[BorderType::Top] = Neighbors::Other;
        neighborMap[BorderType::TopRight] = Neighbors::Other;
        neighborMap[BorderType::Right] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //topright

        neighborMap[BorderType::Right] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //right

        neighborMap[BorderType::Right] = Neighbors::Other;
        neighborMap[BorderType::BottomRight] = Neighbors::Other;
        neighborMap[BorderType::Bottom] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //bottomright

        neighborMap[BorderType::Bottom] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //bottom

        neighborMap[BorderType::Left] = Neighbors::Other;
        neighborMap[BorderType::BottomLeft] = Neighbors::Other;
        neighborMap[BorderType::Bottom] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //bottomleft

        neighborMap[BorderType::Left] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //left

        neighborMap[BorderType::Top] = Neighbors::Other;
        neighborMap[BorderType::TopLeft] = Neighbors::Other;
        neighborMap[BorderType::Left] = Neighbors::Other;
        mNeighborOuterDirMap.push_back( neighborMap );
        neighborMap.clear();    //topleft
    }
    return mNeighborOuterDirMap;
}

IBorderComponent::Borders_t Neighbors::GetBorders( NeighborDirMap_t const& neighborDirs )
{
    IBorderComponent::Borders_t r;
    for (size_t i = 0; i < BorderType::Num_Classes; ++i)
    {
        if (IsMatching( neighborDirs[i] ))
        {
            r.push_back( static_cast<BorderType::Type>(i) );
        }
    }
    return r;
}

Neighbor::Neighbor( int32_t actorGUID )
    : mActorGUID( actorGUID )
{
}

} // namespace map

