#ifndef INCLUDED_CORE_ROTATE_COMPONENT_SYSTEM_H
#define INCLUDED_CORE_ROTATE_COMPONENT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class RotateComponentSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(RotateComponentSystem)
    RotateComponentSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_CORE_ROTATE_COMPONENT_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "rotate_component" -m "double-speed bool-rotating"
