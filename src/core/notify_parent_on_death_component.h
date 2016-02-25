#ifndef INCLUDED_CORE_NOTIFY_PARENT_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_NOTIFY_PARENT_ON_DEATH_COMPONENT_H

#include "i_notify_parent_on_death_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class NotifyParentOnDeathComponent : public INotifyParentOnDeathComponent
{
public:
    NotifyParentOnDeathComponent();
    virtual void SetParentGUID(int32_t parentId);
    virtual int32_t GetParentGUID()const;
    virtual void SetKillerGUID(int32_t killerId);
    virtual int32_t GetKillerGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mParentGUID;
    int32_t mKillerGUID;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void NotifyParentOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<INotifyParentOnDeathComponent>(*this);
    ar & mParentGUID;
    ar & mKillerGUID;
}

class NotifyParentOnDeathComponentLoader : public ComponentLoader<NotifyParentOnDeathComponent>
{
    virtual void BindValues();
protected:
    NotifyParentOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(NotifyParentOnDeathComponent,"notify_parent_on_death_component");
#endif//INCLUDED_CORE_NOTIFY_PARENT_ON_DEATH_COMPONENT_H
