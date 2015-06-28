#include "platform/i_platform.h"
#include "core/map/base_input.h"

namespace map {


BaseInput::~BaseInput()
{

}

void BaseInput::AddInputNode(int32_t id, int_function_t inputNode)
{
    BOOST_ASSERT(mInputNodesId.find(id)==mInputNodesId.end());
    mInputNodesId.insert(std::make_pair(id,inputNode));
    mInputNodes.push_back(inputNode);
}

int_function_t BaseInput::GetInputNodeId(int32_t id)
{
    BOOST_ASSERT(mInputNodesId.find(id)!=mInputNodesId.end());
    return int_function_t(mInputNodesId[id]);
}

int_function_t BaseInput::GetInputNode(int32_t ordinal)
{
    BOOST_ASSERT(0<=ordinal&&ordinal<mInputNodes.size());
    return int_function_t(mInputNodes[ordinal]);
}

} // namespace map
