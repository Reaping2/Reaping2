#ifndef INCLUDED_CORE_MAP_I_INPUT_H
#define INCLUDED_CORE_MAP_I_INPUT_H

#include "platform/auto_id.h"
#include "core/opt.h"
#include "boost/function.hpp"
#include "function_declarations.h"
namespace map {

class IInput
{
public:
    virtual ~IInput() {};
    virtual int_function_t GetInputNode( int32_t ordinal ) = 0;
    virtual int_function_t GetInputNodeId( int32_t id ) = 0;
protected:
    virtual void AddInputNodeId( int32_t id, int_function_t inputNode ) = 0;

};


} // namespace map
#endif//INCLUDED_CORE_MAP_I_INPUT_H
