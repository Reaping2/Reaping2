#ifndef INCLUDED_SCRIPTEDCONTROLLER_SELECTION_TYPE_H
#define INCLUDED_SCRIPTEDCONTROLLER_SELECTION_TYPE_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

namespace scriptedcontroller {

class SelectionType : public platform::Singleton<SelectionType>
{
protected:
    friend class platform::Singleton<SelectionType>;
    SelectionType();
public:
    enum Type
    {
        Current=0,
        LastHitBy,
        Closest,
        Num_Classes
    };
    SelectionType::Type operator()( int32_t Id ) const;
    int32_t operator()( SelectionType::Type type ) const;
private:
    typedef boost::bimap<int32_t,SelectionType::Type> IdToSelectionTypeMap_t;
    IdToSelectionTypeMap_t mIdToSelectionTypeMap;
};

} // namespace scriptedcontroller

#endif//INCLUDED_SCRIPTEDCONTROLLER_SELECTION_TYPE_H


//command:  "classgenerator.exe" -g "enum" -n "scriptedcontroller" -c "selection_type" -m "Current-current LastHitBy-last_hit_by Closest-closest"
