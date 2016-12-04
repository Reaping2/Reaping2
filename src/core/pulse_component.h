#ifndef INCLUDED_CORE_PULSE_COMPONENT_H
#define INCLUDED_CORE_PULSE_COMPONENT_H

#include "i_pulse_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <boost/serialization/vector.hpp>

class PulseComponent : public IPulseComponent
{
public:
    PulseComponent();
    virtual void InitPulses( Pulses_t pulses );
    virtual Pulses_t& GetPulses();
protected:
    friend class ComponentFactory;
    Pulses_t mPulses;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PulseComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IPulseComponent>(*this);
    ar& mPulses;
}

class PulseComponentLoader : public ComponentLoader<PulseComponent>
{
    virtual void BindValues();
protected:
    PulseComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( PulseComponent, PulseComponent, "pulse_component" );

#endif//INCLUDED_CORE_PULSE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "pulse_component" -m "Pulses_t-pulses"
