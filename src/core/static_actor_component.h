#ifndef INCLUDED_CORE_STATIC_ACTOR_COMPONENT_H
#define INCLUDED_CORE_STATIC_ACTOR_COMPONENT_H

#include "i_static_actor_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class StaticActorComponent : public IStaticActorComponent
{
public:
    StaticActorComponent();
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void StaticActorComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IStaticActorComponent>(*this);
}

class StaticActorComponentLoader : public ComponentLoader<StaticActorComponent>
{
    virtual void BindValues();
protected:
    StaticActorComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( StaticActorComponent, StaticActorComponent, "static_actor_component" );

#endif//INCLUDED_CORE_STATIC_ACTOR_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "static_actor_component"
