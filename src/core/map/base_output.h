#ifndef INCLUDED_CORE_MAP_BASE_OUTPUT_H
#define INCLUDED_CORE_MAP_BASE_OUTPUT_H
#include "platform/i_platform.h"

#include "i_output.h"
#include <map>
#include <list>

namespace map {

class BaseOutput : public IOutput
{
public:
    ~BaseOutput();
    void PlugInNode(int32_t ordinal, int_function_t node);
protected:
    typedef std::list<int_function_t> PluggedInFunctions_t;
    typedef std::map<int32_t, PluggedInFunctions_t> OutputNodes_t;
    OutputNodes_t mOutputNodes;
    void DoOutput(int32_t ordinal, int32_t val);
};


} // namespace map
#endif//INCLUDED_CORE_MAP_BASE_OUTPUT_H
