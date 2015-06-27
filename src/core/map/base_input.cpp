#include "platform/i_platform.h"
#include "core/map/base_input.h"

namespace map {


BaseInput::~BaseInput()
{

}

int_function_t BaseInput::GetInputNode(int32_t ordinal)
{
    BOOST_ASSERT(mInputNodes.find(ordinal)!=mInputNodes.end());
    return int_function_t(mInputNodes[ordinal]);
}

void BaseInput::AddInputNode(int32_t ordinal, int_function_t inputNode)
{
    BOOST_ASSERT(mInputNodes.find(ordinal)==mInputNodes.end());
    mInputNodes.insert(std::make_pair(ordinal,inputNode));
}

} // namespace map
