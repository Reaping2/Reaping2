#include "platform/i_platform.h"
#include "core/map/base_output.h"

namespace map {
    
BaseOutput::BaseOutput()
    : mNextOrdinal(0)
{

}

void BaseOutput::PlugInNode(int32_t ordinal, int_function_t node)
{
    BOOST_ASSERT(mOutputNodes.find(ordinal)!=mOutputNodes.end());
    BOOST_ASSERT(mOrdinalIdBimap.left.find(ordinal)!=mOrdinalIdBimap.left.end());
    mOutputNodes.find(ordinal)->second.push_back(node);
    int32_t id=mOrdinalIdBimap.left.find(ordinal)->second;
    BOOST_ASSERT(mOutputNodesId.find(id)!=mOutputNodesId.end());
    mOutputNodesId[id].push_back(node);
}

void BaseOutput::PlugInNodeId(int32_t id, int_function_t node)
{
    BOOST_ASSERT(mOutputNodesId.find(id)!=mOutputNodesId.end());
    BOOST_ASSERT(mOrdinalIdBimap.right.find(id)!=mOrdinalIdBimap.right.end());
    mOutputNodesId[id].push_back(node);
    int32_t ordinal=mOrdinalIdBimap.right.find(id)->second;
    BOOST_ASSERT(mOutputNodes.find(ordinal)!=mOutputNodes.end());
    mOutputNodes.find(ordinal)->second.push_back(node);
}


void BaseOutput::DoOutput(int32_t ordinal, int32_t val)
{
    BOOST_ASSERT(mOutputNodes.find(ordinal)!=mOutputNodes.end());
    OutputNodes_t::iterator node=mOutputNodes.find(ordinal);
    for(PluggedInFunctions_t::iterator i=node->second.begin(), e=node->second.end();i!=e;++i)
    {
        (*i)(val);
    }
}

void BaseOutput::DoOutputId(int32_t id, int32_t val)
{
    BOOST_ASSERT(mOutputNodesId.find(id)!=mOutputNodesId.end());
    PluggedInFunctions_t& node=mOutputNodesId[id];
    for(PluggedInFunctions_t::iterator i=node.begin(), e=node.end();i!=e;++i)
    {
        (*i)(val);
    }
}

void BaseOutput::AddOutputNode(int32_t id)
{
    BOOST_ASSERT(mOutputNodesId.find(id)==mOutputNodesId.end());
    BOOST_ASSERT(mOutputNodes.find(mNextOrdinal)==mOutputNodes.end());
    mOrdinalIdBimap.insert(OrdinalIdPair(mNextOrdinal++,id));
}

BaseOutput::~BaseOutput()
{

}

} // namespace map
