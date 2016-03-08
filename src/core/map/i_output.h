#ifndef INCLUDED_CORE_MAP_I_OUTPUT_H
#define INCLUDED_CORE_MAP_I_OUTPUT_H

#include "platform/auto_id.h"
#include "core/opt.h"
#include "boost/function.hpp"
#include "function_declarations.h"
namespace map {

class IOutput
{
public:
    virtual ~IOutput() {};
    virtual void PlugInNode( int32_t ordinal, int_function_t node ) = 0;
    virtual void PlugInNodeId( int32_t id, int_function_t node ) = 0;
    virtual void DoOutput( int32_t ordinal, int32_t val ) = 0;
    virtual void DoOutputId( int32_t id, int32_t val ) = 0;
protected:
    virtual void AddOutputNodeId( int32_t id ) = 0;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_I_OUTPUT_H
