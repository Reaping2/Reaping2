#ifndef INCLUDED_CORE_STOP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_STOP_ON_DEATH_COMPONENT_H

#include "i_stop_on_death_component.h"
#include "core/property_loader.h"

class StopOnDeathComponent : public IStopOnDeathComponent
{
public:
    StopOnDeathComponent();
    virtual void SetStopped(bool stopped);
    virtual bool IsStopped()const;
protected:
    friend class ComponentFactory;
    bool mStopped;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void StopOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IStopOnDeathComponent>(*this);
    ar & mStopped;
}

class StopOnDeathComponentLoader : public ComponentLoader<StopOnDeathComponent>
{
    virtual void BindValues();
protected:
    StopOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_STOP_ON_DEATH_COMPONENT_H
