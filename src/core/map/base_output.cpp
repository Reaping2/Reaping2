#include "platform/i_platform.h"
#include "core/map/base_output.h"

namespace map {


BaseOutput::~BaseOutput()
{

}

void BaseOutput::PlugInNode(int32_t ordinal, int_function_t node)
{
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

} // namespace map
