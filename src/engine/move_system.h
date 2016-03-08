#ifndef INCLUDED_ENGINE_MOVE_SYSTEM_H
#define INCLUDED_ENGINE_MOVE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class MoveSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( MoveSystem )
    MoveSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_MOVE_SYSTEM_H
