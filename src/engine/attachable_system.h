#ifndef INCLUDED_ENGINE_ATTACHABLE_SYSTEM_H
#define INCLUDED_ENGINE_ATTACHABLE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {
namespace ctf {

class AttachableSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( AttachableSystem )
    AttachableSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace ctf
} // namespace engine

#endif//INCLUDED_ENGINE_ATTACHABLE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "attachable_system" -t "attachable_component"
