#include "platform/i_platform.h"
#include "core/map/base_input.h"

namespace map {


BaseInput::~BaseInput()
{

}

void BaseInput::AddInputNode(int32_t ordinal, int_function_t inputNode)
{
    BOOST_ASSERT(mInputNodes.find(ordinal)==mInputNodes.end());
    mInputNodes.insert(std::make_pair(ordinal,inputNode));
    mInputNodesId.push_back(inputNode);
}

int_function_t BaseInput::GetInputNodeId(int32_t id)
{
    BOOST_ASSERT(mInputNodesId.find(ordinal)!=mInputNodesId.end());
    return int_function_t(mInputNodesId[id]);
}

int_function_t BaseInput::GetInputNode(int32_t ordinal)
{
    BOOST_ASSERT(mInputNodes.find(ordinal)!=mInputNodes.end());
    return int_function_t(mInputNodes[ordinal]);
}

} // namespace map
