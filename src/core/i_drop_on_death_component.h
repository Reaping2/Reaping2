#ifndef INCLUDED_CORE_I_DROP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_DROP_ON_DEATH_COMPONENT_H
#include "component.h"
#include "platform/export.h"

class IDropOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IDropOnDeathComponent)
    virtual void Update( double Seconds )=0;    
    virtual bool IsTriedDrop()=0;
    virtual void SetTriedDrop( bool triedDrop )=0;
protected:
	friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IDropOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(IDropOnDeathComponent, IDropOnDeathComponent,"i_drop_on_death_component");
#endif//INCLUDED_CORE_I_DROP_ON_DEATH_COMPONENT_H