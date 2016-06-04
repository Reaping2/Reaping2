#include "platform/i_platform.h"
#include "core/map/base_input.h"

namespace map {


BaseInput::~BaseInput()
{

}

void BaseInput::AddInputNodeId( int32_t id, int_function_t inputNode )
{
    BOOST_ASSERT( mInputNodesId.find( id ) == mInputNodesId.end() );
    mInputNodesId.insert( std::make_pair( id, inputNode ) );
    mInputNodes.push_back( inputNode );
}

void BaseInput::AddInputNodeId( int32_t id )
{
    BOOST_ASSERT( mBaseInputNodesId.find( id ) == mBaseInputNodesId.end() );
    mBaseInputNodesId[id] = 0;
    AddInputNodeId( id, boost::bind( &BaseInput::BaseInputNode, this, _1, id ) );
}

int_function_t BaseInput::GetInputNodeId( int32_t id )
{
    BOOST_ASSERT( mInputNodesId.find( id ) != mInputNodesId.end() );
    return int_function_t( mInputNodesId[id] );
}

int_function_t BaseInput::GetInputNode( int32_t ordinal )
{
    BOOST_ASSERT( 0 <= ordinal && ordinal < mInputNodes.size() );
    return int_function_t( mInputNodes[ordinal] );
}

void BaseInput::BaseInputNode( int32_t in, int32_t id )
{
    mBaseInputNodesId[id] += in;
}

void BaseInput::ResetValues()
{
    for( BaseInputNodesId_t::iterator it = mBaseInputNodesId.begin(), e = mBaseInputNodesId.end(); it != e; ++it )
    {
        it->second = 0;
    }
}

int32_t BaseInput::GetValueId( int32_t id ) const
{
    auto it = mBaseInputNodesId.find( id );
    BOOST_ASSERT( it != mBaseInputNodesId.end() );
    return it->second;
}

void BaseInput::ResetValueId( int32_t id )
{
    auto it = mBaseInputNodesId.find( id );
    BOOST_ASSERT( it != mBaseInputNodesId.end() );
    it->second = 0;
}

} // namespace map
