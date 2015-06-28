#ifndef INCLUDED_CORE_MAP_BASE_INPUT_H
#define INCLUDED_CORE_MAP_BASE_INPUT_H
#include "platform/i_platform.h"

#include "i_input.h"
#include <map>

namespace map {

class BaseInput : public IInput
{
public:
    ~BaseInput();
    int_function_t GetInputNode(int32_t ordinal);
    int_function_t GetInputNodeId(int32_t id);
protected:
    void AddInputNode(int32_t ordinal, int_function_t inputNode);
    template<typename BASE, typename FUN>
    void AddInputNode(int32_t ordinal, BASE* b, FUN f);
    typedef std::map<int32_t, int_function_t> InputNodesId_t;
    InputNodesId_t mInputNodesId;
    typedef std::vector<int_function_t> InputNodes_t;
    InputNodes_t mInputNodes;
};

template<typename BASE, typename FUN>
void map::BaseInput::AddInputNode(int32_t ordinal, BASE* b, FUN f)
{
    AddInputNode(ordinal,boost::bind(f,b,_1));
}

} // namespace map
#endif//INCLUDED_CORE_MAP_BASE_INPUT_H
