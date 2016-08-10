#ifndef INCLUDED_CORE_I_LEVEL_END_COMPONENT_H
#define INCLUDED_CORE_I_LEVEL_END_COMPONENT_H

#include "component.h"

class ILevelEndComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ILevelEndComponent)
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ILevelEndComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_LEVEL_END_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "level_end_component"
