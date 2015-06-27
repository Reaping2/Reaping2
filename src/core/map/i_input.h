#ifndef INCLUDED_CORE_MAP_I_INPUT_H
#define INCLUDED_CORE_MAP_I_INPUT_H

#include "platform/auto_id.h"
#include "core/opt.h"
#include "boost/function.hpp"
namespace map {

typedef boost::function<void( int32_t )> int_function_t;

class IInput 
{
public:
    virtual ~IInput()=0;
    virtual int_function_t GetInputNode(int32_t ordinal)=0;
protected:
    virtual void AddInputNode(int32_t ordinal, int_function_t inputNode)=0;

};


} // namespace map
#endif//INCLUDED_CORE_MAP_I_INPUT_H
