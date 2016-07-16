#ifndef INCLUDED_MAP_ENTRANCE_TYPE_H
#define INCLUDED_MAP_ENTRANCE_TYPE_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

namespace map {

class EntranceType : public platform::Singleton<EntranceType>
{
protected:
    friend class platform::Singleton<EntranceType>;
    EntranceType();
public:
    enum Type
    {
        Top=0,
        Right,
        Bottom,
        Left,
        Num_Classes
    };
    EntranceType::Type operator()( int32_t Id ) const;
    int32_t operator()( EntranceType::Type type ) const;
private:
    typedef boost::bimap<int32_t,EntranceType::Type> IdToEntranceTypeMap_t;
    IdToEntranceTypeMap_t mIdToEntranceTypeMap;
};

} // namespace map

#endif//INCLUDED_MAP_ENTRANCE_TYPE_H


//command:  "classgenerator.exe" -g "enum" -c "entrance_type" -n "map" -m "Top-top Right-right Bottom-bottom Left-left"
