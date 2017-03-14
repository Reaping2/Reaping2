#ifndef INCLUDED_CORE_WAYPOINT_COMPONENT_H
#define INCLUDED_CORE_WAYPOINT_COMPONENT_H

#include "i_waypoint_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class WaypointComponent : public IWaypointComponent
{
public:
    WaypointComponent();
    virtual void SetLit( bool lit );
    virtual bool IsLit() const;
protected:
    friend class ComponentFactory;
    bool mLit = false;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void WaypointComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IWaypointComponent>(*this);
    ar& mLit;
}

class WaypointComponentLoader : public ComponentLoader<WaypointComponent>
{
    virtual void BindValues();
protected:
    WaypointComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( WaypointComponent, WaypointComponent, "waypoint_component" );

#endif//INCLUDED_CORE_WAYPOINT_COMPONENT_H


//command:  "classgenerator.exe" -g "component" -c "waypoint_component"
