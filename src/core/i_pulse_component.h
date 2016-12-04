#ifndef INCLUDED_CORE_I_PULSE_COMPONENT_H
#define INCLUDED_CORE_I_PULSE_COMPONENT_H

#include "component.h"

class IPulseComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IPulseComponent)

    struct PulseDesc
    {
        // maximum duration this pulse goes in seconds
        double mDuration = 0.0;
        // store some data corresponding to this pulse item. (speed)
        double mSpeed = 0.0;

        // variance to duration. Calculated at init.
        double mVariance = 0.0;
        // current state of duration in seconds
        double mDurationCurrent = 0.0;
        // true - increase, false - decrease
        bool mDirection = true;

        friend class ::boost::serialization::access;
        template<class Archive>
        void serialize( Archive& ar, const unsigned int version );
    };

    typedef std::vector<PulseDesc> Pulses_t;
    virtual void InitPulses(Pulses_t pulses)=0;
    virtual Pulses_t& GetPulses()=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IPulseComponent::PulseDesc::serialize( Archive& ar, const unsigned int version )
{
    ar & mDuration;
    ar & mSpeed;
}

template<class Archive>
void IPulseComponent::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_PULSE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "pulse_component" -m "Pulses_t-pulses"
