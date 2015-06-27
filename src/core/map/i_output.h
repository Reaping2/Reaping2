#ifndef INCLUDED_CORE_MAP_I_OUTPUT_H
#define INCLUDED_CORE_MAP_I_OUTPUT_H

#include "platform/auto_id.h"
#include "core/opt.h"
#include "boost/function.hpp"
namespace map {

typedef boost::function<void( int32_t )> int_function_t;

class IOutput 
{
public:
    virtual ~IOutput()=0;
    virtual void PlugInNode(int32_t ordinal, int_function_t node)=0;
protected:
    virtual void DoOutput(int32_t ordinal, int32_t val)=0;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_I_OUTPUT_H
