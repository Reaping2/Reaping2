#ifndef INCLUDED_CORE_I_NOTIFY_PARENT_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_NOTIFY_PARENT_ON_DEATH_COMPONENT_H

#include "component.h"
#include "platform/export.h"

class INotifyParentOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(INotifyParentOnDeathComponent)
    virtual void SetParentGUID(int32_t parentId)=0;
    virtual int32_t GetParentGUID()const=0;
    virtual void SetKillerGUID(int32_t killerId)=0;
    virtual int32_t GetKillerGUID()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void INotifyParentOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(INotifyParentOnDeathComponent, INotifyParentOnDeathComponent,"i_notify_parent_on_death_component");
#endif//INCLUDED_CORE_I_NOTIFY_PARENT_ON_DEATH_COMPONENT_H
