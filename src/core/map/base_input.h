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
    int_function_t GetInputNode( int32_t ordinal );
    int_function_t GetInputNodeId( int32_t id );
    void ResetValues();
    void ResetValueId( int32_t id );
    int32_t GetValueId( int32_t id );
    BaseInput& operator=( BaseInput const& other );
    BaseInput( BaseInput const& other );
    BaseInput() = default;
protected:
    void AddInputNodeId( int32_t id, int_function_t inputNode );
    template<typename BASE, typename FUN>
    void AddInputNodeId( int32_t id, BASE* b, FUN f );
    void AddInputNodeId( int32_t id );
    typedef std::map<int32_t, int_function_t> InputNodesId_t;
    InputNodesId_t mInputNodesId;
    typedef std::vector<int_function_t> InputNodes_t;
    InputNodes_t mInputNodes;
private:
    typedef std::map<int32_t, int32_t> BaseInputNodesId_t;
    BaseInputNodesId_t mBaseInputNodesId;
    void BaseInputNode( int32_t in, int32_t id );
};

template<typename BASE, typename FUN>
void map::BaseInput::AddInputNodeId( int32_t ordinal, BASE* b, FUN f )
{
    AddInputNodeId( ordinal, boost::bind( f, b, _1 ) );
}

} // namespace map
#endif//INCLUDED_CORE_MAP_BASE_INPUT_H
