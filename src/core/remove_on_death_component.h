#ifndef INCLUDED_CORE_REMOVE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_REMOVE_ON_DEATH_COMPONENT_H

#include "core/i_remove_on_death_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class RemoveOnDeathComponent : public IRemoveOnDeathComponent
{
    double mRemainingTime;
public:
    RemoveOnDeathComponent();
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    double GetRemainingTime() const;
    void SetRemainingTime( double rt );
};

template<class Archive>
void RemoveOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IRemoveOnDeathComponent>(*this);
    ar & mRemainingTime;
}

class RemoveOnDeathComponentLoader: public ComponentLoader<RemoveOnDeathComponent>
{
    virtual void BindValues();
public:
    RemoveOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

BOOST_CLASS_EXPORT_KEY2(RemoveOnDeathComponent,"remove_on_death_component");
#endif//INCLUDED_CORE_REMOVE_ON_DEATH_COMPONENT_H
