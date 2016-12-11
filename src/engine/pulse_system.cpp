#include "platform/i_platform.h"
#include "pulse_system.h"
#include "core/i_pulse_component.h"

namespace engine {

PulseSystem::PulseSystem()
    : mScene( Scene::Get() )
{
}


void PulseSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IPulseComponent>().IsValid(); } );
}


void PulseSystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
       Opt<IPulseComponent> pulseC(actor->Get<IPulseComponent>());
       for (auto& pulse : pulseC->GetPulses())
       {
           pulse.mDurationCurrent += (pulse.mDirection?1:-1)*DeltaTime;
           if (pulse.mDurationCurrent > pulse.mDuration)
           {
               pulse.mDurationCurrent = pulse.mDuration;
               pulse.mDirection = !pulse.mDirection;
           }
           else if (pulse.mDurationCurrent < 0.0)
           {
               pulse.mDurationCurrent = 0.0;
               pulse.mDirection = !pulse.mDirection;
           }
       }
    }
}


} // namespace engine

