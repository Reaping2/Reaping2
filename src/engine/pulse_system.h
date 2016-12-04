#ifndef INCLUDED_ENGINE_PULSE_SYSTEM_H
#define INCLUDED_ENGINE_PULSE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class PulseSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(PulseSystem)
    PulseSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_PULSE_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "pulse_component" -m "Pulses_t-pulses"
