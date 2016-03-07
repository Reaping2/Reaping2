#include "platform/i_platform.h"
#include "core/map/base_output.h"

namespace map {

BaseOutput::BaseOutput()
    : mNextOrdinal( 0 )
{

}

void BaseOutput::PlugInNode( int32_t ordinal, int_function_t node )
{
    BOOST_ASSERT( 0 <= ordinal && ordinal < mOutputNodes.size() );
    BOOST_ASSERT( mOrdinalIdBimap.left.find( ordinal ) != mOrdinalIdBimap.left.end() );
    mOutputNodes[ordinal].push_back( node );
    int32_t id = mOrdinalIdBimap.left.find( ordinal )->second;
    BOOST_ASSERT( mOutputNodesId.find( id ) != mOutputNodesId.end() );
    mOutputNodesId.find( id )->second.push_back( node );
}

void BaseOutput::PlugInNodeId( int32_t id, int_function_t node )
{
    BOOST_ASSERT( mOutputNodesId.find( id ) != mOutputNodesId.end() );
    BOOST_ASSERT( mOrdinalIdBimap.right.find( id ) != mOrdinalIdBimap.right.end() );
    mOutputNodesId.find( id )->second.push_back( node );
    int32_t ordinal = mOrdinalIdBimap.right.find( id )->second;
    BOOST_ASSERT( 0 <= ordinal && ordinal < mOutputNodes.size() );
    mOutputNodes[ordinal].push_back( node );
}


void BaseOutput::DoOutput( int32_t ordinal, int32_t val )
{
    BOOST_ASSERT( 0 <= ordinal && ordinal < mOutputNodes.size() );
    PluggedInFunctions_t& node = mOutputNodes[ordinal];
    for( PluggedInFunctions_t::iterator i = node.begin(), e = node.end(); i != e; ++i )
    {
        ( *i )( val );
    }
}

void BaseOutput::DoOutputId( int32_t id, int32_t val )
{
    BOOST_ASSERT( mOutputNodesId.find( id ) != mOutputNodesId.end() );
    OutputNodesId_t::iterator node = mOutputNodesId.find( id );
    for( PluggedInFunctions_t::iterator i = node->second.begin(), e = node->second.end(); i != e; ++i )
    {
        ( *i )( val );
    }
}

void BaseOutput::AddOutputNodeId( int32_t id )
{
    BOOST_ASSERT( mOutputNodesId.find( id ) == mOutputNodesId.end() );
    BOOST_ASSERT( mNextOrdinal == mOutputNodes.size() );
    mOrdinalIdBimap.insert( OrdinalIdPair( mNextOrdinal++, id ) );
    mOutputNodesId[id] = PluggedInFunctions_t();
    mOutputNodes.push_back( PluggedInFunctions_t() );
}

BaseOutput::~BaseOutput()
{

}

} // namespace map
