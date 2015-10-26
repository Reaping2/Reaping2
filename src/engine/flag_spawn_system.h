#ifndef INCLUDED_ENGINE_FLAG_SPAWN_SYSTEM_H
#define INCLUDED_ENGINE_FLAG_SPAWN_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {
namespace ctf {

class FlagSpawnSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(FlagSpawnSystem)
    FlagSpawnSystem();
    void SetFlagPositionToStart(Actor& actor);
    static Opt<FlagSpawnSystem> Get();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace ctf
} // namespace engine

#endif//INCLUDED_ENGINE_FLAG_SPAWN_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "flag_spawn_system"
