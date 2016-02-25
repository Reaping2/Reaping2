#ifndef INCLUDED_CORE_I_HEAT_SOURCE_COMPONENT_H
#define INCLUDED_CORE_I_HEAT_SOURCE_COMPONENT_H

#include "component.h"
#include <boost/serialization/export.hpp>

class IHeatSourceComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IHeatSourceComponent)
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IHeatSourceComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IHeatSourceComponent,"i_heat_source_component");
#endif//INCLUDED_CORE_I_HEAT_SOURCE_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_heat_source_component"
