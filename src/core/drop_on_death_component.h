#ifndef INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H

#include "core/i_drop_on_death_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

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
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void DropOnDeathComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IDropOnDeathComponent>( *this );
    ar& mTriedDrop;
}

class DropOnDeathComponentLoader: public ComponentLoader<DropOnDeathComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( DropOnDeathComponentLoader )
private:
    virtual void BindValues();
public:
    DropOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( DropOnDeathComponent, DropOnDeathComponent, "drop_on_death_component" );
#endif//INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H
