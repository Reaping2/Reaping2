#ifndef INCLUDED_ENGINE_CONTROLLER_ADAPTER_SYSTEM_H
#define INCLUDED_ENGINE_CONTROLLER_ADAPTER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class ControllerAdapterSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(ControllerAdapterSystem)
    ControllerAdapterSystem();
    void SetCalibrate( bool set );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    bool mCalibrate;
};

} // namespace engine

#endif//INCLUDED_ENGINE_CONTROLLER_ADAPTER_SYSTEM_H
//command:  "../../build-reaping/bin/debug/classgenerator" -g "system" -c "controller_adapter_system" -n "engine"

