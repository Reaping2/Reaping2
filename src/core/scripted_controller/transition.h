#ifndef INCLUDED_CORE_SCRIPTED_CONTROLLER_TRANSITION_H
#define INCLUDED_CORE_SCRIPTED_CONTROLLER_TRANSITION_H

#include "core/controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <vector>
#include "i_condition.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/ptr_container/serialize_ptr_vector.hpp"

namespace scriptedcontroller {

struct Transition
{
    typedef boost::ptr_vector<ICondition> Conditions_t;
    Conditions_t mConditions;
    int32_t mTargetStateId = -1;
    void Update( double Seconds );
    void Load( Json::Value const& setters );
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Transition::serialize( Archive& ar, const unsigned int version )
{
    ar & mConditions;
    ar & mTargetStateId;
}

} // namespace scriptedcontroller

#endif//INCLUDED_CORE_SCRIPTED_CONTROLLER_TRANSITION_H
