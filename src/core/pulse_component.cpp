#include "core/pulse_component.h"

PulseComponent::PulseComponent()
    : mPulses()
{
}

void PulseComponent::InitPulses( Pulses_t pulses )
{
    mPulses = pulses;
    for (auto& pulse : mPulses)
    {
        auto const origDuration = pulse.mDuration;
        pulse.mDuration = std::max<double>( 0, 
            pulse.mDuration + 2 * pulse.mVariance * (RandomGenerator::global()() % 100) / 100 - pulse.mVariance );
        pulse.mSpeed = pulse.mSpeed*(origDuration/pulse.mDuration);
    }
}

IPulseComponent::Pulses_t& PulseComponent::GetPulses()
{
    return mPulses;
}

void PulseComponentLoader::BindValues()
{
    PulseComponent::Pulses_t pulses;
    Json::Value const& json = (*mSetters)["pulse_descs"];
    if (!json.isArray())
    {
        return;
    }
    for (auto& part : json)
    {
        IPulseComponent::PulseDesc d;
        Json::GetDouble( part["duration"], d.mDuration );
        Json::GetDouble( part["variance"], d.mVariance );
        Json::GetDouble( part["speed"], d.mSpeed );
        pulses.push_back( d );
    }
    Bind<PulseComponent::Pulses_t>( &PulseComponent::InitPulses, pulses );
}

PulseComponentLoader::PulseComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( PulseComponent, PulseComponent );
