#ifndef INCLUDED_ENGINE_SWITCH_SYSTEM_H
#define INCLUDED_ENGINE_SWITCH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class SwitchSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(SwitchSystem)
    SwitchSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_SWITCH_SYSTEM_H


//command:  "classgenerator.exe" -g "component" -c "switch_component" -m "SwitchState::Type-state double-secsToEnd"
