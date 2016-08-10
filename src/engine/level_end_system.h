#ifndef INCLUDED_ENGINE_LEVEL_END_SYSTEM_H
#define INCLUDED_ENGINE_LEVEL_END_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class LevelEndSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(LevelEndSystem)
    LevelEndSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_LEVEL_END_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "level_end_component"
