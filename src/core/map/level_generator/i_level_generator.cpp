#include "i_level_generator.h"
#include "platform/auto_id.h"
#include "random.h"
#include "platform/settings.h"

using platform::AutoId;

namespace map {

ILevelGenerator::ILevelGenerator( int32_t Id )
    : mId( Id )
    , mScene( Scene::Get() )
{
    mCellSize = Settings::Get().GetInt( "generator.cell_size", 1000 );
    mCellCount = Settings::Get().GetInt( "generator.cell_count", 5 );;
    mRand.seed( Settings::Get().GetUInt( "generator.seed", unsigned( std::time( 0 ) ) ) );
}


ILevelGenerator::~ILevelGenerator()
{

}

void ILevelGenerator::AddPossibleRoom( int32_t roomId, int32_t possibility )
{
    for (int i = 0; i < possibility; ++i)
    {
        mPossibleRooms.push_back( roomId );
    }
}

GGraphNode::GGraphNode( int32_t ind, NeighbourRooms_t const& nodes )
    : mIndex(ind)
    , mNeighbours(nodes)
{

}


int32_t GGraphNode::operator[]( int32_t neighbourIndex ) const
{
    return mNeighbours[neighbourIndex];
}


int32_t GGraphNode::Size() const
{
    return mNeighbours.size();
}

void GGraphNode::ShuffleNeighbours()
{
    std::shuffle( mNeighbours.begin(), mNeighbours.end(), mRand );
}

void GGraph::ShuffleNodeNeighbours()
{
    for (auto& node : mNodes)
    {
        node.ShuffleNeighbours();
    }
}


map::GGraphNode const& GGraph::operator[]( int32_t nodeIndex ) const
{
    return mNodes[nodeIndex];
}


void GGraph::Clear()
{
    mNodes.clear();
}


void GGraph::AddNode( GGraphNode const& node )
{
    mNodes.push_back( node );
}

} // namespace map

