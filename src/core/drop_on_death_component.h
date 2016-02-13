#ifndef INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H

#include "core/i_drop_on_death_component.h"
#include "core/property_loader.h"

class DropOnDeathComponent : public IDropOnDeathComponent
{
public:
    virtual void Update( double Seconds );
    virtual bool IsTriedDrop();
    virtual void SetTriedDrop( bool triedDrop );
    DropOnDeathComponent();
    friend class ComponentFactory;

    bool mTriedDrop;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void DropOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IDropOnDeathComponent>(*this);
    ar & mTriedDrop;
}

class DropOnDeathComponentLoader: public ComponentLoader<DropOnDeathComponent>
{
    virtual void BindValues();
public:
    DropOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H