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
};

class StopOnDeathComponentLoader : public ComponentLoader<StopOnDeathComponent>
{
    virtual void BindValues();
protected:
    StopOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_STOP_ON_DEATH_COMPONENT_H
