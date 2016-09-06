#ifndef INCLUDED_CORE_MAP_BASE_OUTPUT_H
#define INCLUDED_CORE_MAP_BASE_OUTPUT_H
#include "platform/i_platform.h"

#include "i_output.h"
#include <map>
#include <list>
#include <boost/bimap.hpp>
namespace map {

class BaseOutput : public IOutput
{
public:
    BaseOutput();
    ~BaseOutput();
    void PlugInNode( int32_t ordinal, int_function_t node );
    void PlugInNodeId( int32_t id, int_function_t node );
    void DoOutput( int32_t ordinal, int32_t val );
    void DoOutputId( int32_t id, int32_t val );
    BaseOutput& operator=( BaseOutput const& other );
    BaseOutput( BaseOutput const& other );
protected:
    void AddOutputNodeId( int32_t id );
    typedef std::list<int_function_t> PluggedInFunctions_t;
    typedef std::map<int32_t, PluggedInFunctions_t> OutputNodesId_t;
    OutputNodesId_t mOutputNodesId;
    typedef std::vector<PluggedInFunctions_t> OutputNodes_t;
    OutputNodes_t mOutputNodes;
    typedef boost::bimap<int32_t, int32_t> OrdinalIdBimap_t;
    typedef OrdinalIdBimap_t::value_type OrdinalIdPair;
    OrdinalIdBimap_t mOrdinalIdBimap;
    int32_t mNextOrdinal;
};


} // namespace map
#endif//INCLUDED_CORE_MAP_BASE_OUTPUT_H
