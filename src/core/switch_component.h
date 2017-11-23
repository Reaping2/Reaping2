#ifndef INCLUDED_CORE_SWITCH_COMPONENT_H
#define INCLUDED_CORE_SWITCH_COMPONENT_H

#include "i_switch_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class SwitchComponent : public ISwitchComponent
{
public:
    SwitchComponent();
    virtual void SetState( SwitchState::Type state );
    virtual SwitchState::Type GetState() const;
    virtual void SetSecsToEnd( double secsToEnd );
    virtual double GetSecsToEnd() const;
    virtual void SetSecsToEndMax( double secsToEndMax );
    virtual double GetSecsToEndMax() const;
    virtual void SetStateChanged( bool stateChanged );
    virtual bool IsStateChanged() const;
protected:
    friend class ComponentFactory;
    SwitchState::Type mState;
    double mSecsToEnd;
    double mSecsToEndMax;
    bool mStateChanged;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void SwitchComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ISwitchComponent>(*this);
    ar& mState;
    ar& mSecsToEnd;
    ar& mSecsToEndMax;
    ar& mStateChanged;
}

class SwitchComponentLoader : public ComponentLoader<SwitchComponent>
{
    virtual void BindValues();
protected:
    SwitchComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( SwitchComponent, SwitchComponent, "switch_component" );

#endif//INCLUDED_CORE_SWITCH_COMPONENT_H


//command:  "classgenerator.exe" -g "component" -c "switch_component" -m "SwitchState::Type-state double-secsToEnd"
