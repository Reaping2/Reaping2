#ifndef INCLUDED_CORE_ACTIVITY_COMPONENT_H
#define INCLUDED_CORE_ACTIVITY_COMPONENT_H

#include "i_activity_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class ActivityComponent : public IActivityComponent
{
public:
    ActivityComponent();
    virtual void SetRadius( double radius );
    virtual double GetRadius() const;
protected:
    friend class ComponentFactory;
    double mRadius;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ActivityComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IActivityComponent>(*this);
    ar& mRadius;
}

class ActivityComponentLoader : public ComponentLoader<ActivityComponent>
{
    virtual void BindValues();
protected:
    ActivityComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( ActivityComponent, ActivityComponent, "activity_component" );

#endif//INCLUDED_CORE_ACTIVITY_COMPONENT_H

//command:  "../../build-reap-release/bin/relwithdebinfo/classgenerator" -g "component" -c "activity_component" -m "double-radius"
