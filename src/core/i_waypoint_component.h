#ifndef INCLUDED_CORE_I_WAYPOINT_COMPONENT_H
#define INCLUDED_CORE_I_WAYPOINT_COMPONENT_H

#include "component.h"

class IWaypointComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IWaypointComponent)
    virtual void SetLit( bool lit ) = 0;
    virtual bool IsLit()const = 0;

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IWaypointComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_WAYPOINT_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "waypoint_component"
